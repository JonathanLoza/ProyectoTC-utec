#include <iostream>
#include "automata.h"

using namespace std;

int main() {
  Automata automata;
  automata.read();
  //automata.matrizequivalencia();//Algoritmo 2
  //automata.Algoritmo3();//Algoritmo 2 optimizado
  //automata.Brzozowski();//hacer print para ver el nuevo automata
  //automata.Moore();
  automata.Hopcroft();
  cout<<endl;
  //automata.print();

  return 0;
}
