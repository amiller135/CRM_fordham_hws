//Andrew Narang
//Credit Risk Management - Assignment #1

//All calculations are as of 10/22/19


#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

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

    std::cout << "bond prof1 intermediate implied pd per year between yrs 3 and 5: " << std::fixed << std::setprecision(8) << prof1.ImpliedPD1(zeros1, rr0, prof1_y, pd3, 6) << std::endl;
    
  }

  //Now we do the analysis for real bonds
  double rf_1mo = 1.75/100.0;
  double rf_2mo = 1.69/100.0;
  double rf_3mo = 1.65/100.0;
  double rf_6mo = 1.64/100.0;
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

    std::string target0("GE_implied_pds.txt");
    std::string target1("GE_hzds.txt");
    
    std::ofstream GE_ipds(target0);

    if (GE_ipds.fail())
      {
	std::cerr << "could not write " << target0 << '\n';
	return -1;
      }

    std::ofstream GE_hzds(target1);

    if (GE_hzds.fail())
      {
	std::cerr << "could not write " << target1 << '\n';
	return -2;
      }
    
    GE_ipds << "GE1 yield: " << GE1y << std::endl;

    std::vector<double> zeros1(mats1.size(), rf_1yr); 

    //make it closer to actual treasury yield curve data
    zeros1[0] = rf_1mo;
    zeros1[1] = rf_6mo;
    
    double GE1_pd = GE1.ImpliedPD(zeros1, rr0, GE1y);

    GE_ipds << "GE1 implied pd: " << GE1_pd << std::endl;

    ///calculation of corresponding hazard rates

    double GE01_hzd = hzd(GE1_pd, 0.0);

    GE_hzds << "GE01 hazard rate: " << GE01_hzd << std::endl;
    
    //GE 3year
    //matures on 10/09/22
    //next coupon date is on 04/09/2020
    double coupon3 = 2.7/100.0;

    int pmts_left3 = (2022 - 2019) * freq;

    std::vector<double> mats3(pmts_left3, 0.0);

    mats3[0] = (8.0 + 9.0 + 5.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left3; ++i) mats3[i] = mats3[0] + 0.5 * i;

    Bond GE3(mats3, coupon3, mats3[pmts_left3 - 1], freq);

    double GE3px = 100.307/100.0;
    double GE3y = GE3.Yield(GE3px, 0.1, 0.0001);
    GE_ipds << "\nGE3 bond yield: " << GE3y << std::endl;

    std::vector<double> zeros3(mats3.size(), rf_3yr);

    //make the zero curve more accurate
    zeros3[0] = rf_6mo;
    zeros3[1] = rf_1yr;
    zeros3[2] = rf_1yr;
    zeros3[3] = rf_2yr;
    zeros3[4] = rf_2yr;
    
    double GE3_pd = GE3.ImpliedPD(zeros3, rr0, GE3y);

    GE_ipds << "GE3 implied pd: " << GE3_pd << std::endl;

    double GE13_pd = GE3.ImpliedPD1(zeros3, rr0, GE3y, GE1_pd, 2);

    GE_ipds << "GE13 intermediate pd: " << GE13_pd << std::endl; 

    //calculation of corresponding hazard rate for yrs 1-3

    double GE13_hzd = hzd(GE13_pd, GE1_pd);

    GE_hzds << "GE13 hazard rate: " << GE13_hzd << std::endl;
    
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
    GE_ipds << "\nGE5 bond yield: " << GE5y << std::endl;

    std::vector<double> zeros5(mats5.size(), rf_5yr);

    //make the zeros curve more accurate
    zeros5[0] = rf_1mo;
    zeros5[1] = rf_6mo;
    for (int i = 2; i < 4; ++i) zeros5[i] = rf_1yr;
    for (int i = 4; i < 6; ++i) zeros5[i] = rf_2yr;
    for (int i = 6; i < 10; ++i) zeros5[i] = rf_3yr;
    
    double GE5_pd = GE5.ImpliedPD(zeros5, rr0, GE5y);

    GE_ipds << "GE5 implied pd: " << GE5_pd << std::endl;

    //next 3 pmts are within approx. a year, so we use the implied unconditional pd from GE1 for them                                  
    double GE35_pd = GE5.ImpliedPD2(zeros5, rr0, GE5y, GE1_pd, 3, GE13_pd, 7);

    GE_ipds << "GE35 intermediate pd: " << GE35_pd << std::endl;

    //calculation of hazard rate corresponding to GE yrs 3-5
    double GE35_hzd = hzd(GE35_pd, GE3_pd);

    GE_hzds << "GE35 hazard rate: " << GE35_hzd << std::endl;
    
    GE_ipds.close();
    GE_hzds.close();
  }

  {
    std::string target0("AAPL_implied_pds.txt");
    std::string target1("AAPL_hzds.txt");

    std::ofstream AAPL_ipds(target0);

    if (AAPL_ipds.fail())
      {
        std::cerr << "could not write " << target0 << '\n';
        return -1;
      }

    std::ofstream AAPL_hzds(target1);

    if (AAPL_hzds.fail())
      {
        std::cerr << "could not write " << target1 << '\n';
        return -2;
      }

    AAPL_ipds << "A = AAPL:" << std::endl;

    AAPL_hzds << "A = AAPL:" << std::endl;
    
    //AAPL 1 year
    //matures on 11/13/20
    //next coupon date = 11/13/20
    double coupon1 = 2.0/100.0;
    
    int pmts_left1 = (2020-2019) * freq;
    ++pmts_left1; //one more this year
    
    std::vector<double> mats1(pmts_left1, 0.0);
    mats1[0] = (8.0 + 13.0)/360.0;
    for (int i = 1; i < pmts_left1; ++i) mats1[i] = mats1[0] + (0.5 * i);
    ///generic naming from here on out
    Bond A1(mats1, coupon1, mats1[pmts_left1 - 1], freq);
    
    double A1px = 100.264/100.0;
    double A1y = A1.Yield(A1px, 0.1, 0.000001);
    
    AAPL_ipds << "A1 yield: " << A1y << std::endl;
    
    std::vector<double> zeros1(mats1.size(), rf_1yr);
    
    double A1_pd = A1.ImpliedPD(zeros1, rr0, A1y);
    
    AAPL_ipds << "A1 implied pd: " << A1_pd << std::endl;

    //calculation of hazard rate corresponding with AAPL yrs 0-1
    double A01_hzd = hzd(A1_pd, 0.0);

    AAPL_hzds << "A01 hazard rate: " << A01_hzd << std::endl;
    
    //AAPL 3year
    //matures on 09/11/22
    //next coupon date is on 3/11/2020
    double coupon3 = 1.7/100.0;
    
    int pmts_left3 = (2022 - 2019) * freq;
    
    std::vector<double> mats3(pmts_left3, 0.0);
    
    mats3[0] = (8.0 + 11.0 + 4.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left3; ++i) mats3[i] = mats3[0] + 0.5 * i;
    
    Bond A3(mats3, coupon3, mats3[pmts_left3 - 1], freq);
    
    double A3px = 100.307/100.0;
    double A3y = A3.Yield(A3px, 0.1, 0.0001);
    AAPL_ipds << "\nA3 bond yield: " << A3y << std::endl;
    
    std::vector<double> zeros3(mats3.size(), rf_3yr);
    
    double A3_pd = A3.ImpliedPD(zeros3, rr0, A3y);
    
    AAPL_ipds << "A3 implied pd: " << A3_pd << std::endl;
    
    double A13_pd = A3.ImpliedPD1(zeros3, rr0, A3y, A1_pd, 2);
    
    AAPL_ipds << "A13 intermediate pd: " << A13_pd << std::endl;

    //calculation of hazard rate corresponding with AAPL yrs 1-3
    double A13_hzd = hzd(A13_pd, A1_pd);

    AAPL_hzds << "A13 hazard rate: " << A13_hzd << std::endl;
    
    //AAPL 5yr
    //matures on 05/06/24
    //next coupon date is 12/06/2019
    double coupon5 = 3.45/100.0;
    
    int pmts_left5 = (2024 - 2019) * freq;
    ++pmts_left5;  //still have one more coupon this year
    
    std::vector<double> mats5(pmts_left5, 0.0);
    mats5[0] = (8.0 + 6.0 + 30.0)/360.0;
    for (int i = 1; i < pmts_left5; ++i) mats5[i] = mats5[0] + 0.5 * i;
    
    Bond A5(mats5, coupon5, mats5[pmts_left5 - 1], freq);
    
    double A5px = 100.957/100.0;
    double A5y = A5.Yield(A5px, 0.1, 0.0001);
    AAPL_ipds << "\nA5 bond yield: " << A5y << std::endl;
    
    std::vector<double> zeros5(mats5.size(), rf_5yr);
    
    double A5_pd = A5.ImpliedPD(zeros5, rr0, A5y);
    
    AAPL_ipds << "A5 implied pd: " << A5_pd << std::endl;

    //next 3 pmts are within approx. a year, so we use the implied unconditional pd from A1 for them
    double A35_pd = A5.ImpliedPD2(zeros5, rr0, A5y, A1_pd, 3, A13_pd, 7);
    
    AAPL_ipds << "A35 intermediate pd: " << A35_pd << std::endl;

    //calculation of hazard rate corresponding with AAPL yrs 3-5
    double A35_hzd = hzd(A35_pd, A3_pd);

    AAPL_hzds << "A35 hazard rate: " << A35_hzd << std::endl;
    
    AAPL_ipds.close(); AAPL_hzds.close(); 
  }
    
  {
    std::string target0("TSLA_implied_pds.txt");
    std::string target1("TSLA_hzds.txt");

    std::ofstream ipds(target0);

    if (ipds.fail())
      {
        std::cerr << "could not write " << target0 << '\n';
        return -1;
      }

    std::ofstream hzds(target1);

    if (hzds.fail())
      {
        std::cerr << "could not write " << target1 << '\n';
        return -2;
      }

    ipds << "A = TSLA:" << std::endl;

    hzds << "A = TSLA:" << std::endl;
    
    //TSLA 2 year
    //matures on 03/01/21
    //next coupon date = 03/01/20
    double coupon1 = 1.25/100.0;
    
    int pmts_left1 = (2021-2019) * freq;
    --pmts_left1; //this bond matures sooner in 2021 than the others
    
    std::vector<double> mats1(pmts_left1, 0.0);
    mats1[0] = (8.0 + 4.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left1; ++i) mats1[i] = mats1[0] + (0.5 * i);
    
    Bond A2(mats1, coupon1, mats1[pmts_left1 - 1], freq);
      
    double A2px = 100.264/100.0;
    double A2y = A2.Yield(A2px, 0.1, 0.000001);
      
    ipds << "A2 yield: " << A2y << std::endl;
      
    std::vector<double> zeros1(mats1.size(), rf_2yr);
      
    double A2_pd = A2.ImpliedPD(zeros1, rr0, A2y);
      
    ipds << "A2 implied pd: " << A2_pd << std::endl;

    //calculation of hazard rate corresponding with TSLA yrs 0-2

    double A02_hzd = hzd(A2_pd, 0.0);

    hzds << "A02 hazard rate: " << A02_hzd << std::endl;
    
    //TSLA 3year
    //matures on 03/15/22
    //next coupon date is on 03/15/2020
    double coupon3 = (2.0 + 3.0/8.0)/100.0;
      
    int pmts_left3 = (2022 - 2019) * freq;
      
    std::vector<double> mats3(pmts_left3, 0.0);
      
    mats3[0] = (8.0 + 15.0 + 4.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left3; ++i) mats3[i] = mats3[0] + 0.5 * i;
      
    Bond A3(mats3, coupon3, mats3[pmts_left3 - 1], freq);
      
    double A3px = 100.307/100.0;
    double A3y = A3.Yield(A3px, 0.1, 0.0001);
    ipds << "\nA3 bond yield: " << A3y << std::endl;
      
    std::vector<double> zeros3(mats3.size(), rf_3yr);
      
    double A3_pd = A3.ImpliedPD(zeros3, rr0, A3y);
      
    ipds << "A3 implied pd: " << A3_pd << std::endl;
      
    double A23_pd = A3.ImpliedPD1(zeros3, rr0, A3y, A2_pd, 2);
      
    ipds << "A23 intermediate pd: " << A23_pd << std::endl;

    //calculation of hazard rate corresponding with TSLA yrs 2-3
    double A23_hzd = hzd(A23_pd, A2_pd);

    hzds << "A23 hazard rate: " << A23_hzd << std::endl;

    //TSLA 5yr
    //matures on 05/15/24
    //next coupon date is 12/15/2019
    double coupon5 = 2.0/100.0;
      
    int pmts_left5 = (2024 - 2019) * freq;
    ++pmts_left5;  //still have one more coupon this year
      
    std::vector<double> mats5(pmts_left5, 0.0);
    mats5[0] = (8.0 + 15.0 + 30.0)/360.0;
    for (int i = 1; i < pmts_left5; ++i) mats5[i] = mats5[0] + 0.5 * i;
      
    Bond A5(mats5, coupon5, mats5[pmts_left5 - 1], freq);
      
    double A5px = 100.957/100.0;
    double A5y = A5.Yield(A5px, 0.1, 0.0001);
    ipds << "\nA5 bond yield: " << A5y << std::endl;
      
    std::vector<double> zeros5(mats5.size(), rf_5yr);
      
    double A5_pd = A5.ImpliedPD(zeros5, rr0, A5y);
      
    ipds << "A5 implied pd: " << A5_pd << std::endl;

    //next 3 pmts are within approx. a year, so we use the implied unconditional pd from A2 for them and the 2 after them
    double A35_pd = A5.ImpliedPD2(zeros5, rr0, A5y, A2_pd, 5, A23_pd, 7); 
      
    ipds << "A35 intermediate pd: " << A35_pd << std::endl;

    //calculation of hazard rate corresponding with TSLA yrs 3-5
    double A35_hzd = hzd(A35_pd, A3_pd);

    hzds << "A35 hazard rate: " << A35_hzd << std::endl;

    ipds.close(); hzds.close(); 
  }
    
  {
    std::string target0("MSFT_implied_pds.txt");
    std::string target1("MSFT_hzds.txt");

    std::ofstream ipds(target0);

    if (ipds.fail())
      {
        std::cerr << "could not write " << target0 << '\n';
        return -1;
      }

    std::ofstream hzds(target1);

    if (hzds.fail())
      {
        std::cerr << "could not write " << target1 << '\n';
        return -2;
      }

    ipds << "A = MSFT:" << std::endl;

    hzds << "A = MSFT:" << std::endl;

    //MSFT 1 year
    //matures on 10/01/20
    //next coupon date = 04/01/20
    double coupon1 = 3.0/100.0;
      
    int pmts_left1 = (2020-2019) * freq;
      
    std::vector<double> mats1(pmts_left1, 0.0);
    mats1[0] = (8.0 + 5.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left1; ++i) mats1[i] = mats1[0] + (0.5 * i);
    ///generic naming from here on out
    Bond A1(mats1, coupon1, mats1[pmts_left1 - 1], freq);
      
    double A1px = 100.264/100.0;
    double A1y = A1.Yield(A1px, 0.1, 0.000001);
      
    ipds << "A1 yield: " << A1y << std::endl;
      
    std::vector<double> zeros1(mats1.size(), rf_1yr);
      
    double A1_pd = A1.ImpliedPD(zeros1, rr0, A1y);
      
    ipds << "A1 implied pd: " << A1_pd << std::endl;

    //calculation of corresponding hazard rates for MSFT yrs 0-1
    double A01_hzd = hzd(A1_pd, 0.0);
    hzds << "A01 hazard rate: " << A01_hzd << std::endl;
    
    //MSFT 3year
    //matures on 11/15/22
    //next coupon date is on 11/15/2019
    double coupon3 = (2.0 + 1.0/8.0)/100.0;
      
    int pmts_left3 = (2022 - 2019) * freq;
    ++pmts_left3;   
    std::vector<double> mats3(pmts_left3, 0.0);
      
    mats3[0] = (8.0 + 15.0)/360.0;
    for (int i = 1; i < pmts_left3; ++i) mats3[i] = mats3[0] + 0.5 * i;
      
    Bond A3(mats3, coupon3, mats3[pmts_left3 - 1], freq);
      
    double A3px = 100.307/100.0;
    double A3y = A3.Yield(A3px, 0.1, 0.0001);
    ipds << "\nA3 bond yield: " << A3y << std::endl;
      
    std::vector<double> zeros3(mats3.size(), rf_3yr);
    
    double A3_pd = A3.ImpliedPD(zeros3, rr0, A3y);
    
    ipds << "A3 implied pd: " << A3_pd << std::endl;

    //since the next three payments are within roughly a year, we use A1_pd for all three of them
    double A13_pd = A3.ImpliedPD1(zeros3, rr0, A3y, A1_pd, 3);
      
    ipds << "A13 intermediate pd: " << A13_pd << std::endl;

    //calculation of hazard rate corresponding with MSFT yrs 1-3
    double A13_hzd = hzd(A13_pd, A1_pd);

    hzds << "A13 hazard rate: " << A13_hzd << std::endl;
    
    //MSFT 5 yr (really more like 4.5 yrs)
    //matures on 02/06/24
    //next coupon date is 02/06/2020
    double coupon5 = (2.0 + 7.0/8.0)/100.0;
      
    int pmts_left5 = (2024 - 2019) * freq;
    --pmts_left5;  //this bond matures sooner
      
    std::vector<double> mats5(pmts_left5, 0.0);
    mats5[0] = (8.0 + 6.0 + 3.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left5; ++i) mats5[i] = mats5[0] + 0.5 * i;
      
    Bond A5(mats5, coupon5, mats5[pmts_left5 - 1], freq);
      
    double A5px = 100.957/100.0;
    double A5y = A5.Yield(A5px, 0.1, 0.0001);
    ipds << "\nA5 bond yield: " << A5y << std::endl;
    
    std::vector<double> zeros5(mats5.size(), rf_5yr);
      
    double A5_pd = A5.ImpliedPD(zeros5, rr0, A5y);
      
    ipds << "A5 implied pd: " << A5_pd << std::endl;
      
    double A35_pd = A5.ImpliedPD2(zeros5, rr0, A5y, A1_pd, 2, A13_pd, 6);
      
    ipds << "A35 intermediate pd: " << A35_pd << std::endl;

    //calculation of hazard rate corresponding with MSFT yrs 3-5
    double A35_hzd = hzd(A35_pd, A3_pd);

    hzds << "A35 hazard rate: " << A35_hzd << std::endl;

    ipds.close(); hzds.close(); 
  }
    
  {
    std::string target0("GS_implied_pds.txt");
    std::string target1("GS_hzds.txt");

    std::ofstream ipds(target0);

    if (ipds.fail())
      {
        std::cerr << "could not write " << target0 << '\n';
        return -1;
      }

    std::ofstream hzds(target1);

    if (hzds.fail())
      {
        std::cerr << "could not write " << target1 << '\n';
        return -2;
      }

    ipds << "A = GS:" << std::endl;

    hzds << "A = GS:" << std::endl;

    
    //GS 1 year
    //matures on 09/15/20
    //next coupon date = 03/15/20
    double coupon1 = 2.75/100.0;
        
    int pmts_left1 = (2020-2019) * freq;
        
    std::vector<double> mats1(pmts_left1, 0.0);
    mats1[0] = (8.0 + 15.0 + 4 * 30.0)/360.0;
    for (int i = 1; i < pmts_left1; ++i) mats1[i] = mats1[0] + (0.5 * i);
    ///generic naming from here on out
    Bond A1(mats1, coupon1, mats1[pmts_left1 - 1], freq);
        
    double A1px = 100.264/100.0;
    double A1y = A1.Yield(A1px, 0.1, 0.000001);
        
    ipds << "A1 yield: " << A1y << std::endl;
        
    std::vector<double> zeros1(mats1.size(), rf_1yr);
        
    double A1_pd = A1.ImpliedPD(zeros1, rr0, A1y);
        
    ipds << "A1 implied pd: " << A1_pd << std::endl;

    //calculation of hazard rate corresponding with GS yrs 0-1
    double A01_hzd = hzd(A1_pd, 0.0);

    hzds << "A01 hazard rate: " << A01_hzd << std::endl;
    
    //GS 3year
    //matures on 10/31/22
    //next coupon date is on 10/31/2019
    double coupon3 = 2.876/100.0;
        
    int pmts_left3 = (2022 - 2019) * freq;
    ++pmts_left3;
    std::vector<double> mats3(pmts_left3, 0.0);
        
    mats3[0] = 9.0/360.0;
    for (int i = 1; i < pmts_left3; ++i) mats3[i] = mats3[0] + 0.5 * i;
        
    Bond A3(mats3, coupon3, mats3[pmts_left3 - 1], freq);
        
    double A3px = 100.307/100.0;
    double A3y = A3.Yield(A3px, 0.1, 0.0001);
    ipds << "\nA3 bond yield: " << A3y << std::endl;
        
    std::vector<double> zeros3(mats3.size(), rf_3yr);
      
    double A3_pd = A3.ImpliedPD(zeros3, rr0, A3y);
      
    ipds << "A3 implied pd: " << A3_pd << std::endl;

    //next three coupon dates are effectively within a year, so we use A1_pd for all of them
    double A13_pd = A3.ImpliedPD1(zeros3, rr0, A3y, A1_pd, 3);
        
    ipds << "A13 intermediate pd: " << A13_pd << std::endl;

    //calculation of hazard rate corresponding with GS yrs 1-3
    double A13_hzd = hzd(A13_pd, A1_pd);

    hzds << "A13 hazard rate: " << A13_hzd << std::endl;
    
    //GS 5 yr
    //matures on 03/03/24
    //next coupon date is 03/03/2020
    double coupon5 = 4.0/100.0;
        
    int pmts_left5 = (2024 - 2019) * freq;
    --pmts_left5;  //this bond matures sooner
        
    std::vector<double> mats5(pmts_left5, 0.0);
    mats5[0] = (8.0 + 3.0 + 4.0 * 30.0)/360.0;
    for (int i = 1; i < pmts_left5; ++i) mats5[i] = mats5[0] + 0.5 * i;
        
    Bond A5(mats5, coupon5, mats5[pmts_left5 - 1], freq);
        
    double A5px = 100.957/100.0;
    double A5y = A5.Yield(A5px, 0.1, 0.0001);
    ipds << "\nA5 bond yield: " << A5y << std::endl;
      
    std::vector<double> zeros5(mats5.size(), rf_5yr);
        
    double A5_pd = A5.ImpliedPD(zeros5, rr0, A5y);
        
    ipds << "A5 implied pd: " << A5_pd << std::endl;
        
    double A35_pd = A5.ImpliedPD2(zeros5, rr0, A5y, A1_pd, 2, A13_pd, 6); 
        
    ipds << "A35 intermediate pd: " << A35_pd << std::endl;

    //calculation of hazard rate corresponding with GS yrs 3-5
    double A35_hzd = hzd(A35_pd, A3_pd);

    hzds << "A35 hazard rate: " << A35_hzd << std::endl;

    ipds.close(); hzds.close(); 
  }
    
    
  return 0;
}
