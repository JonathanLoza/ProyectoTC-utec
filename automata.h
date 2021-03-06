#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include "queue.h"
using namespace std;
class Estado;


class Arista{
  int Entrada;
public:
  Estado* estados[2];

  int getentrada(){
    return Entrada;
  }
  void invertir(){
    auto temp=estados[0];
    estados[0]=estados[1];
    estados[1]=temp;
  }

  Arista(Estado* estado1, Estado* estado2, int cadena){
    this->estados[0]=estado1;
    this->estados[1]=estado2;
    this->Entrada=cadena;
  };

};
class Estado{
  int Nombre;
public:
  list<Arista*> aristas;
  set<int> prevestados;
  Estado(int x){
    this-> Nombre=x;
  };

  int getnombre(){
    return Nombre;
  }
  void insertprev(int n){
    prevestados.insert(n);
  }
  int getSalida(int x){
    for(auto item: this->aristas){
      if(item->getentrada()==x){
        return item->estados[1]->getnombre();
      }
    }
  }
};

struct Par{

  Estado* pares[2];
  std::vector<Par*> parestados;
  Par(Estado* estado1, Estado* estado2){
    pares[0]=estado1;
    pares[1]=estado2;
  }
  bool parcorrecto(Estado* estado1, Estado* estado2){
    if(estado1==pares[0] && estado2==pares[1])
      return true;
    else if(estado1==pares[1] && estado2==pares[0])
      return true;
    else
      return false;
  }
};
class Automata {

  vector<Par*> vectorpares;
  map<int,Estado*> mapestados;
  vector<Estado*> estados;
  set<int> iniciales;
  set<int> finales;
  int Nestados;
public:
  set<int>::iterator setit;
  vector<Estado*>::iterator vectite;
  Automata(){
    Nestados=0;
  };


  void Hopcroft(){
    set<set<int>> P;
    set<set<int>> W;
    set<int> allestados;
    for(int i=0; i<Nestados;i++){
      allestados.insert(estados[i]->getnombre());
    }
    set<int> ends=finales;
    set<int> noends;
    set<int> intersect;
    set<int> difference;
    set_difference(allestados.begin(), allestados.end(), ends.begin(), ends.end(),inserter(noends, noends.end()));
    P.insert(ends);
    P.insert(noends);
    W.insert(ends);
    set<set<int>>::iterator iteW;
    set<set<int>>::iterator iteP;
    set<int> X;
    set<int> A;
    set<set<int>> temporal;
    while(!W.empty()){
      iteW=W.begin();
      A=(*iteW);
      W.erase(iteW);
      for(int c=0;c<2;c++){
        for(int i=0;i<Nestados;i++){
          for(auto item:A){
            if(NextEstadowithX(i,c)->getnombre()==item)
              X.insert(i);
          }
        }
        iteP=P.begin();
        while(iteP!=P.end()){
          set_difference((*iteP).begin(), (*iteP).end(), X.begin(), X.end(),inserter(difference, difference.end()));
          set_intersection(X.begin(),X.end(),(*iteP).begin(),(*iteP).end(),inserter(intersect,intersect.begin()));
          if(!intersect.empty() && !difference.empty()){
            set<int> temp=*iteP;
            P.erase(iteP);
            P.insert(difference);
            P.insert(intersect);
            if(W.find(temp)!=W.end()){
              W.erase(temp);
              W.insert(difference);
              W.insert(intersect);
            }
            else{
              if(intersect.size()<=difference.size()){
                W.insert(intersect);
              }
              else{
                W.insert(difference);
              }
            }
          }
          else{
            temporal.insert(*iteP);
            P.erase(*iteP);
          }
          iteP=P.begin();
          difference.clear();
          intersect.clear();
        }
        P=temporal;
        temporal.clear();
        X.clear();
      }
    }
    Estado* nuevo;
    vector<Estado*> newestados;
    int count=0;
    for(auto item:P){
      nuevo=new Estado(count);
      newestados.push_back(nuevo);
      nuevo->prevestados=item;
      count++;
    }
    Arista* arista0;
    Arista* arista1;
    Arista* nueva;
    bool cero=false;
    bool uno=false;
    for(int i=0;i<newestados.size();i++){
      setit=newestados[i]->prevestados.begin();
      for(auto item: mapestados[(*setit)]->aristas){
        if(item->getentrada()==0)
          arista0=item;
        if(item->getentrada()==1)
          arista1=item;
      }
      for(int j=0;j<newestados.size();j++){
        if(!cero && newestados[j]->prevestados.find(arista0->estados[1]->getnombre())!=newestados[j]->prevestados.end()){
          nueva=new Arista(newestados[i],newestados[j],0);
          newestados[i]->aristas.push_back(nueva);
          cero=true;
        }
        if(!uno && newestados[j]->prevestados.find(arista1->estados[1]->getnombre())!=newestados[j]->prevestados.end()){
          nueva=new Arista(newestados[i],newestados[j],1);
          newestados[i]->aristas.push_back(nueva);
          uno=true;
        }
        if(cero && uno){
          break;
        }
        cero=false;
        uno=false;
      }

    }
    bool esfinal=false;
    bool esinicial=false;
    set<int> newfinal;
    set<int> newinicial;
    for(int i=0;i<newestados.size();i++){
      for(setit=finales.begin();setit!=finales.end();++setit){
        if(newestados[i]->prevestados.find(*setit) != newestados[i]->prevestados.end()){
          esfinal=true;
          break;
        }
      }
      if(esfinal){
        newfinal.insert(newestados[i]->getnombre());
      }
      for(setit=iniciales.begin();setit!=iniciales.end();++setit){
        if(newestados[i]->prevestados.find(*setit) != newestados[i]->prevestados.end()){
          esinicial=true;
          break;
        }
      }
      if(esinicial){
        newinicial.insert(newestados[i]->getnombre());
      }
      esinicial=false;
      esfinal=false;
    }
    iniciales.clear();
    finales.clear();
    finales=newfinal;
    iniciales=newinicial;
    estados.clear();
    estados=newestados;
    Nestados=newestados.size();


  }

