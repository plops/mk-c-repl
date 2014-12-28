#include "api.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const struct run_api RUN_API = {
  .init = init,
  .finalize = finalize,
  .reload = reload,
  .unload = unload,
  .step = step
};

void reload_if_new_lib(struct run*run)
{
  struct stat attr;
  if(stat(GAME_LIBRARY,&attr)!=0){
    printf("game library doesn't exist.\n");
    return;
  }
  if (game->id == attr.st_ino) { // note: id is initially 0 and the
				 // file should therefore be loaded at
				 // first call
    printf("file didn't change\n");
    return;
  }
  if(run->handle){
    run->api.unload(run->state);
    dlclose(run->handle);
  }
  void *handle = dlopen(GAME_LIBRARY,RTLD_NOW);
  
}

