//Andrew Narang
//Credit Risk Management - Assignment #1

#include <vector>
#include <cmath>
#include <iostream>

#include "Bond.hpp"
#include "Functors.hpp"
#include "NewtonRaphson.hpp"
#include "IR0.hpp"

Bond::Bond(const std::vector<double>& in_mats, const double& in_coupon, const double& in_mat, const int& in_freq) : mats(in_mats), cfs(std::vector<double>(mats.size(), 0)), coupon(in_coupon), mat(in_mat), freq(in_freq)
{
  for (int i = 0; i < mats.size(); ++i)
    {
      cfs[i] = coupon/freq;
      //std::cout << cfs[i] << std::endl;
    }

  cfs[mats.size() - 1] += 1;

  //std::cout << "Bond object constructed." << std::endl;
}


Bond::~Bond()
{
  //std::cout << "Bond object destructed." << std::endl;
}


double Bond::T_Price(const std::vector<double>& in_zeros) const
{
  double pv = 0;

  for (int i = 0; i < mats.size(); ++i)
    {
      pv += cfs[i] * exp(-1.0 * mats[i] * in_zeros[i]);
    }

  return pv; 
}


//calculates price of a bond given its yield
double Bond::Price(const double& in_y) const
{
  double pv = 0;

  for (int i = 0; i < mats.size(); ++i)
    {
      pv += cfs[i] * exp(-1.0 * mats[i] * in_y);
    }

  return pv; 
}


double Bond::Yield(const double& in_px, const double& x0, const double& prec) const
{
  f a(mats, in_px, coupon/freq);

  f_prime a1(mats, coupon/freq);

  return newton_raphson(a, a1, x0, prec); 
}


double Bond::Duration(const std::vector<double>& in_zeros, const double& x0, const double& prec, const double& acc_i) const
{
  double t_px = T_Price(in_zeros);

  double t_y = Yield(t_px, x0, prec);

  double pv = 0;
  
  for (int i = 0; i < mats.size(); ++i)
    {
      pv += mats[i] * cfs[i] * exp(-1.0 * t_y * mats[i]);
    }

  return pv/(t_px + acc_i); 
}


double Bond::Duration(const double& in_y, const double& acc_i) const
{
  double pv = 0;

  for (int i = 0; i < mats.size(); ++i)
    {
      pv += mats[i] * cfs[i] * exp(-1.0 * in_y * mats[i]);
    }

  return pv/(Price(in_y) + acc_i);
}


double Bond::MDuration(const double& in_y, const double& in_m, const double& acc_i) const
{
  double duration = Duration(R_m2c(in_y, in_m), acc_i);

  return duration/(1 + in_y/in_m); 
}


//calculates convexity given a continuously compounded yield per annum
double Bond::Convexity(const double& in_y, const double& acc_i) const
{
  double pv = 0;

  for (int i = 0; i < mats.size(); ++i)
    {
      pv += cfs[i] * mats[i] * mats[i] * exp(-1.0 * in_y * mats[i]);
    }

  return pv/(Price(in_y) + acc_i);  
}


double Bond::PV01(const double& in_y) const
{
  return (Price(in_y + 0.0001) - Price(in_y)); 
}


//calculates implied probability of default, as per Hull's approach
//input arguments are vector of risk-free rates, recovery rate of this security
//in_yld0 = market yield of this security
//this assumes a constant default probability per period 
double Bond::ImpliedPD(const std::vector<double>& in_zeros, const double& in_rr, const double& in_yld0) const
{
  //for (int i = 0; i < cfs.size(); ++i) std::cout << cfs[i] << ' '; 
  //std::cout << std::endl;
  
  //calculate theoretical risk-free price at t0
  double rf_px0 = T_Price(in_zeros);

  //std::cout << rf_px0 << std::endl;
  
  //calculate theoretical risky price (from market) at t0
  double mk_px0 = Price(in_yld0);

  //set up the forward curve associated with the input zeros
  std::vector<double> fwds(in_zeros.size(), 0.0);

  FR_curve(in_zeros, mats, fwds);

  //determine present value of total losses given defaults at each maturity

  double pv_lgds = 0.0;

  //start at i = 0 since mat[0] = time of the next cash flow from time t = 0
  for (int i = 0; i < mats.size(); ++i)
    {
      //first, calculate the risk-free price of the bond at this maturity using the forward curve
      double px = cfs[i];

      //std::cout << px << std::endl;
      
      for (int i1 = i + 1; i1 < mats.size(); ++i1)
	{
	  //determine appropriate discounting factor
	  double df = 1.0;

	  for (int i2 = i1; i2 > i; --i2)
	    {
	      df *= exp(-1.0 * fwds[i2] * (mats[i2] - mats[i2 - 1]));
	    }

	  //std::cout << (cfs[i1] * df) << '\t';
	  
	  //add it to the present value
	  px += (cfs[i1] * df);
	}
      //px is now = theoretical risk-free price of bond at this maturity

      //std::cout << px << std::endl;
      
      //calculate loss given default @ this maturity, discount it back via corr. zero rate, and add it to the pv of all expected losses
      pv_lgds += (in_rr - px) * exp(-1.0 * in_zeros[i] * mats[i]);
    }

  //now, we know the net present values of all losses given defaults

  return std::abs(mk_px0 - rf_px0)/std::abs(pv_lgds) * freq; 
}


