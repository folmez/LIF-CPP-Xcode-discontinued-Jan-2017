#include "My_Random_Number_Generator.h"
#include "typedefs.h"
#include "small_functions.h"

void generate_LIF_network_adjacency_matrix(std::string network_type,
   Number_t en, Number_t in, Probability_t p,
   Number_t A[], Scale_t &eneff, Scale_t &ineff,
   My_Random_Number_Generator_t RNG)
{
   Number_t n = en + (-1)*in; //= en + (-1)*in;
   Probability_t u;
   Number_t degrees[n-1];
   for (int i=0; i<n; i++)
   { degrees[i] = 0; }
   for (int i=0; i<n*n; i++)
   {
      A[i]=0;
   }
   
   Flag_t display_matrix = false;

   if ( network_type == "ER" )
   {
      // Form edges above the diagonal
      for (int i = 0; i < n; i++)
      {
         for (int j = i+1; j < n; j++)
         {
            // Generate a uniform real number u in (0,1)
            u = RNG.uniform01();
            // Form edges
            if ( u < p )
            { A[i*n+j] = 1; }  // Form an edge if u is less than p
            else
            { A[i*n+j] = 0; }  // Don't form an edge if u is not less than p
         }
      }
      // Form edges below the diagonal
      for (int i = 0; i < n; i++)
      {
         for (int j = i-1; j >= 0; j--)
         {
            A[i*n+j] = A[j*n+i];
         }
      }
   }

   // Calculate degrees
   Number_t int_count;
   for (int i = 0; i < n; i++)
   {
      int_count = 0;
      for (int j = 0; j <n ; j++)
      {
         int_count = int_count + A[i*n+j];
      }
      degrees[i] = int_count;
   }

   // Calculate eneff (average number of neighbors of an excitatory neuron)
   double double_count;
   double_count = 0;
   for (int i = 0; i < en; i++)
   {
      double_count = double_count + degrees[i];
   }
   eneff = double_count/n;
   // Calculate ineff (average number of neighbors of an excitatory neuron)
   double_count = 0;
   for (int i = en; i < n; i++)
   {
      double_count = double_count + degrees[i];
   }
   ineff = double_count/n;

   // Display matrix
   if ( display_matrix)
   {
      for (int i = 0; i < n; i++)
      {
         std::cout << degrees[i] << ": ";
         for (int j = 0; j < n; j++)
         {
            std::cout << " " << A[i*n+j];
         }
         std::cout << std::endl;
      }
      std::cout << "eneff (average number of neighbors ";
      std::cout << "of an excitatory neuron): ";
      std::cout << eneff << std::endl;
      std::cout << "ineff (average number of neighbors ";
      std::cout << "of an ixcitatory neuron): ";
      std::cout << ineff << std::endl;
   }

}
