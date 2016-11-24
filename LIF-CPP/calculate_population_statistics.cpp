#include "typedefs.h"
#include "My_Random_Number_Generator.h"
#include <cassert>

void calculate_population_statistics(Number_t n, Number_t en, Number_t in,
                                     Number_t nr_rps[], Time_t t_unit, Time_t t_end,
                                     std::vector<Number_t> rast_ID, std::vector<Time_t> rast_firing_time,
                                     Time_t warm_up_time, std::string network_type, Probability_t params,
                                     Time_t ref, Rate_t SE, Rate_t SI, Time_t dt, Current_t extI, Rate_t fS,
                                     Number_t external_spike_type_num, Rate_t external_spike_rate,
                                     Scale_t eneff, Scale_t ineff, Time_t tref,
                                     Potential_t v1[], Conductance_t gep1[], Conductance_t gen1[],
                                     Conductance_t gin1[],
                                     Conductance_t &exc_poisson_input, Conductance_t &inh_poisson_input,
                                     Conductance_t &tot_poisson_input, Rate_t &exc_firing_rate,
                                     Rate_t &inh_firing_rate, Rate_t &tot_firing_rate, Potential_t &avg_exc_v,
                                     Potential_t &avg_inh_v, Potential_t &avg_v, Conductance_t &avg_exc_gep,
                                     Conductance_t &avg_inh_gep, Conductance_t &avg_gep,
                                     Conductance_t &avg_exc_gen, Conductance_t &avg_inh_gen,
                                     Conductance_t &avg_gen, Conductance_t &avg_exc_gin,
                                     Conductance_t &avg_inh_gin, Conductance_t &avg_gin)
{
   Flag_t display_input_output_summary = true;
   double sum;
   
   // Excitatory Poisson input
   sum = 0;
   for (int i = 0; i < en; i++) { sum = sum + nr_rps[i]; }
   exc_poisson_input = ( sum * t_unit / t_end ) / en;
   
   // Inhibitory Poisson input
   sum = 0;
   for (int i = en; i < n; i++) { sum = sum + nr_rps[i]; }
   inh_poisson_input = ( sum * t_unit / t_end ) / ((-1)*in);
   
   // Total Poisson input
   sum = 0;
   for (int i = 0; i < n; i++) { sum = sum + nr_rps[i]; }
   tot_poisson_input = ( sum * t_unit / t_end ) / n;
   
   
   
   assert( warm_up_time <= t_end && "FO: Warm-up time must be less than t_end");
   
   // Excitatory firing rate
   sum = 0;
   for (int i = 0; i < rast_ID.size(); i++)
   {      
      if ( rast_ID[i] < en && rast_firing_time[i] > warm_up_time )
      { sum = sum+1; }
   }
   exc_firing_rate = sum * ( t_unit / ( t_end - warm_up_time ) ) / en;
   
   // Inhibitory firing rate
   sum = 0;
   for (int i = 0; i < rast_ID.size(); i++)
   {
      if ( rast_ID[i] >= en && rast_firing_time[i] > warm_up_time )
      { sum = sum+1; }
   }
   inh_firing_rate = sum * ( t_unit / ( t_end - warm_up_time ) ) / ((-1)*in);
   
   // Total firing rate
   sum = 0;
   for (int i = 0; i < rast_ID.size(); i++)
   {
      if ( rast_firing_time[i] > warm_up_time )
      { sum = sum+1; }
   }
   tot_firing_rate = sum * ( t_unit / ( t_end - warm_up_time ) ) / n;

   
   
   // Average excitatory potential
   sum = 0;
   for (int i = 0; i < en; i++) { sum = sum + v1[i]; }
   avg_exc_v = sum / en;
   
   // Average inhitatory potential
   sum = 0;
   for (int i = en; i < n; i++) { sum = sum + v1[i]; }
   avg_inh_v = sum / ((-1)*in);
   
   // Average potential
   sum = 0;
   for (int i = 0; i < n; i++) { sum = sum + v1[i]; }
   avg_v = sum / n;
   
   
   
   // Average excitatory poisson conductance
   sum = 0;
   for (int i = 0; i < en; i++) { sum = sum + gep1[i]; }
   avg_exc_gep = sum / en;
   
   // Average inhibitory poisson conductance
   sum = 0;
   for (int i = en; i < n; i++) { sum = sum + gep1[i]; }
   avg_inh_gep = sum / ((-1)*in);
   
   // Average poisson conductance
   sum = 0;
   for (int i = 0; i < n; i++) { sum = sum + gep1[i]; }
   avg_gep = sum / n;
   
   
   
   // Average excitatory exc.-network conductance
   sum = 0;
   for (int i = 0; i < en; i++) { sum = sum + gen1[i]; }
   avg_exc_gen = sum / en;
   
   // Average inhibitory exc.-network conductance
   sum = 0;
   for (int i = en; i < n; i++) { sum = sum + gen1[i]; }
   avg_inh_gen = sum / ((-1)*in);
   
   // Average exc.-network conductance
   sum = 0;
   for (int i = 0; i < n; i++) { sum = sum + gen1[i]; }
   avg_gen = sum / n;
   
   
   
   // Average excitatory inh.-network conductance
   sum = 0;
   for (int i = 0; i < en; i++) { sum = sum + gin1[i]; }
   avg_exc_gin = sum / en;
   
   // Average inhibitory inh.-network conductance
   sum = 0;
   for (int i = en; i < n; i++) { sum = sum + gin1[i]; }
   avg_inh_gin = sum / ((-1)*in);
   
   // Average inh.-network conductance
   sum = 0;
   for (int i = 0; i < n; i++) { sum = sum + gin1[i]; }
   avg_gin = sum / n;
   
   
   
   if ( display_input_output_summary )
   {
      // Display inputs
      std::cout << "               ----------------------------------\n";
      std::cout << "               | LIF network simulation summary |\n";
      std::cout << "               ----------------------------------\n";
      std::cout << "\t               Network: " << network_type << " (" << params << ")" << std::endl;
      std::cout << "\t          [#exc, #inh]: [" << en << ", " << in << "]" << std::endl;
      std::cout << "\t                  Neff: [" << eneff<< ", " << ineff << "]" << std::endl;
      std::cout << "\t       Refractory time: " << tref << " ms" << std::endl;
      std::cout << "\t     Cortical strength: [" << SE << ", " << SI << "]" << std::endl;
      std::cout << "\t                    dt: " << dt << " ms" << std::endl;
      std::cout << "\t                  Time: " << t_end << " ms" << std::endl;
      std::cout << "\t               Warm-up: " << warm_up_time << " ms" << std::endl;
      std::cout << "\t    Ext. input current: " << extI << " per sec" << std::endl;
      if ( external_spike_type_num == 1 )
      {
         std::cout << "\t       Ext. input type: Poisson (rate: " << external_spike_rate << ", " <<  "strength: " << fS << ")" << std::endl;
      }
      else if ( external_spike_type_num == 2 )
      {
         std::cout << "\t       Ext. input type: Deterministic (rate: " << external_spike_rate << ", " <<  "strength: " << fS << ")" << std::endl;
      }
      std::cout << std::endl;
      // Display outputs
      std::cout << "\t Poisson input to exc.: " << exc_poisson_input << std::endl;
      std::cout << "\t Poisson input to inh.: " << inh_poisson_input << std::endl;
      std::cout << "\t   Total Poisson input: " << tot_poisson_input << std::endl;
      std::cout << std::endl;
      std::cout << "\tExcitatory firing rate: " << exc_firing_rate << std::endl;
      std::cout << "\tInhibitory firing rate: " << inh_firing_rate << std::endl;
      std::cout << "\t     Total firing rate: " << tot_firing_rate << std::endl;
      std::cout << "\tAverage exc. potential: " << avg_exc_v << std::endl;
      std::cout << "\tAverage inh. potential: " << avg_inh_v << std::endl;
      std::cout << "\t     Average potential: " << avg_v << std::endl;
      std::cout << std::endl;
      std::cout << "\t      Average exc. gep: " << avg_exc_gep << std::endl;
      std::cout << "\t      Average inh. gep: " << avg_inh_gep << std::endl;
      std::cout << "\t           Average gep: " << avg_gep << std::endl;
      std::cout << std::endl;
      std::cout << "\t      Average exc. gen: " << avg_exc_gen << std::endl;
      std::cout << "\t      Average inh. gen: " << avg_inh_gen << std::endl;
      std::cout << "\t           Average gen: " << avg_gen << std::endl;
      std::cout << std::endl;
      std::cout << "\t      Average exc. gin: " << avg_exc_gin << std::endl;
      std::cout << "\t      Average inh. gin: " << avg_inh_gin << std::endl;
      std::cout << "\t           Average gin: " << avg_gin << std::endl;
   }
   
   
   
   
   
   
   
}