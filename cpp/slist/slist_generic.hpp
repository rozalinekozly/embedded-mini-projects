/*------------------------------------------------------------------------------
Description: single-linked list class declaration, with nested node class hidden generic
Developer: rozaline 
Reviewer: steve
Version: 1
------------------------------------------------------------------------------*/ 
#ifndef ILRD_RD42_SLIST_HPP
#define ILRD_RD42_SLIST_HPP

#include <cstddef> //size_t 
#include <cassert> //assert

namespace hrd42
{
template <typename T>
class Slist
{
    public:

        explicit Slist();
        ~Slist();

        //public member funcitons
        void PushFront(const T val);
        T PopFront(); //UB if slist is empty

        size_t Size() const;
        bool IsEmpty() const;

    private:
        class Node
        {
            public: 
               
                explicit Node(const T val_ = 0 , Node* const next_ = nullptr); //ctor
                //generated cctor, copy assignment operator, dtor
                T GetData() const; //UB if node is nullptr
                Node* GetNext() const; //UB if node is nullptr

            private:

                T m_val;
                Node* m_next;
        };
        Node* m_start;
        Slist(const Slist& other);
        Slist& operator=(const Slist& other);
};
/*--------------------------------------------------------------------------------------------*/
/* node implementation */
template <typename T>
Slist<T>::Node::Node(const T val_, Node* const next_)
{
    this->m_val = val_;
    this->m_next = next_;
}

template <typename T>
T Slist<T>::Node::GetData() const
{
    return (this->m_val);   
}

template <typename T>
typename Slist<T>::Node* Slist<T>::Node::GetNext() const
{
    return (this->m_next);
}
/*--------------------------------------------------------------------------------------------*/
/* slist implementation */
template <typename T>
Slist<T>::Slist()
{
    m_start = nullptr;  
}

template <typename T>
void Slist<T>::PushFront(const T val_)
{
    Slist<T>::Node* node = new Node(val_, this->m_start);
    this->m_start = node;   
}

template <typename T>
size_t Slist<T>::Size() const
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

template <typename T>
bool Slist<T>::IsEmpty() const
{
    return (nullptr == this->m_start);
}

template <typename T>
T Slist<T>::PopFront()
{
    assert(!this->IsEmpty());
    Node* node = this->m_start;
    T ret = node->GetData();
    this->m_start = node->GetNext();
    delete node;
    return ret;
}

template <typename T>
Slist<T>::~Slist()
{
    Node* itr = this->m_start;
    while (nullptr != itr)
    {
        Node* next = itr->GetNext();
        delete itr;
        itr = next;
    }
}
}//end of namespace hrd42

#endif //ILRD_RD42_SLIST_HPP