  void Moore(){
    int** equimatrix=Algoritmo3();
    map<Estado*,bool> visitados;
    for(int i=0; i<Nestados;i++){
      visitados.insert(pair<Estado*,bool> (estados[i],false));
    }

    int count=0;
    Estado* newEstado;
    vector<Estado*> newestados;

    for(int i=0;i<Nestados;i++){
      if(visitados[estados[i]]==false){
        newEstado=new Estado(count);
        newestados.push_back(newEstado);
        count++;
        visitados[estados[i]]=true;
        newEstado->prevestados.insert(estados[i]->getnombre());
        for(int j=0;j<Nestados;j++){
          if(i!=j && equimatrix[i][j]==1){
            newEstado->prevestados.insert(estados[j]->getnombre());
            visitados[estados[j]]=true;
          }
        }
      }
    }

    for(int i=0;i<newestados.size();i++){
      cout<<newestados[i]->getnombre()<<" : ";
      for(auto item: newestados[i]->prevestados){
        cout<<item<<" ";
      }
      cout<<endl;
    }
    Arista* arista0;
    Arista* arista1;
    Arista* nueva;
    bool cero=false;
    bool uno=false;
    for(int i=0;i<newestados.size();i++){
      setit=newestados[i]->prevestados.begin();
      for(auto item: mapestados[(*setit)]->aristas){
        if(item->getentrada()==0)
          arista0=item;
        if(item->getentrada()==1)
          arista1=item;
      }
      for(int j=0;j<newestados.size();j++){
        if(!cero && newestados[j]->prevestados.find(arista0->estados[1]->getnombre())!=newestados[j]->prevestados.end()){
          nueva=new Arista(newestados[i],newestados[j],0);
          newestados[i]->aristas.push_back(nueva);
          cero=true;
        }
        if(!uno && newestados[j]->prevestados.find(arista1->estados[1]->getnombre())!=newestados[j]->prevestados.end()){
          nueva=new Arista(newestados[i],newestados[j],1);
          newestados[i]->aristas.push_back(nueva);
          uno=true;
        }
        if(cero && uno){
          break;
        }
        cero=false;
        uno=false;
      }

    }
    /*for(int i=0;i<newestados.size();i++){
      cout<<newestados[i]->getnombre()<<" : ";
      for(auto item: newestados[i]->aristas){
        cout<<item->estados[1]->getnombre()<<":"<<item->getentrada()<<" ";
      }
      cout<<endl;
    }*/

    bool esfinal=false;
    bool esinicial=false;
    set<int> newfinal;
    set<int> newinicial;
    for(int i=0;i<newestados.size();i++){
      for(setit=finales.begin();setit!=finales.end();++setit){
        if(newestados[i]->prevestados.find(*setit) != newestados[i]->prevestados.end()){
          esfinal=true;
          break;
        }
      }
      if(esfinal){
        newfinal.insert(newestados[i]->getnombre());
      }
      for(setit=iniciales.begin();setit!=iniciales.end();++setit){
        if(newestados[i]->prevestados.find(*setit) != newestados[i]->prevestados.end()){
          esinicial=true;
          break;
        }
      }
      if(esinicial){
        newinicial.insert(newestados[i]->getnombre());
      }
      esinicial=false;
      esfinal=false;
    }
    iniciales.clear();
    finales.clear();
    finales=newfinal;
    iniciales=newinicial;
    estados.clear();
    estados=newestados;
    Nestados=newestados.size();

  }

