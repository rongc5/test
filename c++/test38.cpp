#include <iostream>
#include <exception>
using namespace std;

int main () {
  try
  {
      //throw std::string("hello world");
      1/0;
  }
  //catch (exception & e)
  catch (...)
  {
    cout << "Standard exception: " << endl;
    //cout << "Standard exception: " << endl;
    //cout << str.c_str() << endl;
  }
  return 0;
}
