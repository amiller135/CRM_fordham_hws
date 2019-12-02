//Andrew Narang
//CRM Assignment 2

//testing out the hazard rate function

//problems referenced are from Hull's Risk Management and Financial Institutions, 4th Edition

#include <iostream>

#include "IR0.hpp"

int main()
{
  //the implied pd functions calculate the unconditional pds during that time period, which corresponds with ucpd1 in these examples.
  //cupd0 = implied pd of the bond leading up to the period being evaluated, like that of the 3-yr bond when estimating hazard rate for 3yr-5yr period
  
  //problem 19.3
  { 
    double ucpd1 = 3.904/100.0;
    double cupd0 = 0.0;
    double hzd1 = hzd(ucpd1, cupd0);

    std::cout << "problem 19.3 hazard rate result: " << hzd1 << std::endl;
    
  }

  //problem 19.4
  {
    double ucpd3 = (5.371 - 3.071)/100.0;
    double cupd2 = 3.071/100.0;

    double hzd3 = hzd(ucpd3, cupd2);

    std::cout << "problem 19.4 hazard rate result: " << hzd3 << std::endl;
  }
  
  return 0;
}