//this function calculates the intermediate default intensity
//in_prdi = prior default intensity
//in_break = index after maturity that will be covered by prior default intensity
double Bond::ImpliedPD1(const std::vector<double>& in_zeros, const double& in_rr, const double& in_yld0, const double& in_prdi, const int& in_break) const
{
  //calculate theoretical risk-free price at t0
  double rf_px0 = T_Price(in_zeros);

  //std::cout << rf_px0 << std::endl;
  
  //calculate theoretical risky price (from market) at t0
  double mk_px0 = Price(in_yld0);

  //set up the forward curve associated with the input zeros
  std::vector<double> fwds(in_zeros.size(), 0.0);

  FR_curve(in_zeros, mats, fwds);

  //determine present value of total losses given defaults at each maturity

  double pv_lgds = 0.0;

  double pv_lgds1 = 0.0;
  
  //start at i = 0 since mat[0] = time of the next cash flow from time t = 0
  for (int i = 0; i < in_break; ++i)
    {
      //first, calculate the risk-free price of the bond at this maturity using the forward curve
      double px = cfs[i];

      //std::cout << px << std::endl;
      
      for (int i1 = i + 1; i1 < mats.size(); ++i1)
	{
	  //determine appropriate discounting factor
	  double df = 1.0;

	  for (int i2 = i1; i2 > i; --i2)
	    {
	      df *= exp(-1.0 * fwds[i2] * (mats[i2] - mats[i2 - 1]));
	    }

	  //std::cout << (cfs[i1] * df) << '\t';
      
	  //add it to the present value
	  px += (cfs[i1] * df);
	}
      //px is now = theoretical risk-free price of bond at this maturity

      //std::cout << px << std::endl;
      
      //calculate loss given default @ this maturity, discount it back via corr. zero rate, and add it to the pv of all expected losses
      pv_lgds += (in_rr - px) * exp(-1.0 * in_zeros[i] * mats[i]);
    }

  //now, we know the net present values of all losses given defaults covered by the prior default intensity
  //start at i = in_break since mat[in_break] = time of the first cash flow coveredy by intermediate default intensity
  for (int i = in_break; i < mats.size(); ++i)
    {
      //first, calculate the risk-free price of the bond at this maturity using the forward curve
      double px = cfs[i];

      //std::cout << px << std::endl;
        
      for (int i1 = i + 1; i1 < mats.size(); ++i1)
	{
	  //determine appropriate discounting factor
	  double df = 1.0;

	  for (int i2 = i1; i2 > i; --i2)
	    {
	      df *= exp(-1.0 * fwds[i2] * (mats[i2] - mats[i2 - 1]));
	    }

	  //std::cout << (cfs[i1] * df) << '\t';
        
	  //add it to the present value
	  px += (cfs[i1] * df);
	}
      //px is now = theoretical risk-free price of bond at this maturity

      //std::cout << px << std::endl;
      
      //calculate loss given default @ this maturity, discount it back via corr. zero rate, and add it to the pv of all expected losses
      pv_lgds1 += (in_rr - px) * exp(-1.0 * in_zeros[i] * mats[i]);
      //std::cout << pv_lgds1 << std::endl;
    }

  //now, we know the net present values of all losses given defaults after the initial period
     
  // return std::abs(mk_px0 - rf_px0)/std::abs(pv_lgds) * freq;

  //std::cout << pv_lgds1 << std::endl;
  
  return std::abs( (std::abs(mk_px0 - rf_px0) - std::abs(pv_lgds) * (in_prdi / freq))/std::abs(pv_lgds1) * freq ); 
}



  

