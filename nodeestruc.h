#ifndef NODEESTRUC_H
#define NODEESTRUC_H

#include <iostream>

template <typename T>
struct Nodeestruc {
    T data;
    struct Nodeestruc* next;
    void killSelf(){
        if(next!=nullptr)
            next->killSelf();
        delete this;
    };
};

#endif
