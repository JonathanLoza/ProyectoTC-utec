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
};

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


class Automata {

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

  void Brzozowski(){
    reverse();
    reachdet();
    reverse();
    reachdet();
  }

  void reachdet(){
    int cont=0;
    bool existe=true;
    Arista* arista=nullptr;
    set<int> setestados;
    vector<Estado*> newestados;
    Estado* estado=new Estado(cont);
    cout<<"iniciales"<<endl;
    for(setit=iniciales.begin();setit!=iniciales.end();++setit){
      cout<<*setit<<endl;
      estado->insertprev(*setit);
    }
    iniciales.clear();
    iniciales.insert(cont);
    newestados.push_back(estado);
    cont++;
    std::vector<Estado*>::size_type size = newestados.size();
    for(int k=0;k<size; k++){
      cout<<"vive"<<endl;
      cout<<"Estado nombre "<<newestados[k]->getnombre()<<endl;
      cout<<"Para 0"<<endl;
      for(setit=newestados[k]->prevestados.begin();setit!=newestados[k]->prevestados.end();++setit){
        cout<<"Estados previos "<<*setit<<endl;
        estado=buscarestado(*setit);
        for(auto item: estado->aristas){
          if(item->getentrada()==0){
            cout<<"insertar a setestados "<<item->estados[1]->getnombre()<<endl;
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
        cout<<" Existe "<<endl;
        if(!estado){
          cout<<"no Existe "<<endl;
          estado=new Estado(cont);
          for(setit=setestados.begin();setit!=setestados.end();++setit){
            cout<<"insertandoprevios "<<*setit<<endl;
            estado->insertprev(*setit);
          }
          cont++;
          newestados.push_back(estado);
          cout<<"Insertar nuevo estado a newestados"<<endl;
          size++;
        }
        arista= new Arista(newestados[k],estado, 0);
        cout<<"Insertar arista"<<endl;
        newestados[k]->aristas.push_back(arista);
      }
      setestados.clear();
      cout<<"Para 1"<<endl;
      for(setit=newestados[k]->prevestados.begin();setit!=newestados[k]->prevestados.end();++setit){
        cout<<"Estados previos "<<*setit<<endl;
        estado=buscarestado(*setit);
        for(auto item: estado->aristas){
          if(item->getentrada()==1){
            cout<<"insertar a setestados "<<item->estados[1]->getnombre()<<endl;
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
        cout<<" Existe "<<endl;
        if(!estado){
          cout<<"no Existe "<<endl;
          estado=new Estado(cont);
          for(setit=setestados.begin();setit!=setestados.end();++setit){
            estado->insertprev(*setit);
          }
          cont++;
          newestados.push_back(estado);
          size++;
          cout<<"Insertar nuevo estado a newestados"<<endl;
        }
        arista= new Arista(newestados[k],estado, 1);
        cout<<"Insertar arista"<<endl;
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
    cout<<"Finales"<<endl;
    for(int i=0;i<newestados.size();i++){
      for(setit=finales.begin();setit!=finales.end();++setit){
        if(newestados[i]->prevestados.find(*setit) != newestados[i]->prevestados.end()){
          existe=true;
          break;
        }
      }
      if(existe){
        cout<<newestados[i]->getnombre()<<endl;
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
