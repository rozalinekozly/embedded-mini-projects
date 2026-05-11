#ifndef HRD42_SLIST_HPP
#define HRD42_SLIST_HPP

#include <cstddef> //size_t

namespace hrd42
{
    
class Slist
{
    public: //public access modifier: accessible from anywhere 
            //inside, outside class or other files 

        //special public member functions
        explicit Slist(); //default ctor, creates an empty list instance
        ~Slist(); //dtor

        //public member functions
        void PushFront(int val_); 
        //UB if slist is empty
        int PopFront();
        size_t Size() const; 
        bool IsEmpty() const;

        //instruction to the compiler not to create these special functions by default
        Slist(const Slist& other_) = delete;
        Slist& operator=(const Slist& other_) = delete;

    private: //private access modifier: accessible only inside class itself 
            //(memeber methods) + hide data from outside 
        //private data members
        class Node;     //restricting that class's visisbility 
                        //exclusively to the containing class
        Node* m_start;  //start of the slist

};

}// namespace hrd42

#endif //HRD42_SLIST_HPP


