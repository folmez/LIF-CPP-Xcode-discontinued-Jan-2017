#include "typedefs.h"

Conductance_t get_beta(Conductance_t ge, Conductance_t gi, Conductance_t gL,
   Potential_t vE, Potential_t vI, Potential_t vL, Current_t extI)
{
      return ge * vE + gi * vI + gL * vL + extI;
}
