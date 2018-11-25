#include "iostream"
#include "algorithm"
#include "set"

using namespace std;

int main(){


  set<int> a;
  for(int i=0;i<10;i++){
    a.insert(i);
  }

  set<int>::iterator ite;
  int j=50;
  for(ite=a.begin();ite!=a.end();){
    if((*ite)%2==0){
      cout<<*ite<<endl;
      ite=
      a.erase(ite++);
    }
    if((*ite)%3==0){
      a.insert(j);
      j++;
      ++ite;
    }
    //cout<<*ite<<endl;
  }
  for(auto item:a){
    cout<<item<<" ";
  }



  return 0;
}
