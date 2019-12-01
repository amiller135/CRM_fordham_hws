//Andrew Narang
//Credit Risk Management - Assignment #1

//Implementation of more flexible Newton-Raphson function.


#ifndef NewtonRaphson_CPP
#define NewtonRaphson_CPP


#include <iostream>
#include <cmath>

#include "NewtonRaphson.hpp"


template<typename f, typename f_prime>
double newton_raphson(const f& in_f, const f_prime& in_f_prime, const double& x0, const double& prec)
{
  double x_0 = x0;

  double x_1 = (x_0 - (in_f(x0)/in_f_prime(x0)));

  double difference = 0.0;

  difference = std::abs(x_1 - x_0);

  while (difference > prec)
    {
      x_0 = x_1;

      x_1 = (x_0 - (in_f(x_0)/in_f_prime(x_0)));

      difference = std::abs(x_1 - x_0);

      //std::cout << "looped: " << difference << std::endl;
    }

  return x_1;
  
}









#endif
