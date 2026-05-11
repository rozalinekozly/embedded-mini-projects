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

















} // namespace hrd42
