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
  void *handle = dlopen(RUN_LIBRARY,RTLD_NOW);
  if(!handle){
    run->handle = NULL;
    run->id = 0;
    printf("error during dlopen.\n");
    return;
  }
  run->handle = handle;
  run->id = attr.st_ino;
  const struct run_api *api =
    dlsym(run->handle,"RUN_API");
  if(api == NULL){
    dlclose(run->handle);
    run->handle = NULL;
    run->id = 0;
    printf("error during dlsym of api struct.\n");
    return ;
  }

  run->api = *api;
  if(run->state == NULL)
    run->state = run->api.init();
  run->api.reload(run->state);
}

int main(void)
{
  struct run run = {0};
  for(;;){
    load_if_new_lib(&run);
    if(run.handle)
      if(!run.api.step(run.state))
	break;
    usleep(32000);
  }
  run_unload(&run);
  return 0;
}
