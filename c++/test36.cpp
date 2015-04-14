#include <iostream>
#include <exception>

using namespace std;

class myexception: public exception
{
  virtual const char* what() const throw()
  {
    return "My exception happened";
  }
} myex;

int main () {
  try
  {
    //throw myex;
    //throw("hello");
    throw (1);
  }
  catch (exception& e)
  {
    cout << e.what() << '\n' <<  "error";
  }
  return 0;
}

