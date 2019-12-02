//Andrew Narang
//testing out overload of stream operator for easier, more precise data entry
//inspired by this webpage: https://docs.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=vs-2019

#include <iostream>
#include <iomanip>

//std::ostream& operator << (std::ostream& os, const double& in_value, const int& prec = 8);
class vodka
{
public:
  int x;
  
};
std::ostream& operator << (std::ostream& os, const double& in_value); 

int main()
{
  double pi = 3.14159265359;

  std::cout << "pi: " << pi << '\n';
  
  
  return 0;
}


//std::ostream& operator << (std::ostream& os, const double& in_value, const int& prec)
//{
//  os << std::fixed << std::setprecision(prec) << in_value;
//
//  return os;
//}

std::ostream& operator << (std::ostream& os, const double& in_value)
{
  os << std::fixed << std::setprecision(8) << in_value;

  return os;
}
