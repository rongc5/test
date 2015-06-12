#include <iostream>
#include <exception>
using namespace std;

int main () {
  try
  {
      throw std::string("hello world");
  }
  //catch (exception & e)
  //catch (std::string & str)
  {
    cout << "Standard exception: " << e.what() << endl;
    //cout << "Standard exception: " << endl;
    //cout << str.c_str() << endl;
  }
  return 0;
}
