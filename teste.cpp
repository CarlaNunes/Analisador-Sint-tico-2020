#include <iostream>
#include <fstream>
#include <string> 
#include <vector> 


int main ()
{
  std::string str ("T:st string :");
  for (unsigned i=0; i<str.length()-1; ++i)
  {
    std::cout << int(str.at(1))  ;
  }
  return 0;
}