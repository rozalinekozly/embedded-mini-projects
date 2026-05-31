#include <iostream>
using namespace std;
	
class Public_Transport
{
public:
	Public_Transport() : m_license_plate(++s_count) //ctor
	{ 
		cout << "Public_Transport::Ctor()" << m_license_plate << endl; 
	}

	virtual ~Public_Transport() //dest
	{ 
		--s_count; 
		cout << "Public_Transport::Dtor()" << m_license_plate  << endl; 
	}

	Public_Transport(const Public_Transport &other) : m_license_plate(++s_count) //cctor
	{ 
		cout << "Public_Transport::CCtor() " << m_license_plate << endl; 
	}

	virtual void Display() //virtual 
	{ 
		cout << "Public_Transport::Display(): " << m_license_plate << endl; 
	}

	static void PrintCount() { cout << "s_count: " << s_count << endl;} //virtual
protected:
	int GetID() { return m_license_plate; }

private:
	static int s_count;
	int m_license_plate;
	Public_Transport &operator=(const Public_Transport &); // disabled
};

int Public_Transport::s_count = 0;

class Minibus: public Public_Transport
{
public:
	Minibus()
	: m_numSeats(20)
	{ 
		cout << "Minibus::Ctor()" << endl; 
	}
	Minibus(const Minibus &other )
	: Public_Transport(other),
	  m_numSeats(other.m_numSeats)
	{ 
		cout << "Minibus::CCtor()" << endl; 
	}
   ~Minibus() { cout << "Minibus::dtor()" << endl; }
	void Display() 
	{ 
		cout << "Minibus::Display() ID:"<<GetID();
		cout << " num seats:" << m_numSeats << endl; 
	}
	virtual void Wash() { cout << "Minibus::Wash() ID:"<<GetID() << endl; }

private:
	int m_numSeats;
};

class Taxi: public Public_Transport
{
public:
	Taxi() { cout << "Taxi::Ctor()" << endl; }
	Taxi(const Taxi &other ): Public_Transport(other) { cout << "Taxi::CCtor()" << endl; }
   ~Taxi() { cout << "Taxi::dtor()" << endl; }
	void Display() { cout << "Taxi::Display() ID:"<<GetID()<< endl; }

private:
};

template<class T>
inline T maxFunc(const T &t1, const T &t2)
{
	return ((t1 > t2)? t1: t2);
}

class Special_Taxi : public Taxi
{
public:
	Special_Taxi() { cout << "Special_Taxi::Ctor()" << endl; }
	Special_Taxi(const Special_Taxi &other): Taxi(other) 
	{ 
		cout << "Special_Taxi::CCtor()" << endl; 
	}
   ~Special_Taxi() { cout << "Special_Taxi::dtor()" << endl; }
	void Display() { cout << "Special_Taxi::Display() ID:"<<GetID()<<endl;}
private:
}; 


class Public_Convoy: public Public_Transport
{
public:
	Public_Convoy() 
	: m_pt1(new Minibus()), m_pt2(new Taxi())
	{  }

    // using intializing list for taxi and minibus bc relevant operator = is blocked
   Public_Convoy(const Public_Convoy& other)
        : Public_Transport(other), 
          m_pt1 (new Minibus(static_cast<const Minibus&>(*other.m_pt1))),
          m_pt2 (new Taxi(static_cast<const Taxi&>(*other.m_pt2))),
          m_m(other.m_m), 
          m_t(other.m_t)  
    {
        cout << "Public_Convoy::CCtor()" << endl;
    }
	~Public_Convoy() 
	{ 
		delete m_pt1; 
		delete m_pt2;
	}

	void Display()
	{
		m_pt1->Display();
		m_pt2->Display();
		m_m.Display();
		m_t.Display();
	}

private:
  Public_Transport *m_pt1;
  Public_Transport *m_pt2;
  Minibus m_m;
  Taxi m_t;
};

void PrintInfo(Public_Transport &a) { a.Display(); }
void PrintInfo() { Public_Transport::PrintCount(); }

Public_Transport PrintInfo(int i)
{
	Minibus ret;
	cout<<"PrintInfo(int i)"<<endl;
	ret.Display();
	return ret;
}

void TaxiDisplay(Taxi s) { s.Display(); }

int main(int argc, char **argv, char **envp)
{
	Minibus m;
	PrintInfo(m);
	PrintInfo(3).Display();


    
	Public_Transport *array[] = 
	{
		new Minibus(),
		new Taxi(),
		new Minibus()
	};

std::cout<<"---------------------------"<<endl;

	for(int i = 0; i < 3; ++i) array[i]->Display();
	for(int i = 0; i < 3; ++i) delete array[i];
 std::cout<<"---------------------------"<<endl;

Public_Transport arr2[] =
	{
		Minibus(),
		Taxi(),
		Public_Transport()
	};
 std::cout<<"---------------------------"<<endl;

	for(int i = 0; i < 3; ++i) 
	{
        arr2[i].Display();
    }
    std::cout<<"---------------------------"<<endl;
    
    Public_Transport::PrintCount();
    Minibus m2;
    m2.PrintCount();
    std::cout<<"---------------------------"<<endl;
 
 
	Minibus arr3[4];
    std::cout<<"---------------------------"<<endl;
  
	Taxi *arr4 = new Taxi[4];
	delete [] arr4;
    std::cout<<"---------------------------"<<endl;


    std::cout<<"print the max"<<endl;
	std::cout<<maxFunc<int>(1,2)<<std::endl;
	Special_Taxi st;
	TaxiDisplay(st);
std::cout<<"---------------------------"<<endl;



	Public_Convoy *ts1 = new Public_Convoy(); 
	Public_Convoy *ts2 = new Public_Convoy(*ts1); 
std::cout<<"---------------------------"<<endl;

	ts1->Display();
	ts2->Display();
	delete ts1;

	ts2->Display(); // this crashes. fix the bug!
	delete ts2;

 std::cout<<"---------------------------"<<endl;
	return 0;
} 