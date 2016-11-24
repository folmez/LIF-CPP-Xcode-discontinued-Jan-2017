#include "My_Random_Number_Generator.h"
#include "typedefs.h"
#include "small_functions.h"

void evolve_ge_poisson_input(Time_t t_current, Time_t dt, Conductance_t gep1[],
   Conductance_t hep[], Number_t nr_rps[], Time_t lpst[],
   Rate_t tau_r_AMPA, Rate_t tau_d_AMPA, Rate_t fS, Rate_t fR,
   Rate_t rate, Number_t single_track_id, Number_t n, Time_t t_next,
   My_Random_Number_Generator_t RNG, Number_t external_spike_type_num)
{
   Flag_t display_track_progress_single_neuron = false;
   Flag_t use_daiwei_jump = true;
   
   Time_t Tprev;

   // Display a header line for Poisson spikes
   if ( display_track_progress_single_neuron && t_current == 0.0 )
   { std::cout << "\tID\tTime\tEvent\t#spikes\tgep\thep" << std::endl; }

   // Evolve
   for (int i = 0; i < n; i++)
   {
      Tprev =  t_current;
      while ( lpst[i] < t_next )
      {
         // Evolve g and h to the next spiking time
         analytic(lpst[i]-Tprev, tau_d_AMPA, tau_r_AMPA, gep1[i], hep[i]);
         // Make the Poisson jump for the next spiking time
         if ( use_daiwei_jump )
         { hep[i] = hep[i] + fS/tau_r_AMPA; }
         else
         { hep[i] = hep[i] + fS/tau_d_AMPA; }
         // Increase number of received Poisson spikes by 1
         nr_rps[i] = nr_rps[i]+1;
         // Update previous time
         Tprev = lpst[i];
         // Generate new spike time
         lpst[i] = lpst[i] + RNG.next_spike_waiting_time(rate, external_spike_type_num);
         // Display evolution if tracked neuron received a spike
         if ( display_track_progress_single_neuron && i == single_track_id )
         {
            std::cout << std::fixed << std::setprecision(4);
            std::cout << "\t" << single_track_id+1;
            std::cout << "\t" << Tprev;
            std::cout << "\t" << "Spike";
            std::cout << "\t" << nr_rps[single_track_id];
            std::cout << "\t" << gep1[single_track_id];
            std::cout << "\t" << hep[single_track_id] << std::endl;
         }
      }
      // Last evolution from previous time (Tprev) to t+dt
      analytic(t_next-Tprev, tau_d_AMPA, tau_r_AMPA, gep1[i], hep[i]);
      // Display final evolution
      if ( display_track_progress_single_neuron && i == single_track_id )
      {
         std::cout << "\t" << single_track_id+1;
         std::cout << "\t" << t_next;
         std::cout << "\t" << " ";
         std::cout << "\t" << " ";
         std::cout << "\t" << gep1[single_track_id];
         std::cout << "\t" << hep[single_track_id] << std::endl;
      }
   }
}
