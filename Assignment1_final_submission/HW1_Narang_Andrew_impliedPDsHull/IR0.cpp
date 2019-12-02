//Andrew Narang
//Credit Risk Management - Assignment #1

//Implementations of interest-rate conversion functions

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

#include "IR0.hpp"

double R_m2c(const double& in_Rm, const double& in_m)
{
  return in_m * log(1.0 + in_Rm / in_m); 
}


double R_c2m(const double& in_Rc, const double& in_m)
{
  return in_m * (exp(in_Rc/in_m) - 1);
}

//rate compounded in_m times per year for in_n years                                                                                                        
double R(const double& in_FV, const double& in_PV, const double& in_n, const double& in_m)
{
  return (pow((in_FV/in_PV), (1.0/(in_m * in_n))) - 1.0) * in_m;
}

//cont. compounded rate for in_n years
double R(const double& in_FV, const double& in_PV, const double& in_n)
{
  return log(in_FV/in_PV)/in_n; 
}


//calculation of forward rate per annum between two maturities given the zero rates corresponding with each maturity
double FR(const double& in_Z1, const double& in_T1, const double& in_Z2, const double& in_T2)
{
  return ((in_Z2 * in_T2) - (in_Z1 * in_T1))/(in_T2 - in_T1); 
}


//fills a curve of forward rates given a curve of zero rates and assoc. maturities (all per annum and based on cont. compounding)
void FR_curve(const std::vector<double>& in_zeros, const std::vector<double>& in_mats, std::vector<double>& in_forwards)
{
  //the 0th element of the forward rate vector should be = the 0th element of the zeros vector, since the forward rate from time 0 to time 1 = the zero rate from time 0 to time 1
  //in_forwards[0] = in_zeros[0];

  //perhaps the 0th element of the forward rate vector should just be = 0, since it does not actually exist...
  in_forwards[0] = 0.0; 
  
  
  for (int i = 1; i < in_zeros.size(); ++i)
    {
      in_forwards[i] = FR(in_zeros[i - 1], in_mats[i - 1], in_zeros[i], in_mats[i]);
    }
}


//calculates par yield of a given bond-maturity. set to semi-annual compounding by default. assumes that there are payments at all given maturities. To work with diff. payment schemes, create an alternate version of the vector and input that instead - then adjust the compounding frequency.                                       
double ParYield(const std::vector<double>& in_mats, const std::vector<double>& in_zeros, const int& target_mat, const int& coupon_freq)
{
  double coeff_sum = 0;

  for (int i = 0; i < target_mat + 1; ++i)
    {
      coeff_sum += (exp(-1.0 * in_mats[i] * in_zeros[i])/coupon_freq);
    }

  double princ_pmt = exp(-1.0 * in_zeros[target_mat] * in_mats[target_mat]);

  return (1 - princ_pmt)/coeff_sum; 

}


//calculates the next zero rate using a series of zero rates and a bond with payment at the next maturity.
//given a vector of maturities, zeros, the position of the next zero rate to be filled, and the bond info that will be used to fill the next zero rate                                                                                                                               
//in_cpn = coupon payment per period, i.e. 0.03 for 6% semiannual coupon                                                                      
void next_zero(const std::vector<double>& in_mats, std::vector<double>&	in_zeros, const	int& target_mat, const double& in_px0, const double& in_cpn)
{
  double cfs0 = 0;

  for (int i = 0; i < target_mat; ++i)
    {
      cfs0 += in_cpn * exp(-1.0 * in_zeros[i] * in_mats[i]);
      //std::cout << cfs0 << std::endl;
    }

  //std::cout << cfs0 << std::endl;

  in_zeros[target_mat] = log((in_px0 - cfs0)/(in_cpn + 1))/(-1.0 * in_mats[target_mat]); 
}


//calculates cheapest to deliver bond given vector of quoted prices, vector of assoc. conversion factors, and most recent settlement price
double c2d(const std::vector<double>& pxs, const std::vector<double>& conv_fs, const double& set_px, int& pos)
{
  //vector of delivery costs
  std::vector<double> dcs(pxs);

  for (int i = 0; i < dcs.size(); ++i)
    {
      dcs[i] = dcs[i] - (set_px * conv_fs[i]);
    }

  auto target = std::min_element(dcs.begin(), dcs.end());

  pos = target - dcs.begin();

  return (*target); 
}


//calculates the hazard rate given the unconditional default probability between [t, t + dt], the cumulative probability of default between [0, t], and dt (= 1 by default, representing 1 year)
double hzd(const double& in_ucpd, const double& in_cupd, const double& dt)
{
  double x =  in_ucpd/((1.0 - in_cupd) * dt);

  return -log(1 - x)/dt; 
}
