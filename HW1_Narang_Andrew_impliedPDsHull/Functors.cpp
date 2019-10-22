//Andrew Narang
//Credit Risk Management - Assignment #1


#include <iostream>
#include <vector>
#include <cmath>

#include "Functors.hpp"

//f::f(const std::vector<double>& in_mats, const double& in_px0, const double& in_coupon, const double& in_par) : mats(in_mats), px0(in_px0), coupon(in_coupon), par(in_par)
f::f(const std::vector<double>& in_mats, const double& in_px0, const double& in_coupon) : mats(in_mats), px0(in_px0), coupon(in_coupon)
{
  //std::cout << "f object constructed." << std::endl;
}

f::~f()
{
  //std::cout << "f object deconstructed." << std::endl;
}


double f::operator () (const double& in_x) const
{
  double result = 0;

  for (int i = 0; i < mats.size(); ++i)
    {
      result += (coupon * exp(-1.0 * in_x * mats[i]));
    }

  result += (exp(-1.0 * in_x * mats[mats.size() - 1]));

  result -= px0;

  return result;
}






//f_prime::f_prime(const std::vector<double>& in_mats, const double& in_px0, const double& in_coupon, const double& in_par) : mats(in_mats), px0(in_px0), coupon(in_coupon), par(in_par)
//{
//  std::cout << "f_prime object constructed." << std::endl;
//}

//f_prime::f_prime(const std::vector<double>& in_mats, const double& in_coupon, const double& in_par) : mats(in_mats), coupon(in_coupon), par(in_par)
//{
  //std::cout << "f_prime object constructed." << std::endl;
//}

f_prime::f_prime(const std::vector<double>& in_mats, const double& in_coupon) : mats(in_mats), coupon(in_coupon)
{
  //std::cout << "f_prime object constructed." << std::endl;                                                                                
}

f_prime::~f_prime()
{
  //std::cout << "f_prime object deconstructed." << std::endl;
}


double f_prime::operator () (const double& in_x) const
{
  double result = 0;

  for (int i = 0; i < mats.size(); ++i)
    {
      result += (coupon * exp(-1.0 * in_x * mats[i]) * (-1.0 * mats[i]));
    }

  result += (exp(-1.0 * in_x * mats[mats.size() - 1]) * (-1.0 * mats[mats.size() - 1])); 

  return result;
}
