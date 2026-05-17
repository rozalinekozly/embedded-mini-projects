/*------------------------------------------------------------------------------
Description: single-linked list class declaration, with nested node class hidden
Developer: rozaline 
Reviewer: steve
Version: 1
------------------------------------------------------------------------------*/    
#ifndef ILRD_RD42_SLIST_HPP
#define ILRD_RD42_SLIST_HPP

#include <cstddef> //size_t

namespace hrd42
{
    
class Slist //uncopiable
{
    public: 

        //special public member functions
        explicit Slist(); //default ctor, creates an empty list instance
        ~Slist(); //dtor

        //public member functions
        void PushFront(int val); 
        int PopFront(); //UB if slist is empty

        size_t Size() const; 
        bool IsEmpty() const;

    private: 

        class Node;   
        Node* m_start;  

        Slist(const Slist& other);
        Slist& operator=(const Slist& other);

};

}// namespace hrd42

#endif //ILRD_RD42_SLIST_HPP


