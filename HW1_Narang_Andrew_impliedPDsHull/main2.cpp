//Andrew Narang
//Credit Risk Management - Assignment #1

//testing of new ImpliedPD functionality within the Bond class

#include <iostream>
#include <vector>

#include "IR0.hpp"
#include "Bond.hpp"

int main()
{
  {
    int freq = 2;
    double coupon = 5.0/100.0;
    std::vector<double> mats({0.5, 1.0, 1.5, 2.0, 2.5, 3.0});

    Bond prof0(mats, coupon, 3.0, freq);

    double prof0_y = 6.5/100.0;

    std::cout << "bond prof0 market px: " << prof0.Price(prof0_y) << std::endl;

    double rr0 = 0.6;

    std::vector<double> zeros(mats.size(), 0.04);

    std::vector<double> fwds(mats.size(), 0.0);

    FR_curve(zeros, mats, fwds);

    /*
    for (int i = 0; i < mats.size(); ++ i)
      {
	std::cout << mats[i] << "\t\t" << zeros[i] << "\t\t" << fwds[i] << std::endl;
      }
    */
    
    std::cout << "bond prof0 implied pd per year for this maturity: " << prof0.ImpliedPD(zeros, rr0, prof0_y) << std::endl;
  }
  
  return 0;
}
