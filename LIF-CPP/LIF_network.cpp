#include "My_Random_Number_Generator.h"
#include "typedefs.h"
#include "small_functions.h"

int main()
{
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////// Initialization //////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    My_Random_Number_Generator_t RNG;
    
    Number_t en;                // Number of excitatory neurons
    Number_t in;                // Number of inhibitory neurons
    std::string network_type;   // "ER" or ...
    Probability_t params;       // Network parameters
    
    Time_t t_end;               // Total simulation time (ms)
    Time_t dt;                  // Time-step length (ms)
    Time_t tref;                // Refractory period
    Time_t warm_up_time;        // Warm-up time is ignored from stats
    Number_t nr_pops;           // # of populations
    Number_t sn;                // # of sampled neurons from each pop
    
    Current_t extI;             // External input current
    Rate_t fS;
    Rate_t fR;
    Rate_t SE;
    Rate_t SI;
    
    Number_t external_spike_type_num;   // External spike type:
                                        // 1-Poisson, 2-Deterministic
    Rate_t external_spike_rate;         // External spike input rate
                                        // (per sec) to each neuron
    
    read_params(en, in, network_type, t_end, dt, params,
                external_spike_type_num, external_spike_rate,
                tref, warm_up_time, nr_pops, sn, extI,
                fS, fR, SE, SI);
    
    Number_t n = en+(-1)*in;    // Number of all neurons

    //    en=20;
    //    in=-20;
    //    network_type = "ER";
    //    t_end=500;
    //    dt=0.1;
    //    params=1;
    //    external_spike_type_num=1;
    //    external_spike_rate=4000;
    //  tref=0.0;         // Refractory period
    //  warm_up_time=0;           // Warm-up time is ignored from stats
    //  nr_pops=2;           // # of populations
    //  sn=1;           // # of sampled neurons from each pop
    
    //  fS=0.005;
    //  fR=1.0;
    //  SE=0.05;
    //  SI=0.5;
    //  extI=0.0;         // External input current
    
    
    Time_t t_unit        = 1000.0;      // Time conversion factor from ms to s
    Potential_t vE       = 14.0/3;      // Excitatory synaptic reversal potential
    Potential_t vI       = -2.0/3;      // Inhibitory synaptic reversal potential
    Potential_t vL       = 0.0;         // Reversal (Reset) potential for leakage
    Conductance_t gL     = 50.0/t_unit; //
    Potential_t vThresh  = 1.0;         // Normalized membrane potential threshold
    
    Rate_t tau_r_AMPA    = 1.0;
    Rate_t tau_d_AMPA    = 5.0;
    Rate_t tau_r_GABA    = 1.0;
    Rate_t tau_d_GABA    = 5.0;
    
    Flag_t track_sample_neurons = true;
    Flag_t want_time_sim = true;       // Do you want to time the simulation?
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////// Initialization //////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    // Total number of steps (#) in the loop
    Total_t nr_time_steps= t_end/dt;
    // INSERT ASSERT FOR NR OF TIMESTEPS. IT MUST BE AN INTEGER!!!
    
    //Convert rate from #/s to #/ms
    Rate_t rate = external_spike_rate/t_unit;
    
    // Generate the network adjacency matrix and related variables
    //   boost::multi_array<int, 2> A(boost::extents[n][n]);
    Number_t A[n*n-1];
    Scale_t eneff = 0.0;
    Scale_t ineff = 0.0;
    generate_LIF_network_adjacency_matrix(network_type, en, in, params, A,
                                          eneff, ineff, RNG);
    
    // Neuron types in the network: first en neurons are excitatory (1) and the
    // last in neurons are inhibitory (-1)
    Number_t eori[n-1];
    for (int i = 0; i < n; i++)
    {
        if (i<en) { eori[i] = 1; }
        else      { eori[i] = -1;}
    }
    
    // Last external Poisson spike time into each neuron
    Time_t lpst[n-1];
    for (int i = 0; i < n; i++)
    { lpst[i] = RNG.next_spike_waiting_time(rate, external_spike_type_num); }
    
    Conductance_t gep0[n-1];   // Poisson conductance at t
    Conductance_t gen0[n-1];   // Excitatory network conductance at t
    Conductance_t gin0[n-1];   // Inhibitory network conductance at t
    Conductance_t get0[n-1];   // Total excitatory conductance at t
    Conductance_t git0[n-1];   // Total inhibitory conductance at t
    Potential_t     v0[n-1];   // Membrane potential at t
    for (int i=0; i<n; i++)
    { gep0[i] = 0; gen0[i]=0; gin0[i]=0; get0[i]=0; git0[i]=0; v0[i]=0; }
    
    Conductance_t gep1[n-1];   // Poisson conductance at t+dt
    Conductance_t gen1[n-1];   // Excitatory network conductance at t+dt
    Conductance_t gin1[n-1];   // Inhibitory network conductance at t+dt
    Conductance_t get1[n-1];   // Total excitatory conductance at t+dt
    Conductance_t git1[n-1];   // Total inhibitory conductance at t+dt
    Potential_t     v1[n-1];   // Membrane potential at t+dt
    for (int i=0; i<n; i++)
    { gep1[i] = 0; gen1[i]=0; gin1[i]=0; get1[i]=0; git1[i]=0; v1[i]=0; }
    
    Conductance_t  hep[n-1];   // Total Poisson h-jump
    Conductance_t  hen[n-1];   // Total excitatory network h-jump
    Conductance_t  hin[n-1];   // Total inhibitory network h-jump
    Conductance_t  genJump[n-1];
    Conductance_t  henJump[n-1];
    Conductance_t  ginJump[n-1];
    Conductance_t  hinJump[n-1];
    for (int i=0; i<n; i++)
    { hep[i] = 0; hen[i]=0; hin[i]=0; genJump[i]=0; henJump[i]=0;
        ginJump[i]=0; hinJump[i]=0; }
    
    std::vector<Number_t> rast_ID;            // Firing neuron ID
    std::vector<Time_t>   rast_firing_time;   // Firing time
    
    Number_t    nr_rps[n-1];   // # of received Poisson spikes
    Time_t         lft[n-1];   // Last firing time (i.e. v>vThresh)
    Number_t        rs[n-1];   // 0: Not in refractory state
    //-1: In refractory period but will exit
    //    in the next time step
    // 1: In refractory period and will exit
    //    after the next time step
    for (int i=0; i<n; i++)
    { nr_rps[i] = 0; lft[i]=0; rs[i]=0; }
    
    ///////////////////////////////////////////////////////////////////////
    ////////////////////////////// TRACKING ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    // Select index to track
    Number_t idx_set_track[nr_pops*sn-1];
    Number_t single_track_id = 0;;
    
    if ( track_sample_neurons )
    {
        // TAKE CARE OF THESE RANDOM INDICES LATER
        idx_set_track[0] = 1 - 1;
        idx_set_track[1] = en+1 - 1;
        single_track_id =  0;
    }
    ///////////////////////////////////////////////////////////////////////
    ////////////////////////////// THE LOOP ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    Time_t t_current;                // Current simulation time
    Time_t t_next;                   // Next simulation time
    double current_progress = 0;     // Progress of the loop
    double progress_unit    = 0.01;  // Unit of progress to be tracked
    // Initialize simulation timing
    auto     tSIM = std::chrono::steady_clock::now();
    // Initialize display timing
    auto tDisplay = std::chrono::steady_clock::now();
    // Time spent since last progress update
    std::chrono::duration<double> elapsed_time_since_last_display = tSIM-tDisplay;
    // Total time spent
    std::chrono::duration<double> total_elapsed_time = tSIM-tDisplay;
    
    for(int i = 0; i < nr_time_steps; i++)
    {
        // Update variables at t_current
        t_current = i*dt;
        for (int j = 0; j < n; j++)
        {
            gen0[j] = gen1[j];
            gin0[j] = gin1[j];
            // Evolve network condtuctances and h to next time step
            analytic(dt, tau_d_AMPA, tau_r_AMPA, gen1[j], hen[j]);
            analytic(dt, tau_d_GABA, tau_r_GABA, gin1[j], hin[j]);
            gep0[j] = gep1[j];
            v0[j]   = v1[j];
        }
        t_next    = (i+1)*dt;
        
        // Evolve Poisson conductance input from t_current to t_next
        evolve_ge_poisson_input(t_current, dt, gep1, hep, nr_rps, lpst,
                                tau_r_AMPA, tau_d_AMPA, fS, fR, rate, single_track_id,
                                n, t_next, RNG, external_spike_type_num);
        
        // Update total conductances
        for (int j = 0; j < n; j++)
        {
            get0[j] = gen0[j] + gep0[j];
            get1[j] = gen1[j] + gep1[j];
            git0[j] = gin0[j];
            git1[j] = gin1[j];
        }
        
        // Use total conductances to solve LIF and evolve membrane potential to
        // the next time step t+dt. Also, determine neurons that have reached
        // the threshold and evaluate how much network conductances need to be
        // jumped.
        evolve_potential(t_current, dt, get0, git0, get1, git1, v0, en, in, A, eneff,
                         ineff, eori, lft, single_track_id, extI, vE, vI, vL, gL, vThresh,
                         SE, SI, tau_r_AMPA, tau_d_AMPA, tau_r_GABA, tau_d_GABA, tref,
                         rs, v1, rast_ID, rast_firing_time,
                         genJump, henJump, ginJump, hinJump, t_next);
        
        // Evolve network conductances
        for (int j = 0; j < n; j++)
        {
            gen1[j] = gen1[j] + genJump[j];
            hen[j]  = hen[j]  + henJump[j];
            gin1[j] = gin1[j] + ginJump[j];
            hin[j]  = hin[j]  + hinJump[j];
        }
        
        // Display simulation progress
        if ( want_time_sim )
        {
            elapsed_time_since_last_display = std::chrono::steady_clock::now()-tDisplay;
            if ( elapsed_time_since_last_display.count() > 6.0  )
            {
                // Reset display clock
                tDisplay = std::chrono::steady_clock::now();
                // Update total elapsed time
                total_elapsed_time = std::chrono::steady_clock::now()-tSIM;
                // Display progress
                std::cout << std::fixed << std::setprecision(2);
                std::cout << 100.0*double(i)/double(nr_time_steps) << " %";
                std::cout << " is completed in ";
                std::cout << std::fixed << std::setprecision(1);
                std::cout << total_elapsed_time.count()/60;
                std::cout << std::fixed << std::setprecision(4);
                std::cout << " minutes " << std::endl;
                // Update current_progress
                current_progress = current_progress+progress_unit;
            }
        }
    }
    
    total_elapsed_time = std::chrono::steady_clock::now()-tSIM;
    std::cout << " Simulation is completed in ";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << total_elapsed_time.count()/60;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << " minutes. " << std::endl;
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////// CALCULATE POPULATION STATISTICS /////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    Conductance_t  exc_poisson_input, inh_poisson_input, tot_poisson_input;
    Rate_t         exc_firing_rate,   inh_firing_rate,   tot_firing_rate;
    Potential_t    avg_exc_v,         avg_inh_v,         avg_v;
    Conductance_t  avg_exc_gep,       avg_inh_gep,       avg_gep;
    Conductance_t  avg_exc_gen,       avg_inh_gen,       avg_gen;
    Conductance_t  avg_exc_gin,       avg_inh_gin,       avg_gin;
    
    calculate_population_statistics(n, en, in, nr_rps, t_unit, t_end,
                                    rast_ID, rast_firing_time, warm_up_time,
                                    network_type, params, tref, SE, SI, dt, extI, fS,
                                    external_spike_type_num, external_spike_rate, eneff, ineff, tref,
                                    v1, gep1, gen1, gin1,
                                    exc_poisson_input, inh_poisson_input, tot_poisson_input,
                                    exc_firing_rate,   inh_firing_rate,   tot_firing_rate,
                                    avg_exc_v,         avg_inh_v,         avg_v,
                                    avg_exc_gep,       avg_inh_gep,       avg_gep,
                                    avg_exc_gen,       avg_inh_gen,       avg_gen,
                                    avg_exc_gin,       avg_inh_gin,       avg_gin);
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////// WRITE SIMULATION OUTPUT /////////////////////////
    ///////////////////////////////////////////////////////////////////////

    write_simulation_output(n, en, in, nr_rps, t_unit, t_end,
                            rast_ID, rast_firing_time, warm_up_time,
                            network_type, params, tref, SE, SI, dt, extI, fS,
                            external_spike_type_num, external_spike_rate, eneff, ineff, tref,
                            v1, gep1, gen1, gin1,
                            exc_poisson_input, inh_poisson_input, tot_poisson_input,
                            exc_firing_rate,   inh_firing_rate,   tot_firing_rate,
                            avg_exc_v,         avg_inh_v,         avg_v,
                            avg_exc_gep,       avg_inh_gep,       avg_gep,
                            avg_exc_gen,       avg_inh_gen,       avg_gen,
                            avg_exc_gin,       avg_inh_gin,       avg_gin);
    
    
    ///////////////////////////////////////////////////////////////////////
    ////////////////////////////// END ////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    return 0;
}
