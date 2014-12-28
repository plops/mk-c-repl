// http://nullprogram.com/blog/2014/12/23/

#include "api.h"

const char *RUN_LIBRARY = "./librun.so";

struct run {
  void *handle;
  ino_t id;
  struct run_api api;
  struct run_state *state;
};
