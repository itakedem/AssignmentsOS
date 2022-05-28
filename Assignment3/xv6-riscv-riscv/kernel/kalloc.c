// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

#define NUM_PYS_PAGES ((PHYSTOP-KERNBASE) / PGSIZE)
void freerange(void *pa_start, void *pa_end);
extern uint64 cas(volatile void *addr, int expected, int newval);
extern char end[]; // first address after kernel.
// defined by kernel.ld.

struct run {
    struct run *next;
};

struct {
    struct spinlock lock;
    struct run *freelist;
    uint page_ref_count[NUM_PYS_PAGES];
} kmem;

void
kinit()
{
    initlock(&kmem.lock, "kmem");
    memset(kmem.page_ref_count, 0, sizeof(kmem.page_ref_count));
    freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
    char *p;
    p = (char*)PGROUNDUP((uint64)pa_start);
    for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE){
        kmem.page_ref_count[(((uint64)p-KERNBASE) / PGSIZE)] = 1;
        kfree(p);
    }
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
    struct run *r;

    if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
        panic("kfree");

    dec_reference_count((uint64)pa);
    if (get_reference_count((uint64)pa) > 0)
        return;

    // Fill with junk to catch dangling refs.
    memset(pa, 1, PGSIZE);

    r = (struct run*)pa;

    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
    struct run *r;

    acquire(&kmem.lock);
    r = kmem.freelist;
    if(r)
        kmem.freelist = r->next;
    release(&kmem.lock);

    if(r)
        memset((char*)r, 5, PGSIZE); // fill with junk

    if(r)
        inc_reference_count((uint64)r);

    return (void*)r;
}


void inc_reference_count(uint64 pa){
    uint64 entry = (pa-KERNBASE) / PGSIZE;
    while(cas(kmem.page_ref_count + entry, kmem.page_ref_count[entry], kmem.page_ref_count[entry] + 1));
}

void dec_reference_count(uint64 pa){
    uint64 entry = (pa-KERNBASE) / PGSIZE;
    while(cas(kmem.page_ref_count + entry, kmem.page_ref_count[entry], kmem.page_ref_count[entry] - 1));
}

int get_reference_count(uint64 pa){
    return kmem.page_ref_count[(pa-KERNBASE) / PGSIZE];
}