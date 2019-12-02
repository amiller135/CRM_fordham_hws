//Andrew Narang
//Credit Risk Management - Assignment #1

//Header file for functions for converting interest rates.


#ifndef IR0_HPP
#define IR0_HPP

#include <vector>

//these functions calculate equivalent interest rates quoted on per annum basis
//in_m = # of times per year the rate is compounded, i.e. semiannual compounding should be entered as 2 rather than 0.5

//in_m compoundings per year to cont. compounding
double R_m2c(const double& in_Rm, const double& in_m);

//cont. compounding to in_m compoundings per year
double R_c2m(const double& in_Rc, const double& in_m);


//these functions determine interest rates per annum based on future values, present values, and years
//in_n = # of years

//rate compounded m times per year
double R(const double& in_FV, const double& in_PV, const double& in_n, const double& in_m);

//cont. compounded rate
double R(const double& in_FV, const double& in_PV, const double& in_n);

//calculation of forward rate per annum between two maturities given the zero rates corresponding with each maturity
//maturity should be entered in years
//based on cont. compounding 
double FR(const double& in_Z1, const double& in_T1, const double& in_Z2, const double& in_T2); 

//fills a curve of forward rates given a curve of zero rates and assoc. maturities (all per annum and based on cont. compounding)
void FR_curve(const std::vector<double>& in_zeros, const std::vector<double>& in_mats, std::vector<double>& in_forwards); 

//calculates par yield of a given bond-maturity. set to semi-annual compounding by default. assumes that there are payments at all given maturities. To work with diff. payment schemes, create an alternate version of the vector and input that instead - then adjust the compounding frequency.
double ParYield(const std::vector<double>& in_mats, const std::vector<double>& in_zeros, const int& target_mat, const int& coupon_freq = 2); 

//calculates the next zero rate using a series of zero rates and a bond with payment at the next maturity.
//given a vector of maturities, zeros, the position of the next zero rate to be filled, and the bond info that will be used to fill the next zero rate
//in_cpn = coupon payment per period, i.e. 0.03 for 6% semiannual coupon
void next_zero(const std::vector<double>& in_mats, std::vector<double>& in_zeros, const int& target_mat, const double& in_px0, const double& in_cpn); 

//calculates cheapest to deliver bond given vector of quoted prices, vector of assoc. conversion factors, and most recent settlement price
double c2d(const std::vector<double>& pxs, const std::vector<double>& conv_fs, const double& set_px, int& pos); 

//calculates the hazard rate given the unconditional default probability between [t, t + dt], the cumulative probability of default between [0, t], and dt (= 1 by default, representing 1 year)
double hzd(const double& in_ucpd, const double& in_cupd, const double& dt = 1);

#endif
