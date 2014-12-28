#include "api.h"

const struct run_api RUN_API = {
  .init = init,
  .finalize = finalize,
  .reload = reload,
  .unload = unload,
  .step = step
};
  
