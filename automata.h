#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <iostream>
#include <vector>
#include <list>
#include <set>
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
  set<int> iniciales;
  set<int> finales;
  int Nestados;
public:
  Automata(){
    Nestados=0;
  };
  void read(){
    int inicial;
    int cantidadfinal;
    int final;
    int transicion;
    cout<<"ingresa"<<endl;
    cin >> Nestados >> inicial >> cantidadfinal;
    iniciales.insert(inicial);
    for(int i = 0; i < cantidadfinal; i ++){
      cin >> final;
      finales.insert(final);
    }
    cout<<"insertando estados"<<endl;
    for(int i=0; i<Nestados; i++){
      insertarestado(i);
    }
    cout<<"insertando transiciones"<<endl;
    for(int j = 0; j < 2*Nestados;j++){
      cout<<"escriba transicion "<<j<<endl;
      cin >> inicial >> transicion >> final;
      insertartransicion(inicial,final,transicion);
    }
    cout<<"acabo"<<endl;
    cout<<endl;

  }
  void insertarestado(int x){
      Estado* temp=buscarestado(x);
      if(!temp){
        Estado* estado=new Estado(x);
        estados.push_back(estado);
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

  Arista* buscararista(int x, int y){
    Estado* temp=buscarestado(x);
    Arista* ptrarista=nullptr;
    for(auto& item: temp->aristas){
      if(item->estados[1]->getnombre()==y){
        ptrarista=item;
        break;
      }
    }
    return ptrarista;
  }

  void print() {// Print del grafo
    cout<<Nestados<<" ";
    std::set<int>::iterator it=iniciales.begin();
    cout<<*it<<" ";
    cout<<finales.size()<<" ";
    for(std::set<int>::iterator it=finales.begin(); it!=finales.end(); ++it){
      cout<<*it<<" ";
    }
    cout<<endl;
  	for (int i = 0; i < estados.size(); i++) {
  		for ( auto& item : estados[i]->aristas )
  		{
  			cout<<item->estados[0]->getnombre()<<" ";
        cout<<item->getentrada()<<" ";
        cout<<item->estados[1]->getnombre()<<endl;
  		}
  	}
  };
  ~Automata(){
    for(int i=0; i<estados.size();i++){
      for(auto& item: estados[i]->aristas){
        delete item;
      }
      delete estados[i];
    }
  }
};


#endif
