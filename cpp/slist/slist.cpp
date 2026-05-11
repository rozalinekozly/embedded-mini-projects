#include <cassert>      //assert

#include "slist.hpp"

namespace hrd42
{

class Slist::Node
{
    public: 

        explicit Node(int val_ = 0, Node* const next_= 0); //ctor
        //generated cctor, copy assignment operaotr, dtor
          
        //getters 
        int GetData() const; //UB if node is nullptr
        Node* GetNext() const; //UB if node is nullptr

    private:
        
        int m_val;
        Node* m_next; 
};
/*---------------------------------------------------------------------------------*/
/*Node member functions implementaiton */
Slist::Node::Node(int val_, Node* next_)
{
    this->m_val = val_;
    this->m_next = next_;
}

int Slist::Node::GetData() const 
{
    return (this->m_val);
}

Slist::Node* Slist::Node::GetNext() const
{
    return (this->m_next);
}
/*---------------------------------------------------------------------------------*/
/* slist member functions implementations */
Slist::Slist()
{
    m_start = nullptr;
}

void Slist::PushFront(int val_)
{
    Slist::Node* node = new Node(val_, this->m_start);
    //updating head
    this->m_start = node;
} 

size_t Slist::Size() const
{
    size_t ret_counter = 0;
    Node* itr = this->m_start;

    while(NULL !=  itr)
    {
        ++ret_counter;
        itr = itr -> GetNext();
    }
    return ret_counter;
} 

bool Slist::IsEmpty() const
{
    return (nullptr == this->m_start);
}

int Slist::PopFront()
{
    assert(!this->IsEmpty());

    Node* node = this->m_start;
    int ret = node->GetData();
    this->m_start = node->GetNext();
    delete node;
    return ret;
}

Slist::~Slist()
{
    Node* itr = this->m_start;
    while (nullptr != itr)
    {
        Node* next = itr->GetNext();
        delete itr;
        itr = next;
    }
}














} // namespace hrd42
