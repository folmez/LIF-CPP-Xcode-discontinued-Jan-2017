#include "My_Random_Number_Generator.h"
#include "typedefs.h"
#include "small_functions.h"

void read_params(Number_t &en, Number_t &in, std::string &network_type,
                 Time_t &t_end, Time_t &dt, Probability_t &params,
                 Number_t &external_spike_type_num, Rate_t &external_spike_rate,
                 Time_t &tref, Time_t &warm_up_time,
                 Number_t &nr_pops, Number_t &sn, Current_t &extI,
                 Rate_t &fS, Rate_t &fR, Rate_t &SE, Rate_t &SI)
{
    // Open the config file
    std::ifstream input_file("../LIF_network_config.txt");
    // String to hold lines in the config file
    std::string line;
    // Position of = sign to determine variable names and values
    std::size_t pos_of_equal_sign;
    // String to hold variable names in the config file (before = sign)
    std::string var_name;
    // String to hold variable values in the config file (after = sign)
    std::string var_value_str;
    
    std::cout << std::fixed << std::setprecision(4);
    
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << " READING PARAMTERS FROM LIF_network_config.txt" << std::endl;
    std::cout << std::endl;
    
    int line_number = 0;
    while ( std::getline(input_file, line) )
    {
        line_number = line_number+1;
        std::cout << "Line " << line_number << ": ";
        
        pos_of_equal_sign = line.find_first_of("=");
        if ( line[0] == '#' )
        {
            std::cout << line << std::endl;
        }
        else if ( pos_of_equal_sign == std::string::npos )
        {
            std::cout << std::endl;
        }
        else
        {
            var_name = line.substr(0,pos_of_equal_sign);
            var_value_str = line.substr(pos_of_equal_sign+1,
                                        line.length()-2);
            if ( var_name == "en" )
            {
                
                std::cout << "Variable 'en'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                en = std::stoi(var_value_str);
                std::cout << "\t\tAssigned: en=" << en << std::endl;
            }
            else if ( var_name == "in" )
            {
                std::cout << "Variable 'in'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                in = std::stoi(var_value_str);
                std::cout << "\t\tAssigned: in=" << in << std::endl;
            }
            else if ( var_name == "network_type" )
            {
                std::cout << "Variable 'network_type'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                network_type = var_value_str;
                std::cout << "\t\tAssigned: network_type=" << network_type << std::endl;
            }
            else if ( var_name == "t_end" )
            {
                std::cout << "Variable 't_end'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                t_end = std::stod(var_value_str);
                std::cout << "\t\tAssigned: t_end=" << t_end << std::endl;
            }
            else if ( var_name == "dt" )
            {
                std::cout << "Variable 'dt'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                dt = std::stod(var_value_str);
                std::cout << "\t\tAssigned: dt=" << dt << std::endl;
            }
            else if ( var_name == "params" )
            {
                std::cout << "Variable 'params'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                params = std::stod(var_value_str);
                std::cout << "\t\tAssigned: params=" << params << std::endl;
            }
            else if ( var_name == "external_spike_type_num" )
            {
                std::cout << "Variable 'external_spike_type_num'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                external_spike_type_num = std::stoi(var_value_str);
                std::cout << "\t\tAssigned: external_spike_type_num=" << external_spike_type_num << std::endl;
            }
            else if ( var_name == "external_spike_rate" )
            {
                std::cout << "Variable 'external_spike_rate'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                external_spike_rate = std::stod(var_value_str);
                std::cout << "\t\tAssigned: external_spike_rate=" << external_spike_rate << std::endl;
            }
            else if ( var_name == "tref" )
            {
                std::cout << "Variable 'tref'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                tref = std::stod(var_value_str);
                std::cout << "\t\tAssigned: tref=" << tref << std::endl;
            }
            else if ( var_name == "warm_up_time" )
            {
                std::cout << "Variable 'warm_up_time'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                warm_up_time = std::stod(var_value_str);
                std::cout << "\t\tAssigned: warm_up_time=" << warm_up_time << std::endl;
            }
            else if ( var_name == "nr_pops" )
            {
                std::cout << "Variable 'nr_pops'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                nr_pops = std::stoi(var_value_str);
                std::cout << "\t\tAssigned: nr_pops=" << nr_pops << std::endl;
            }
            else if ( var_name == "sn" )
            {
                std::cout << "Variable 'sn'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                sn = std::stoi(var_value_str);
                std::cout << "\t\tAssigned: sn=" << sn << std::endl;
            }
            else if ( var_name == "extI" )
            {
                std::cout << "Variable 'extI'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                extI = std::stod(var_value_str);
                std::cout << "\t\tAssigned: extI=" << extI << std::endl;
            }
            else if ( var_name == "fS" )
            {
                std::cout << "Variable 'fS'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                fS = std::stod(var_value_str);
                std::cout << "\t\tAssigned: fS=" << fS << std::endl;
            }
            else if ( var_name == "fR" )
            {
                std::cout << "Variable 'fR'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                fR = std::stod(var_value_str);
                std::cout << "\t\tAssigned: fR=" << fR << std::endl;
            }
            else if ( var_name == "SE" )
            {
                std::cout << "Variable 'SE'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                SE = std::stod(var_value_str);
                std::cout << "\t\tAssigned: SE=" << SE << std::endl;
            }
            else if ( var_name == "SI" )
            {
                std::cout << "Variable 'SI'\tis found. Its value is ";
                std::cout << var_value_str << std::endl;
                SI = std::stod(var_value_str);
                std::cout << "\t\tAssigned: SI=" << SI << std::endl;
            }
            else
            {
                std::cout << "WHAT IS THIS: " << line << std::endl;
            }
        }
    }
    
    std::cout << std::endl;
    std::cout << " PARAMETER READING COMPLETED." << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << std::endl;
    
}


