#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3 && (argc != 4 || (argc == 4 && !strcmp(argv[2], "-s")))){  //check if hard\symbol link legal
    fprintf(2, "Usage hard link: ln old new\n Usage symbol link: ln -s old new \n");
    exit(1);
  }

  if(argc == 3 && link(argv[1], argv[2]) < 0)
    fprintf(2, "link %s %s: failed\n", argv[1], argv[2]);

  if(argc == 4 && symlink(argv[2], argv[3]) < 0)
      fprintf(2, "symbol link %s %s: failed\n", argv[2], argv[3]);

  exit(0);
}
