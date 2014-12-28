// http://nullprogram.com/blog/2014/12/23/

#include "api.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
 #include <dlfcn.h>

const char *RUN_LIBRARY = "./librun.so";

struct run {
  void *handle;
  ino_t id;
  struct run_api api;
  struct run_state *state;
};

static void run_unload(struct run*run)
{
  if(run->handle){
    run->api.unload(run->state);
    dlclose(run->handle);
  }
}

static void run_load_if_new_lib(struct run*run)
{
  struct stat attr;
  if(stat(RUN_LIBRARY,&attr)!=0){
    printf("run library doesn't exist.\n");
    return;
  }
  if (run->id == attr.st_ino) { // note: id is initially 0 and the
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
    run_load_if_new_lib(&run);
    if(run.handle)
      if(!run.api.step(run.state))
	break;
    usleep(32000);
  }
  run_unload(&run);
  return 0;
}
