#include "typedefs.h"
#include <math.h>

void analytic(Time_t dt, Rate_t tau_d, Rate_t tau_r,
   Conductance_t &g, Conductance_t &h)
{
	Time_t etd = exp(-dt/tau_d);
    Time_t etr = exp(-dt/tau_r);
	Scale_t  c = (tau_r/(tau_d-tau_r)) * (etd-etr);
	g = ( g * etd ) + ( c * h );
	h = h * etr;
}
