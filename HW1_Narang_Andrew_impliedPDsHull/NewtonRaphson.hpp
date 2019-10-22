//Andrew Narang
//Credit Risk Management - Assignment #1

//Declaration of more flexible Newton-Rapshon function.
//Can work with any function object with an overloaded () operator, so our range of functions we can apply the Newton-Raphson function to is extended significantly.

//Can update this to work with numeric template classes rather than only double as well.


#ifndef NewtonRaphson_HPP
#define NewtonRaphson_HPP


template<typename f, typename f_prime>
double newton_raphson(const f& in_f, const f_prime& in_f_prime, const double& x0, const double& prec); 



#ifndef NewtonRaphson_CPP
#include "NewtonRaphson.cpp"
#endif



#endif
