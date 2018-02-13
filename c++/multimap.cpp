#include<iostream>
#include<map>
#include <string>

using namespace std;

//forward declaration
void printer(multimap<string, int> pN);

int main()
{
  multimap<string, int> phoneNums;

  //Insert key, value as pairs
  phoneNums.insert(pair<string, int>("Joe",123));
  phoneNums.insert(pair<string, int>("Will",444));
  printer(phoneNums);

  //Insert duplicates
  phoneNums.insert(pair<string, int>("Joe",369));
  phoneNums.insert(pair<string, int>("Smith",567));
  phoneNums.insert(pair<string, int>("Joe",888));
  phoneNums.insert(pair<string, int>("Will",999));
  printer(phoneNums);

  //Checking frequency of different keys
  cout<<"\n\nFrequency of different names"<<endl;
  cout<<"Number of Phones for Joe = "<<phoneNums.count("Joe")<<endl;
  cout<<"Number of Phones for Will = "<<phoneNums.count("Will")<<endl;
  cout<<"Number of Phones for Smith = "<<phoneNums.count("Smith")<<endl;
  cout<<"Number of Phones for Zahid = "<<phoneNums.count("Zahid")<<endl;

  //Print all Joe from the list and then erase them
  pair<multimap<string,int>::iterator, multimap<string,int>::iterator> ii;
  multimap<string, int>::iterator it; //Iterator to be used along with ii
  ii = phoneNums.equal_range("Joe"); //We get the first and last entry in ii;
  cout<<"\n\nPrinting all Joe and then erasing them"<<endl;
  for(it = ii.first; it != ii.second; ++it)
  {
    cout<<"Key = "<<it->first<<"    Value = "<<it->second<<endl;
  }
  phoneNums.erase(ii.first, ii.second);
  printer(phoneNums);

  return 0;
}

//This method prints the vector
void printer(multimap<string, int> pN)
{
  cout<<"\n\nMultimap printer method"<<endl;
  cout<<"Map size = "<<pN.size()<<endl;
  multimap<string, int>::iterator it = pN.begin();
  while(it != pN.end())
  {
    cout<<"Key = "<<it->first<<"    Value = "<<it->second<<endl;
    it++;
  }
}
