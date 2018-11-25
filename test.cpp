#include "iostream"
#include "algorithm"
#include "set"
#include "vector"

using namespace std;

int main(){

set<set<int>> x;
set<int> a;
set<int> b;
set<int> c;
set<int> e;

a.insert(1);
b.insert(10);
b.insert(1);
x.insert(b);
x.insert(a);
c.insert(1);
c.insert(2);
cout<<x.size();

if(x.find(c)!=x.end())
  cout<<"true"<<endl;
else
  cout<<"false"<<endl;

if(e.begin()==e.end())
  cout<<"bica"<<endl;
cout<<e.size();
  return 0;
}
