//Andrew Narang
//Credit Risk Management - Assignment #1

#ifndef Functors_HPP
#define Functors_HPP

#include <vector>

//make these work without par => per $1 notional value

class f
{
private:
  const std::vector<double>& mats;

  const double& px0;

  const double& coupon;

  //const double& par;

public:
  f(const std::vector<double>& in_mats, const double& in_px0 = 0.0, const double& in_coupon = 0.0);             
  //f(const std::vector<double>& in_mats, const double& in_px0 = 0.0, const double& in_coupon = 0.0, const double& in_par = 0.0);

  ~f();

  double operator () (const double& in_x) const; 
};


class f_prime
{
private:
  const std::vector<double>& mats;

  //const double& px0;

  const double& coupon;

  //const double& par;

public:

  f_prime(const std::vector<double>& in_mats, const double& in_coupon = 0.0);
  //f_prime(const std::vector<double>& in_mats, const double& in_coupon = 0.0, const double& in_par = 0.0);
  //f_prime(const std::vector<double>& in_mats, const double& in_px0 = 0.0, const double& in_coupon = 0.0, const double& in_par = 0.0);

  ~f_prime();

  double operator () (const double& in_x) const;
};


#endif