  bool distinguible(Estado* nodo1,Estado* nodo2){
    if(nodo1 == nodo2){return false;}
    Estado* nodo1_salida0;
    Estado* nodo1_salida1;
    Estado* nodo2_salida0;
    Estado* nodo2_salida1;
    for(auto item: nodo1->aristas){
      if(item->getentrada()==0){
        nodo1_salida0=item->estados[1];
      }
      else if(item->getentrada()==1){
        nodo1_salida1= item->estados[1];
      }
    }
    for(auto item: nodo2->aristas){
      if(item->getentrada()==0){
        nodo2_salida0=item->estados[1];
      }
      else if(item->getentrada()==1){
        nodo2_salida1=item->estados[1];
      }
    }
    if((finales.find(nodo1_salida0->getnombre())!=finales.end())&&(finales.find(nodo2_salida0->getnombre())==finales.end())){
      return true;
    }
    else if((finales.find(nodo1_salida0->getnombre())==finales.end())&&(finales.find(nodo2_salida0->getnombre())!= finales.end())){
      return true;
    }
    else if((finales.find(nodo1_salida1->getnombre())!=finales.end())&&(finales.find(nodo2_salida1->getnombre())== finales.end())){
      return true;
    }
    else if((finales.find(nodo1_salida1->getnombre())==finales.end())&&(finales.find(nodo2_salida1->getnombre())!= finales.end())){
      return true;
    }
    else{
      if(distinguible(nodo1_salida0,nodo2_salida0))
        return true;
      else if(distinguible(nodo1_salida1,nodo2_salida1))
        return true;
      else
        return false;
    }
  }

  int** matrizequivalencia(){
    int **equimatrix=new int*[Nestados];
    for(int x=0;x<Nestados;x++){
      equimatrix[x]=new int[Nestados];
    }
    for(int i = 0; i < Nestados; i++){
      for(int j = 0; j < Nestados; j++){
        equimatrix[i][j] = 1;
      }
    }

    for(int i = 0; i < Nestados; i++){
      Estado* nodo1 = estados[i];
      for(int j = 0; j < Nestados; j++){
        Estado* nodo2 = estados[j];
        if(nodo1 != nodo2 && equimatrix[nodo1->getnombre()][nodo2->getnombre()] == 1){
          if((finales.find(nodo1->getnombre())!=finales.end())&&(finales.find(nodo2->getnombre())==finales.end())){
              equimatrix[nodo1->getnombre()][nodo2->getnombre()] = 0;
              equimatrix[nodo2->getnombre()][nodo1->getnombre()] = 0;
          }
          else if((finales.find(nodo2->getnombre())!=finales.end())&&(finales.find(nodo1->getnombre())==finales.end())){
              equimatrix[nodo1->getnombre()][nodo2->getnombre()] = 0;
              equimatrix[nodo2->getnombre()][nodo1->getnombre()] = 0;
          }
          else if(distinguible(nodo1,nodo2)){
            equimatrix[nodo1->getnombre()][nodo2->getnombre()] = 0;
            equimatrix[nodo2->getnombre()][nodo1->getnombre()] = 0;
          }
        }
      }
    }
    for(int z = 0; z < Nestados; z++){
      for(int y = 0; y < Nestados; y++){
          cout << equimatrix[z][y] << " ";
      }
      cout << endl;
    }
    return equimatrix;
  }

