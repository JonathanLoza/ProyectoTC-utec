#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <iostream>
#include <vector>
#include <list>
using namespace std;
class Arista;

class Estado{
  int Nombre;
public:
  list<Arista*> aristas;
  Estado(int x){
    this-> Nombre=x;
  };

  int getnombre(){
    return Nombre;
  }
};

class Arista{
  int Entrada;
public:
  Estado* estados[2];

  int getentrada(){
    return Entrada;
  }

  Arista(Estado* estado1, Estado* estado2, int cadena){
    this->estados[0]=estado1;
    this->estados[1]=estado2;
    this->Entrada=cadena;
  };

};


class Automata {

  vector<Estado*> estados;
  int Nestados;
public:
  Automata(){
    this->Nestados=0;
  };

  void insertarestado(int x){
      Estado* temp=buscarestado(x);
      if(!temp){
        Estado* estado=new Estado(x);
        estados.push_back(estado);
        Nestados++;
      }else{
        cout<<"Estado ya existente"<<endl;
      }
  };

  void insertartransicion(int x, int y, int cadena){
    Estado* temp=buscarestado(x);
    Estado* temp2=buscarestado(y);
    if(!temp || !temp2){
						cout<<"no existe el estado"<<"\n";
		}
    else{
			for ( auto& item : temp->aristas )
			{
				if(item->estados[1]->getnombre()==y && item->getentrada()==cadena){
					cout<<"arista ya existente"<<endl;
					throw "arista ya existente";
				}
			}
			Arista* arista=new Arista(temp,temp2,cadena);
			temp->aristas.push_back(arista);
		}
  }

  Estado* buscarestado(int x){
    Estado* temp=nullptr;
    for(int i=0; i<estados.size(); i++){
      if(x==estados[i]->getnombre()){
        temp=estados[i];
        break;
      }
    }
    return temp;
  }

};


#endif
