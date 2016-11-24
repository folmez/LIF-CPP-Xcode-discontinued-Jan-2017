#include "typedefs.h"
#include "small_functions.h"

Potential_t rk2(Time_t dt, Potential_t v0, double a0, double a1,
   double b0, double b1, Potential_t vI)
{
   Flag_t enable_assert = true;

   Potential_t v1;
   double k1, k2;

   k1 = evaluate_k(v0, a0, b0);
   k2 = evaluate_k(v0 + k1*dt, a1, b1);
   v1 = v0 + (k1+k2) * dt * 0.5 ;

   if (enable_assert)
   {
      // Potential must not be less than vI!
      assert( v1 >= vI);
   }

   return v1;
}