  int** Algoritmo3(){

    for(int i=0; i<Nestados;i++){
      auto estado1=mapestados[i];
      for(int j=i+1;j<Nestados;j++){
        auto estado2=mapestados[j];
        Par* par=new Par(estado1,estado2);
        vectorpares.push_back(par);
      }
    }

    Arista* arista00;
    Arista* arista01;
    Arista* arista10;
    Arista* arista11;
    Par* parsiguiente;
    for(int i=0; i<vectorpares.size();i++){
        for(auto item: vectorpares[i]->pares[0]->aristas){
          if(item->getentrada()==0)
            arista00=item;
          if(item->getentrada()==1)
            arista10=item;
        }
        for(auto item: vectorpares[i]->pares[1]->aristas){
          if(item->getentrada()==0)
            arista01=item;
          if(item->getentrada()==1)
            arista11=item;
        }
        for(int j=0;j<vectorpares.size();j++){
          if(vectorpares[j]->parcorrecto(arista00->estados[1],arista01->estados[1])){
            parsiguiente=vectorpares[i];
            vectorpares[j]->parestados.push_back(parsiguiente);
          }
          if(vectorpares[j]->parcorrecto(arista10->estados[1],arista11->estados[1])){
            parsiguiente=vectorpares[i];
            vectorpares[j]->parestados.push_back(parsiguiente);
          }
        }
    }
    int **equimatrix=new int*[Nestados];
    for(int x=0;x<Nestados;x++){
      equimatrix[x]=new int[Nestados];
    }
    for(int i = 0; i < Nestados; i++){
      for(int j = 0; j < Nestados; j++){
        equimatrix[i][j] = 1;
      }
    }
    Queue<Par*> cola;
    for(int k=0; k<vectorpares.size();k++){
      if(((finales.find(vectorpares[k]->pares[0]->getnombre())!=finales.end())&&(finales.find(vectorpares[k]->pares[1]->getnombre())==finales.end())) || ((finales.find(vectorpares[k]->pares[1]->getnombre())!=finales.end())&&(finales.find(vectorpares[k]->pares[0]->getnombre())==finales.end())) ){
        equimatrix[vectorpares[k]->pares[0]->getnombre()][vectorpares[k]->pares[1]->getnombre()]=0;
        equimatrix[vectorpares[k]->pares[1]->getnombre()][vectorpares[k]->pares[0]->getnombre()]=0;
        for(auto item: vectorpares[k]->parestados){
          cola.push(item);
        }
      }
    }
    while(!cola.empty()){
      parsiguiente=cola.get();
      cola.pop();
      if(equimatrix[parsiguiente->pares[0]->getnombre()][parsiguiente->pares[1]->getnombre()]!=0){
        equimatrix[parsiguiente->pares[0]->getnombre()][parsiguiente->pares[1]->getnombre()]=0;
        equimatrix[parsiguiente->pares[1]->getnombre()][parsiguiente->pares[0]->getnombre()]=0;
        for(auto item: parsiguiente->parestados){
          cola.push(item);
        }
      }
    }
    for(int z = 0; z < Nestados; z++){
      for(int y = 0; y < Nestados; y++){
          cout << equimatrix[z][y] << " ";
      }
      cout << endl;
    }

    return equimatrix;
  }

  void Brzozowski(){
    reverse();
    reachdet();
    reverse();
    reachdet();
    for(auto item:estados){
      item->prevestados.clear();
    }
  }

