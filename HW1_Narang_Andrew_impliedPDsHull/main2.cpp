//Andrew Narang
//Credit Risk Management - Assignment #1



#include <iostream>
#include <vector>

#include "IR0.hpp"
#include "Bond.hpp"

int main()
{
  {
    //testing of new ImpliedPD functionality within the Bond class using Prof's posted spreadsheet
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

    double pd3 = prof0.ImpliedPD(zeros, rr0, prof0_y);
    
    std::cout << "bond prof0 implied pd per year for this maturity: " << pd3 << std::endl;

    //calculation of bond prof1 - second example

    std::vector<double> mats1({0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0});
    double coupon1 = 5.5/100.0;

    Bond prof1(mats1, coupon1, 5.0, freq);

    double prof1_y = 7.5/100.0;

    std::cout << "bond prof1 market px: " << prof1.Price(prof1_y) << std::endl;

    std::vector<double> zeros1(mats1.size(), 0.04);
    std::vector<double> fwds1(mats1.size(), 0.0);
    FR_curve(zeros1, mats1, fwds1);

    double pd5 = prof1.ImpliedPD(zeros1, rr0, prof1_y);

    std::cout << "bond prof1 implied pd per year for this maturity: " << pd5 << std::endl;

    std::cout << "bond prof1 intermediate implied pd per year between yrs 3 and 5: " << prof1.ImpliedPD1(zeros1, rr0, prof1_y, pd3, 6) << std::endl;
    
  }

  double rf_1yr = 0.0;
  double rf_3yr = 0.0;
  double rf_5yr = 0.0; 
  
  //Now we do the analysis for real bonds
  {
    //GE
  }
  
  return 0;
}
