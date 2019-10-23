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

  //Now we do the analysis for real bonds
  
  double rf_1yr = 1.59/100.0;
  double rf_2yr = 1.60/100.0;
  double rf_3yr = 1.59/100.0;
  double rf_5yr = 1.60/100.0; 

  //all corp bonds are semiannual and using 30/360 day count convention
  int freq = 2;

  //all bonds have same recovery rate
  double rr0 = 0.4; 
  
  std::cout << std::endl << std::endl; 
  
  {
    //GE 1 year
    //matures on 11/15/20
    //next coupon date = 11/15/20
    double coupon1 = 2.342/100.0;

    int pmts_left1 = (2020-2019) * freq;
    ++pmts_left1;
    
    std::vector<double> mats1(pmts_left1, 0.0);
    mats1[0] = (8.0 + 15.0)/360.0;
    for (int i = 1; i < pmts_left1; ++i) mats1[i] = mats1[0] + (0.5 * i);

    Bond GE1(mats1, coupon1, mats1[pmts_left1 - 1], freq); 

    double GE1px = 99.863/100.0;
    double GE1y = GE1.Yield(GE1px, 0.1, 0.000001);

    std::cout << "GE1 yield: " << GE1y << std::endl;

    std::vector<double> zeros1(mats1.size(), rf_1yr); 
    
    double GE1_pd = GE1.ImpliedPD(zeros1, rr0, GE1y);

    std::cout << "GE1 implied pd: " << GE1_pd << std::endl;

    //GE 3year
    //matures on 10/09/22
    //next coupon date is on 5/09/2020
    double coupon3 = 2.7/100.0;

    int pmts_left3 = (2022 - 2019) * freq;

    std::vector<double> mats3(pmts_left3, 0.0);

    mats3[0] = (8.0 + 9.0 + 6.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left3; ++i) mats3[i] = mats3[0] + 0.5 * i;

    Bond GE3(mats3, coupon3, mats3[pmts_left3 - 1], freq);

    double GE3px = 100.307/100.0;
    double GE3y = GE3.Yield(GE3px, 0.1, 0.0001);
    std::cout << "\nGE3 bond yield: " << GE3y << std::endl;

    std::vector<double> zeros3(mats3.size(), rf_3yr);
    
    double GE3_pd = GE3.ImpliedPD(zeros3, rr0, GE3y);

    std::cout << "GE3 implied pd: " << GE3_pd << std::endl;

    double GE13_pd = GE3.ImpliedPD1(zeros3, rr0, GE3y, GE1_pd, 2);

    std::cout << "GE13 intermediate pd: " << GE13_pd << std::endl; 
    
    //GE 5yr
    //matures on 11/15/24
    //next coupon date is 11/15/2019
    double coupon5 = 4.2/100.0;

    int pmts_left5 = (2024 - 2019) * freq;
    ++pmts_left5;  //still have one more coupon this year

    std::vector<double> mats5(pmts_left5, 0.0);
    mats5[0] = (8.0 + 15.0)/360.0;
    for (int i = 1; i < pmts_left5; ++i) mats5[i] = mats5[0] + 0.5 * i;

    Bond GE5(mats5, coupon5, mats5[pmts_left5 - 1], freq);

    double GE5px = 100.957/100.0;
    double GE5y = GE5.Yield(GE5px, 0.1, 0.0001);
    std::cout << "\nGE5 bond yield: " << GE5y << std::endl;

    std::vector<double> zeros5(mats5.size(), rf_5yr);

    double GE5_pd = GE5.ImpliedPD(zeros5, rr0, GE5y);

    std::cout << "GE5 implied pd: " << GE5_pd << std::endl;

    double GE35_pd = GE5.ImpliedPD1(zeros5, rr0, GE5y, GE3_pd, 7);

    std::cout << "GE35 intermediate pd: " << GE35_pd << std::endl;
    
  }

  {
    //AAPL 1 year
    //matures on 11/13/20
    //next coupon date = 11/13/20
    double coupon1 = 2.0/100.0;
    
    int pmts_left1 = (2020-2019) * freq;
    ++pmts_left1; //one more this year
    
    std::vector<double> mats1(pmts_left1, 0.0);
    mats1[0] = (8.0 + 13.0)/360.0;
    for (int i = 1; i < pmts_left1; ++i) mats1[i] = mats1[0] + (0.5 * i);
    
    Bond aapl1(mats1, coupon1, mats1[pmts_left1 - 1], freq);
    
    double aapl1px = 100.264/100.0;
    ///////////////////////UPDATE FROM HERE ON 
    double GE1y = GE1.Yield(GE1px, 0.1, 0.000001);
    
    std::cout << "GE1 yield: " << GE1y << std::endl;
    
    std::vector<double> zeros1(mats1.size(), rf_1yr);
    
    double GE1_pd = GE1.ImpliedPD(zeros1, rr0, GE1y);
    
    std::cout << "GE1 implied pd: " << GE1_pd << std::endl;
    
    //GE 3year
    //matures on 10/09/22
    //next coupon date is on 5/09/2020
    double coupon3 = 2.7/100.0;
    
    int pmts_left3 = (2022 - 2019) * freq;
    
    std::vector<double> mats3(pmts_left3, 0.0);
    
    mats3[0] = (7.0 + 9.0 + 6.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left3; ++i) mats3[i] = mats3[0] + 0.5 * i;
    
    Bond GE3(mats3, coupon3, mats3[pmts_left3 - 1], freq);
    
    double GE3px = 100.307/100.0;
    double GE3y = GE3.Yield(GE3px, 0.1, 0.0001);
    std::cout << "\nGE3 bond yield: " << GE3y << std::endl;
    
    std::vector<double> zeros3(mats3.size(), rf_3yr);
    
    double GE3_pd = GE3.ImpliedPD(zeros3, rr0, GE3y);
    
    std::cout << "GE3 implied pd: " << GE3_pd << std::endl;
    
    double GE13_pd = GE3.ImpliedPD1(zeros3, rr0, GE3y, GE1_pd, 2);
    
    std::cout << "GE13 intermediate pd: " << GE13_pd << std::endl;
    
    //GE 5yr
    //matures on 11/15/24
    //next coupon date is 11/15/2019
    double coupon5 = 4.2/100.0;
    
    int pmts_left5 = (2024 - 2019) * freq;
    ++pmts_left5;  //still have one more coupon this year
    
    std::vector<double> mats5(pmts_left5, 0.0);
    mats5[0] = (7.0 + 15.0)/360.0;
    for (int i = 1; i < pmts_left5; ++i) mats5[i] = mats5[0] + 0.5 * i;
    
    Bond GE5(mats5, coupon5, mats5[pmts_left5 - 1], freq);
    
    double GE5px = 100.957/100.0;
    double GE5y = GE5.Yield(GE5px, 0.1, 0.0001);
    std::cout << "\nGE5 bond yield: " << GE5y << std::endl;
    
    std::vector<double> zeros5(mats5.size(), rf_5yr);
    
    double GE5_pd = GE5.ImpliedPD(zeros5, rr0, GE5y);
    
    std::cout << "GE5 implied pd: " << GE5_pd << std::endl;
    
    double GE35_pd = GE5.ImpliedPD1(zeros5, rr0, GE5y, GE3_pd, 7);
    
    std::cout << "GE35 intermediate pd: " << GE35_pd << std::endl;
    
    }
    
    
  return 0;
}