  void reachdet(){
    int cont=0;
    bool existe=true;
    Arista* arista=nullptr;
    set<int> setestados;
    vector<Estado*> newestados;
    Estado* estado=new Estado(cont);
    for(setit=iniciales.begin();setit!=iniciales.end();++setit){
      estado->insertprev(*setit);
    }
    iniciales.clear();
    iniciales.insert(cont);
    newestados.push_back(estado);
    cont++;
    std::vector<Estado*>::size_type size = newestados.size();
    for(int k=0;k<size; k++){
      for(setit=newestados[k]->prevestados.begin();setit!=newestados[k]->prevestados.end();++setit){
        estado=buscarestado(*setit);
        for(auto item: estado->aristas){
          if(item->getentrada()==0){
            setestados.insert(item->estados[1]->getnombre());
          }
        }
      }
      if(!setestados.empty()){
        existe=true;
        estado=nullptr;
        for(int i=0; i<newestados.size();i++){
          for(setit=setestados.begin();setit!=setestados.end();++setit){
            if(newestados[i]->prevestados.find(*setit) == newestados[i]->prevestados.end()){
              existe=false;
              break;
            }
          }
          if(existe && setestados.size()==newestados[i]->prevestados.size()){
            estado=newestados[i];
            break;
          }
          existe=true;
        }
        if(!estado){
          estado=new Estado(cont);
          for(setit=setestados.begin();setit!=setestados.end();++setit){
            estado->insertprev(*setit);
          }
          cont++;
          newestados.push_back(estado);
          size++;
        }
        arista= new Arista(newestados[k],estado, 0);
        newestados[k]->aristas.push_back(arista);
      }
      setestados.clear();
      for(setit=newestados[k]->prevestados.begin();setit!=newestados[k]->prevestados.end();++setit){
        estado=buscarestado(*setit);
        for(auto item: estado->aristas){
          if(item->getentrada()==1){
            setestados.insert(item->estados[1]->getnombre());
          }
        }
      }
      if(!setestados.empty()){
        existe=true;
        estado=nullptr;
        for(int i=0; i<newestados.size();i++){
          for(setit=setestados.begin();setit!=setestados.end();++setit){
            if(newestados[i]->prevestados.find(*setit) == newestados[i]->prevestados.end()){
              existe=false;
              break;
            }
          }
          if(existe && setestados.size()==newestados[i]->prevestados.size()){
            estado=newestados[i];
            break;
          }
          existe=true;
        }
        if(!estado){
          estado=new Estado(cont);
          for(setit=setestados.begin();setit!=setestados.end();++setit){
            estado->insertprev(*setit);
          }
          cont++;
          newestados.push_back(estado);
          size++;
        }
        arista= new Arista(newestados[k],estado, 1);
        newestados[k]->aristas.push_back(arista);

      }
      setestados.clear();
    }
    estado=new Estado(cont);
    arista=new Arista(estado, estado,0);
    estado->aristas.push_back(arista);
    arista=new Arista(estado, estado,1);
    estado->aristas.push_back(arista);
    bool usado=false;
    bool cero=false;
    bool uno=false;
    for(int i=0; i<newestados.size();i++){
      for(auto item:newestados[i]->aristas){
        if(item->getentrada()==1)
          uno=true;
        if(item->getentrada()==0)
          cero=true;
      }
      if(!uno){
        arista=new Arista(newestados[i],estado,1);
        newestados[i]->aristas.push_back(arista);
        usado=true;
      }
      if(!cero){
        arista=new Arista(newestados[i],estado,0);
        newestados[i]->aristas.push_back(arista);
        usado=true;
      }
      uno=cero=false;
    }
    if(!usado){
      for(auto item:estado->aristas){
        delete item;
      }
      delete estado;
    }
    else{
      newestados.push_back(estado);
    }
    existe=false;
    set<int> newfinal;
    for(int i=0;i<newestados.size();i++){
      for(setit=finales.begin();setit!=finales.end();++setit){
        if(newestados[i]->prevestados.find(*setit) != newestados[i]->prevestados.end()){
          existe=true;
          break;
        }
      }
      if(existe){
        newfinal.insert(newestados[i]->getnombre());
      }
      existe=false;
    }
    finales.clear();
    finales=newfinal;
    estados.clear();
    estados=newestados;
    Nestados=newestados.size();

  }

  void read(){
    int inicial;
    int cantidadfinal;
    int final;
    int transicion;
    int numeroestados;
    cout<<"ingresa"<<endl;
    cin >> numeroestados >> inicial >> cantidadfinal;
    iniciales.insert(inicial);
    for(int i = 0; i < cantidadfinal; i ++){
      cin >> final;
      finales.insert(final);
    }
    for(int i=0; i<numeroestados; i++){
      insertarestado(i);
    }
    for(int j = 0; j < 2*Nestados;j++){
      cin >> inicial >> transicion >> final;
      insertartransicion(inicial,final,transicion);
    }
    cout<<endl;

  }
  void insertarestado(int x){
      Estado* temp=buscarestado(x);
      if(!temp){
        Estado* estado=new Estado(x);
        estados.push_back(estado);
        mapestados.insert(pair<int,Estado*>(x,estado));
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
  void reverse(){
    list<Arista*> newaristas;
    for(int i=0; i<estados.size();i++){
      for(auto item: estados[i]->aristas){
        item->invertir();
        newaristas.push_back(item);
      }
      estados[i]->aristas.clear();
    }
    Estado* estado;
    for(auto item: newaristas){
      estado=buscarestado(item->estados[0]->getnombre());
      estado->aristas.push_back(item);
    }
    set<int> temp=iniciales;
    iniciales=finales;
    finales=temp;
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

  Estado* NextEstadowithX(int x, int transicion){
    for(auto item:mapestados[x]->aristas){
      if(item->getentrada()==transicion)
        return item->estados[1];
    }
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

  Arista* buscartransicionX(int x, int transicion){
    for(auto item:mapestados[x]->aristas){
      if(item->getentrada()==transicion)
        return item;
    }
  }

  void clearautomata(){
    for(int i=0; i<estados.size();i++){
      for(auto& item: estados[i]->aristas){
        delete item;
      }
      delete estados[i];
    }
    Nestados=0;
    estados.clear();
    iniciales.clear();
    finales.clear();
  }

  void print() {// Print del grafo
    cout<<Nestados<<" ";
    std::set<int>::iterator it=iniciales.begin();
    for(it;it!=iniciales.end();++it){
      cout<<*it<<" ";
    }
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
