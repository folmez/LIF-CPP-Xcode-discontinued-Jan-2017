#ifndef SMALL_FUNCTIONS_H
#define SMALL_FUNCTIONS_H

#include "typedefs.h"
#include "My_Random_Number_Generator.h"

void read_params(Number_t &en, Number_t &in, std::string &network_type,
                 Time_t &t_end, Time_t &dt, Probability_t &params,
                 Number_t &external_spike_type_num, Rate_t &external_spike_rate,
                 Time_t &tref, Time_t &warm_up_time,
                 Number_t &nr_pops, Number_t &sn, Current_t &extI,
                 Rate_t &fS, Rate_t &fR, Rate_t &SE, Rate_t &SI);

Conductance_t get_alpha(Conductance_t ge, Conductance_t gi, Conductance_t gL);

Conductance_t get_beta(Conductance_t ge, Conductance_t gi, Conductance_t gL,
                       Potential_t vE, Potential_t vI, Potential_t vL, Current_t extI);

double evaluate_k(Potential_t v, double a, double b);

Potential_t rk2(Time_t dt, Potential_t v0, double a0, double a1,
                double b0, double b1, Potential_t vI);

Potential_t shelley_and_tao_2001(Time_t dt, Time_t firing_time,
                                 double a0, double a1, double b0, double b1,
                                 Potential_t vI, Potential_t vL);

void analytic(Time_t dt, Rate_t tau_d, Rate_t tau_r,
              Conductance_t &g, Conductance_t &h);

void evolve_ge_poisson_input(Time_t t_current, Time_t dt,
                             Conductance_t gep1[], Conductance_t hep[], Number_t nr_rps[],
                             Time_t lpst[], Rate_t tau_r_AMPA, Rate_t tau_d_AMPA, Rate_t fS,
                             Rate_t fR, Rate_t rate, Number_t single_track_id, Number_t n,
                             Time_t t_next, My_Random_Number_Generator_t RNG,
                             Number_t external_spike_type_num);

void generate_LIF_network_adjacency_matrix(std::string network_type,
                                           Number_t en, Number_t in, Probability_t p,
                                           Number_t A[], Scale_t &eneff, Scale_t &ineff,
                                           My_Random_Number_Generator_t RNG);

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
                      Conductance_t ginJump[], Conductance_t hinJump[], Time_t t_next);

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
                                     Conductance_t &avg_inh_gin, Conductance_t &avg_gin);

#endif
