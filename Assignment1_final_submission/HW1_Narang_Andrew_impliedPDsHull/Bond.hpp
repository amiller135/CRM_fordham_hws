//Andrew Narang
//Credit Risk Management - Assignment #1

//Declaration of Bond class - should be expanded over time

#ifndef Bond_HPP
#define Bond_HPP

#include <vector>

//perhaps T_Price should be named Price as well?


class Bond
{
private:
  //maturities of cash flows in terms of yrs
  std::vector<double> mats;

  //cash flows
  std::vector<double> cfs;

  //annual basis
  double coupon;

  //Maturity in terms of yrs
  double mat;
  
  //# of payments per year
  int freq;

public:

  Bond(const std::vector<double>& in_mats, const double& in_coupon, const double& in_mat, const int& in_freq);

  ~Bond();

  //Member Functions

  //input vector of zero rates should have one zero per cf
  double T_Price(const std::vector<double>& in_zeros) const;

  //calculates clean price of a bond given its cont. compounded yield per annum
  double Price(const double& in_y) const; 

  //calculates yield given clean price of a bond
  double Yield(const double& in_px, const double& x0, const double& prec) const; 

  //calculates duration given a vector of zeros - calculates theoretical price and yield as part of calculations
  //acc_i = accrued interest per $1 notional -> added directly to clean price to define B
  double Duration(const std::vector<double>& in_zeros, const double& x0, const double& prec, const double& acc_i = 0.0) const;

  //calculates duration given a continuously compounded yield per annum
  double Duration(const double& in_y, const double& acc_i = 0.0) const;

  //calculates duration given a yield compounded in_m times per annum
  double MDuration(const double& in_y, const double& in_m, const double& acc_i = 0.0) const;

  //calculates convexity given a continuously compounded yield per annum
  double Convexity(const double& in_y, const double& acc_i = 0.0) const;

  //calculates PV01 = change in bond value for a 1 basis point (0.01%) change in the cont. compounded yield per annum 
  double PV01(const double& in_y) const;

  //calculates implied probability of default, as per Hull's approach
  //input arguments are vector of risk-free rates, recovery rate of this security
  //in_yld0 = market yield of this security
  //this assumes a constant default probability per period
  double ImpliedPD(const std::vector<double>& in_zeros, const double& in_rr, const double& in_yld0) const;

  //this function calculates the intermediate default intensity
  //in_prdi = prior default intensity
  //in_break = index after maturity that will be covered by prior default intensity
  double ImpliedPD1(const std::vector<double>& in_zeros, const double& in_rr, const double& in_yld0, const double& in_prdi, const int& in_break) const;

  //this function calculates the intermediate default intensity given two prior default intensities - good for 2 intermediary periods = 3 periods total
  //in_prdi0 = prior default intensity for first intermediate period
  //in_break0 = index after maturity that will be covered by intermediate default intensity 0 (first period)
  //in_prdi1 = prior default intensity for second intermediate period
  //in_break1 = index after maturity that will be coverd by intermediate default intensity 1 (second period)
  double ImpliedPD2(const std::vector<double>& in_zeros, const double& in_rr, const double& in_yld0, const double& in_prdi0, const int& in_break0, const double& in_prdi1, const int& in_break1) const;

};





#endif
