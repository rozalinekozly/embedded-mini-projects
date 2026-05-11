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
        //setters
        void SetNext(Node* next_);
        //getters
        int GetData() const;
        Node* GetNext() const;

    private:
        Node(const Node& other);
        Node& operator=(const Node& other); 
        
        int m_val;
        Node* m_next;
};
} // namespace hrd42
