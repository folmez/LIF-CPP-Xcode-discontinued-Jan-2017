#ifndef MY_RANDOM_NUMBER_GENERATOR_H
#define MY_RANDOM_NUMBER_GENERATOR_H

#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <random>
#include <stdio.h>
#include <string>
#include <time.h>
#include <cassert>
#include "typedefs.h"
#include <chrono>
#include <sstream>
#include <time.h>

struct My_Random_Number_Generator_t
{
   static double next_spike_waiting_time(double rate,
      Number_t external_spike_type_num)
   {
      static std::mt19937 mersenne_generator(std::random_device{}());
      static std::uniform_real_distribution<> distribution(0, 1);
      if ( external_spike_type_num == 1 )
      {
         return (-1)*log(distribution(mersenne_generator))/rate;
      }
      else if ( external_spike_type_num == 2 )
      {
         return 1/rate;
      }
      else
      {
         return -1.0;
      }
   }

   static double uniform01()
   {
      static std::mt19937 mersenne_generator01(std::random_device{}());
      static std::uniform_real_distribution<> distribution01(0, 1);
      return distribution01(mersenne_generator01);
   }

};

#endif
