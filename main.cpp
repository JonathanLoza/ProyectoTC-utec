#include <iostream>
#include "automata.h"

using namespace std;

int main() {
  Automata automata;
  automata.read();
  //automata.print();
  cout<<endl;
  automata.reverse();
  automata.print();
  cout<<endl;
  automata.reachdet();
  automata.print();
  cout<<endl;
  automata.reverse();
  automata.print();
  cout<<endl;
  automata.reachdet();
  automata.print();
  /*automata.Brzozowski();
  automata.print();*/
  return 0;
}
