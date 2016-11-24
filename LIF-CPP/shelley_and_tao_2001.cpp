#include <cassert>
#include "typedefs.h"
#include "small_functions.h"

Potential_t shelley_and_tao_2001(Time_t dt, Time_t firing_time,
   double a0, double a1, double b0, double b1,
   Potential_t vI, Potential_t vL)
{
   Flag_t enable_assert = true;

   if (enable_assert)
   {
       // Firing time must lie outsite [t, t+dt]!
       assert( firing_time >= 0);
       assert( firing_time <= dt);
   }

   Potential_t v0;
   Potential_t v1;
   v0 = ( vL - 0.5*firing_time * ( b0 + b1 - a1*b0*dt ) ) /
            ( 1 + 0.5*firing_time * ( (-1)*a0 + (-1)*a1 + a1*a0*dt ) );
   v1 = rk2(dt, v0, a0, a1, b0, b1, vI);

   if (enable_assert)
   {
       // Potential must not be less than vI!
       assert( v1 >= vI);
   }

   return v1;
}
