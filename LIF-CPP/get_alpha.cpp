#include "typedefs.h"

Conductance_t get_alpha(Conductance_t ge, Conductance_t gi,
   Conductance_t gL)
{
      return ge + gi + gL;
}
