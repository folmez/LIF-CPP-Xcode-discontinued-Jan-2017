#include "typedefs.h"
#include "My_Random_Number_Generator.h"

void write_simulation_output(Number_t n, Number_t en, Number_t in,
                             Number_t nr_rps[], Time_t t_unit, Time_t t_end,
                             std::vector<Number_t> rast_ID, std::vector<Time_t> rast_firing_time,
                             Time_t warm_up_time, std::string network_type, Probability_t params,
                             Time_t ref, Rate_t SE, Rate_t SI, Time_t dt, Current_t extI, Rate_t fS,
                             Number_t external_spike_type_num, Rate_t external_spike_rate,
                             Scale_t eneff, Scale_t ineff, Time_t tref,
                             Potential_t v1[], Conductance_t gep1[], Conductance_t gen1[],
                             Conductance_t gin1[],
                             Conductance_t &exc_poisson_input, Conductance_t inh_poisson_input,
                             Conductance_t tot_poisson_input, Rate_t exc_firing_rate,
                             Rate_t inh_firing_rate, Rate_t tot_firing_rate, Potential_t avg_exc_v,
                             Potential_t avg_inh_v, Potential_t avg_v, Conductance_t avg_exc_gep,
                             Conductance_t avg_inh_gep, Conductance_t avg_gep,
                             Conductance_t avg_exc_gen, Conductance_t avg_inh_gen,
                             Conductance_t avg_gen, Conductance_t avg_exc_gin,
                             Conductance_t avg_inh_gin, Conductance_t avg_gin)
{
    // Obtain date and time as string: Nov24_1406
    time_t rawtime;
    struct tm * timeinfo;
    char datestamp [20];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(datestamp, 11, "%b%d_%H%M", timeinfo);
    
    ////////////////////////////////////////////////////////////
    ////////////////  SIMULATION INPUT  ////////////////////////
    ////////////////////////////////////////////////////////////
    
    // Initialize simulation input filename
    std::string sim_input_filename = "../sim_input_";
    sim_input_filename = sim_input_filename + datestamp;
    sim_input_filename = sim_input_filename + ".txt";
    
    // Initialize simulation input file
    std::ofstream sim_input_file;
    
    // Open simulation input file
    sim_input_file.open (sim_input_filename, std::ofstream::out);
    
    sim_input_file << network_type  << "\tNetwork type"                 << std::endl;
    sim_input_file << params        << "\tNetwork param"                << std::endl;
    sim_input_file << en            << "\tNumber of excitatory neurons" << std::endl;
    sim_input_file << in            << "\tNumber of inhibitory neurons" << std::endl;
    sim_input_file << eneff         << "\tExcitatory Neff coefficient"  << std::endl;
    sim_input_file << ineff         << "\tInhibitory Neff coefficient"  << std::endl;
    sim_input_file << tref          << "\tRefractory period"            << std::endl;
    sim_input_file << SE            << "\tExcitatory cortical strength" << std::endl;
    sim_input_file << SI            << "\tInhibitory cortical strength" << std::endl;
    sim_input_file << dt            << "\tSimulation stepsize"          << std::endl;
    sim_input_file << t_end         << "\tTotal simulation time"        << std::endl;
    sim_input_file << warm_up_time  << "\tWarm up time"                 << std::endl;
    sim_input_file << extI          << "\tExternal input current"       << std::endl;
    
    sim_input_file << external_spike_type_num   << "\tExternal spike type (1 for Poisson, 2 for Deterministic)" << std::endl;
    sim_input_file << external_spike_rate       << "\tExternal spike rate"                                      << std::endl;
    sim_input_file << fS                        << "\tExternal spike strength"                                  << std::endl;
    
    // Close simulation input file
    sim_input_file.close();
    
    ////////////////////////////////////////////////////////////
    ////////////////  SIMULATION OUTPUT  ///////////////////////
    ////////////////////////////////////////////////////////////
    
    // Initialize simulation output filename
    std::string sim_output_filename = "../sim_output_";
    sim_output_filename = sim_output_filename + datestamp;
    sim_output_filename = sim_output_filename + ".txt";
    
    // Initialize simulation output file
    std::ofstream sim_output_file;
    
    // Open simulation output file
    sim_output_file.open (sim_output_filename, std::ofstream::out);
    
    // Set the output precision
    sim_output_file << std::fixed << std::setprecision(4);
    
    // Write
    sim_output_file << exc_firing_rate << "\tExcitatory firing rate" << std::endl;
    sim_output_file << inh_firing_rate << "\tInhibitory firing rate" << std::endl;
    sim_output_file << tot_firing_rate << "\tTotal firing rate" << std::endl;

    // Close simulation output file
    sim_output_file.close();
    
    ////////////////////////////////////////////////////////////
    //////////////////////  RASTER  ////////////////////////////
    ////////////////////////////////////////////////////////////
    
    // Initialize raster filename
    std::string raster_filename = "../raster_";
    raster_filename = raster_filename + datestamp;
    raster_filename = raster_filename + ".txt";
    
    // Initialize raster file
    std::ofstream raster_file;

    // Open raster file
    raster_file.open (raster_filename, std::ofstream::out);
    
    // Set the output precision
    raster_file << std::fixed << std::setprecision(4);
    
    // Write
    raster_file << "Neuron ID";
    raster_file << "\t" << "Firing Time";
    raster_file << std::endl;
    for (int i = 0; i < rast_ID.size(); i++)
    {
        raster_file << "\t" << rast_ID[i];
        raster_file << "\t" << rast_firing_time[i];
        raster_file << std::endl;
    }
    
    // Close raster file
    raster_file.close();
    
}
