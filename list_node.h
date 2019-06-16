//
// Created by orsht on 11/28/2018.
//

class List_Node{
    List_Node* next;
    List_Node* previous;
    int* address;
public:
    List_Node(int* address);
    ~List_Node()= default;
    int* getAddress();
    List_Node* getNext();
    List_Node* getPrevious();
    void setNext(List_Node* ln);
    void setPrevious(List_Node* ln);
};

List_Node::List_Node(int* address):
        next(nullptr),
        previous(nullptr),
        address(address){}

int* List_Node:: getAddress(){
    return address;
}

List_Node* List_Node::getNext(){
    return next;
}

List_Node* List_Node:: getPrevious(){
    return previous;
}

void List_Node::setNext(List_Node* ln){
    next=ln;
}

void List_Node::setPrevious(List_Node* ln){
    previous=ln;

}

