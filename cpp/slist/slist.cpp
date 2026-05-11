#include <cassert>      //assert

#include "slist.hpp"

namespace hrd42
{
//nested hidden class
class Slist::Node
{
    public: //public to slist type
        //special public member funcitons
        explicit Node(int val_, Node* next_); //ctor
          
        //public member funciton (methods on object)
        //getters
        int GetData() const;
        Node* GetNext() const;

    private:
        Node(const Node& other);
        Node& operator=(const Node& other); 
        
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

















} // namespace hrd42
