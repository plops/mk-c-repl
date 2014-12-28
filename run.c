#include "api.h"
#include <stdio.h>

struct run_state{
  int a;
};

struct run_state run_state;

void init(struct run_state *state)
{
  printf("init\n");
  run_state.a = 10;
  return &run_state;
}
void finalize(struct run_state *state)
{
  printf("finalize\n");
}
void reload(struct run_state *state)
{
  printf("reload\n");
}
void unload(struct run_state *state)
{
  printf("unload\n");
}
int step(struct run_state *state)
{
  printf("step\n");
  state->a --;
  return state->a;
}


const struct run_api RUN_API = {
  .init = init,
  .finalize = finalize,
  .reload = reload,
  .unload = unload,
  .step = step
};


