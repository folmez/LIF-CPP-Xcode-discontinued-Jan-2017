#include "My_Random_Number_Generator.h"
#include "typedefs.h"
#include "small_functions.h"

void evolve_potential(Time_t t_current, Time_t dt, Conductance_t get0[],
   Conductance_t git0[], Conductance_t get1[], Conductance_t git1[],
   Potential_t v0[], Number_t en, Number_t in, Number_t A[],
   Scale_t eneff, Scale_t ineff, Number_t eori[], Time_t lft[],
   Number_t single_track_id, Current_t extI, Potential_t vE,
   Potential_t vI, Potential_t vL, Conductance_t gL, Potential_t vThresh,
   Rate_t SE, Rate_t SI, Rate_t tau_r_AMPA, Rate_t tau_d_AMPA,
   Rate_t tau_r_GABA, Rate_t tau_d_GABA, Time_t tref,
   Number_t rs[], Potential_t v1[],
   std::vector<Number_t> &rast_ID, std::vector<Time_t> &rast_firing_time,
   Conductance_t genJump[], Conductance_t henJump[],
   Conductance_t ginJump[], Conductance_t hinJump[], Time_t t_next)
{
   Flag_t enable_assert = true;
   Flag_t display_track_progress_single_neuron = false;

   std::cout << std::fixed << std::setprecision(4);

   // Display a header line for Poisson spikes
   if ( display_track_progress_single_neuron && t_current == 0.0 )
   {
      std::cout << "\tID\tTime\tEvent\t v(t)\tge-tot\tgi-tot" << std::endl;
      std::cout << "\t" << single_track_id+1;
      std::cout << "\t" << 0;
      std::cout << "\t";
      std::cout << "\t" << v0[single_track_id];
      std::cout << "\t" << get0[single_track_id];
      std::cout << "\t" << git0[single_track_id] << std::endl;
   }

   Number_t n = en+(-1)*in;
   double a0, a1, b0, b1;
   Time_t firing_time;
   Time_t firing_time_post_tref;
   Number_t firing_round_counter;
   Conductance_t hNew;
   Conductance_t gNew;

   for (int i = 0; i < n; i++)
   {
      genJump[i] = 0.0;
      henJump[i] = 0.0;
      ginJump[i] = 0.0;
      hinJump[i] = 0.0;
   }

   // Evolve potentials of all neurons
   for (int i = 0; i < n; i++)
   {

      // Calculate RK2 parameters
      a0 = get_alpha(get0[i], git0[i], gL);
      a1 = get_alpha(get1[i], git1[i], gL);
      b0 = get_beta(get0[i], git0[i], gL, vE, vI, vL, extI);
      b1 = get_beta(get1[i], git1[i], gL, vE, vI, vL, extI);

      // Evolve potentital based on the refractory state
      if ( rs[i] == 0 )       // Evolve potentials of neurons that are not in
                              // refractory period
      {
         // Use modified RK2 to evolve their potentials
         v1[i] = rk2(dt, v0[i], a0, a1, b0, b1, vI);
      }
      else if ( rs[i] == -1 ) // Evolve potentials of neurons that exit
                              // refractory period in this time step using
                              // Shelley and Tao, 2001
      {
         // Transpose their spike times according to the refractory period
         firing_time = lft[i] + tref - t_current;
         // Use Shelley and Tao 2001 to evolve their potentials accordingly
         v1[i] = shelley_and_tao_2001(dt, firing_time, a0,a1,b0,b1, vI, vL);
         // Reset refractory state
         rs[i] = 0;
      }
      else if ( rs[i] == 1 )  // Update refractory state of neurons whose
                              // refractory states are 1, i.e. neurons which
                              // will not fire in this time step, to -1
                              // if they exit refractory period in the next
                              // time step
      {
         if ( lft[i] + tref <= t_current + 2*dt )
         { rs[i] = -1; }
         v1[i] = vL;
      }

      // Display potential at t+dt
      if ( display_track_progress_single_neuron && i == single_track_id )
      {
         std::cout << "\t" << single_track_id+1;
         std::cout << std::setprecision(2);
         std::cout << "\t" << t_next;
         std::cout << std::setprecision(4);
         std::cout << "\t";
         std::cout << "\t" << v1[single_track_id];
         std::cout << "\t" << get1[single_track_id];
         std::cout << "\t" << git1[single_track_id] << std::endl;
      }

      // Calibrate firing events using Shelley and Tao, 2001
      firing_round_counter = 0;
      while ( v1[i] > vThresh )
      {
         firing_round_counter = firing_round_counter+1;
         // Assert that a neuron fires at most once in one time step
         if (enable_assert)
         {
            // Choose smaller dt! A neuron fires twice!
            assert( firing_round_counter <= 1);
         }
         // Find the firing time in [0,dt]
         firing_time = (vThresh-v0[i]) / (v1[i]-v0[i])*dt;

         if ( firing_time + tref <= dt )
         // Refractory period ends during the current time step
         {
            // The neuron's potential can only start rising at the end of the
            // refractory period. Set the refractory state to 0.
            firing_time_post_tref = firing_time + tref;
            v1[i] = shelley_and_tao_2001(dt, firing_time_post_tref,
               a0, a1, b0, b1, vI, vL);
            rs[i] = 0;
         }
         else if ( dt < firing_time + tref && firing_time + tref <= 2*dt )
         // Refractory period will end during the next time step. Set the
         // refractory state to -1
         {
            v1[i] = vL;
            rs[i] = -1;
         }
         else if ( 2*dt < firing_time + tref )
         // Refractory period will sometime in the future. Set the refractory
         // state to 1 and set the potential to reset potential
         {
            v1[i] = vL;
            rs[i] = 1;
         }

         // Update the last firing time in [t, t+dt]
         lft[i] = t_current + firing_time;

         // Display calibrated potential if tracked neuron has fired
         if ( display_track_progress_single_neuron && i == single_track_id )
         {
            std::cout << "\t";
            std::cout << "\t";
            std::cout << "\tFIRED";
            std::cout << "\t" << v1[single_track_id] << std::endl;
         }

         // Record firing times
         rast_ID.push_back(i);
         rast_firing_time.push_back(lft[i]);

         // Evaluate post synaptic impact of the firing
         if ( firing_round_counter > 0 )  // if neuron i fires
         {
            gNew = 0.0;
            hNew = 0.0;
            if ( eori[i] == 1 )     // if i is an excitatory neuron
            {
               hNew = ( SE/eneff ) / tau_r_AMPA;
               analytic(dt-firing_time, tau_d_AMPA, tau_r_AMPA,
                        gNew, hNew);
            }
            else if (eori[i] == -1) // if i is an inhibitory neuron
            {
               hNew = ( SI/ineff ) / tau_r_GABA;
               analytic(dt-firing_time, tau_d_GABA, tau_r_GABA,
                        gNew, hNew);
            }

            for (int j = 0; j < n; j++)
            {
               if ( A[i*n+j] == 1 )        // if j is a neighbor of i
               {
                  if ( eori[i] == 1 )     // if i is an excitatory neuron
                  {
                     genJump[j] = genJump[j] + gNew;
                     henJump[j] = henJump[j] + hNew;
                     if ( display_track_progress_single_neuron &&
                        j == single_track_id )
                     {
                        std::cout << "\tRECEIVED EXCITATORY FIRING" << std::endl;
                     }
                  }
                  else if (eori[i] == -1) // if i is an inhibitory neuron
                  {
                     ginJump[j] = ginJump[j] + gNew;
                     hinJump[j] = hinJump[j] + hNew;
                     if ( display_track_progress_single_neuron &&
                        j == single_track_id )
                     {
                        std::cout << "\tRECEIVED INHIBITORY FIRING" << std::endl;
                     }
                  }
               }
            }
         }
      }
   }
}
