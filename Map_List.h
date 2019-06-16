//
// Created by orsht on 11/28/2018.
//

#include "list_node.h"


class Map_List{
    List_Node* first_node;
    int size;

public:

    Map_List();
    ~Map_List();
    int getSize();
    List_Node* getFirst();
    void setFirst(List_Node* node);
    void addNode(int* address);
    void deleteNode(List_Node* node);

};

    Map_List::Map_List():
            first_node(nullptr),
            size(0){}

    Map_List::~Map_List(){
        while(!first_node){
            List_Node* temp=first_node;
            first_node=first_node->getNext();
            delete temp;
        }
    }

    int Map_List:: getSize(){
        return size;
    }

    List_Node* Map_List::getFirst(){
        return first_node;
    }

    void Map_List::addNode(int* address){
        List_Node* new_node = List_Node(address);
        new_node->setNext(getFirst());
        if(first_node) {
            first_node->setPrevious(new_node);
        }
        first_node=new_node;
        ++size;
    }

void Map_List:: deleteNode(List_Node* node){
    List_Node* prev = node->getPrevious();
    List_Node* next = node->getNext();
    if(!prev){
        setFirst(node->getNext());
        if(next){
            (getFirst())->setPrevious(nullptr);
        }
    }
    else if(!next){
        prev->setNext(nullptr);

    } else{
        next->setPrevious(prev);
        prev->setNext(next);
    }
    delete(node);
    --size;
};

    void Map_List:: setFirst(List_Node* node){
        first_node=node;
    }
