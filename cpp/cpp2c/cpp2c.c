#include <stdio.h>

//-------------------------------------------------
// Vtable Index Maps
#define DTOR_IDX        0   
#define DISPLAY_IDX     1   
#define WASH_IDX        2   

static int count = 0;
// template 
#define maxFunc(a, b) ((a) > (b) ? (a) : (b))

typedef void (*VirtualMethod)(void*);

//---------------------------------------------------------------
//Public_Transport
typedef struct 
{
    VirtualMethod* vptr;
    int m_license_plate;
    
} Public_Transport;

void Public_Transport_Dtor(void* this);
void Public_Transport_Display(void* this);


static VirtualMethod Public_Transport_Vtable[] =
{
    Public_Transport_Dtor, 
    Public_Transport_Display
};

// ctor
void Public_Transport_Ctor(Public_Transport* this)
{
    this-> vptr = Public_Transport_Vtable;
    this->m_license_plate = ++count;
    printf("Public_Transport::Ctor()\n");
}

void Public_Transport_Cctor(Public_Transport* this, const Public_Transport* other)
{
    this->vptr = Public_Transport_Vtable;
    this->m_license_plate = ++count; 
    printf("Public_Transport::CCtor()\n");
}

void Public_Transport_Dtor(void* this)
{
    Public_Transport* self = (Public_Transport*)this;
    --count;
    printf("Public_Transport::Dtor()\n");
}

void Public_Transport_Display(void* this)
{
    Public_Transport* self = (Public_Transport*)this;
    printf("Public_Transport::Display(): %d\n", self->m_license_plate);
}

void Public_Transport_PrintCount()
{
    printf("s_count: %d\n", count);
}

int Public_Transport_GetId(Public_Transport* this)
{
    return (this->m_license_plate);
}

VirtualMethod* Public_Transport_Get_Vptr(Public_Transport* this)
{
    return (this->vptr);
}
//------------------------------------------------------------------------------
typedef struct
{
    Public_Transport base_class;
    int m_numSeats;
}Minibus;

void  Minibus_Dtor(void* this);
void Minibus_Display(void* this);
void Minibus_Wash(void* this);


static VirtualMethod Minibus_Vtable[] =
{
    Minibus_Dtor, 
    Minibus_Display,
    Minibus_Wash
};

// ctor
void Minibus_Ctor(void* this)
{
    Minibus* self = (Minibus*)this;
    Public_Transport_Ctor(&self->base_class); 
    self-> base_class.vptr = Minibus_Vtable;
    self->m_numSeats= 20;
    printf("Minibus::Ctor()\n");
}

void Minibus_Cctor(Minibus* this, Minibus* other)
{
    Public_Transport_Cctor(&this->base_class, &other->base_class);
    this->base_class.vptr = Minibus_Vtable;
    this->m_numSeats = other->m_numSeats;
}

void  Minibus_Dtor(void* this)
{
    Minibus* self = (Minibus*)this;
    printf("Minibus::dtor()\n");
    Public_Transport_Dtor(&self->base_class);
}

void Minibus_Display(void* this)
{
    Minibus* self = (Minibus*)this;
    printf("MInibus::Display() ID %d", Public_Transport_GetId(&self->base_class));
    printf(" num seats: %d\n", self->m_numSeats);
}

void Minibus_Wash(void* this)
{
    Minibus* self = (Minibus*)this;
    printf("Minibus::Wash() ID: %d\n",Public_Transport_GetId(&self->base_class));
}

VirtualMethod* Minibus_Get_Vptr(Minibus* this)
{
    return (this->base_class.vptr);
}
//------------------------------------------------------------------------------
//Taxi class

typedef struct 
{
    Public_Transport base_class;
} Taxi;

void  Taxi_Dtor(void* this);
void Taxi_Display(void* this);
void Taxi_Wash(void* this);

static VirtualMethod Taxi_Vtable[] =
{
    Taxi_Dtor, 
    Taxi_Display
};

// ctor
void Taxi_Ctor(void* this)
{
    Taxi* self = (Taxi*)this;
    Public_Transport_Ctor(&self->base_class); 
    self-> base_class.vptr = Taxi_Vtable;
    printf("Taxi::Ctor()\n");
}

void Taxi_Cctor(Taxi* this, Taxi* other)
{
    Public_Transport_Cctor(&this->base_class, &other->base_class);
    this->base_class.vptr = Taxi_Vtable;
}

void  Taxi_Dtor(void* this)
{
    Taxi* self = (Taxi*)this;
    printf("Taxi::dtor()\n");
    Public_Transport_Dtor(&self->base_class);
}

void Taxi_Display(void* this)
{
    Taxi* self = (Taxi*)this;
    printf("Taxi::Display() ID %d", Public_Transport_GetId(&self->base_class));
}

VirtualMethod* Taxi_Get_Vptr(Taxi* this)
{
    return (this->base_class.vptr);
}
//------------------------------------------------------------------------------
//special taxi class
typedef struct 
{
    Taxi base_class_taxi;

} Special_Taxi;

void  Special_Taxi_Dtor(void* this);
void Special_Taxi_Display(void* this);

static VirtualMethod Special_Taxi_Vtable[] =
{
    Special_Taxi_Dtor, 
    Special_Taxi_Display
};


//ctor
void Special_Taxi_Ctor(Special_Taxi* this)
{
    Taxi_Ctor(&this->base_class_taxi);
    this->base_class_taxi.base_class.vptr = Special_Taxi_Vtable;
    printf("Special_Taxi::Ctor()\n");
}

//cctor
void Special_Taxi_Cctor(Special_Taxi* this, Special_Taxi* other)
{
    Taxi_Ctor(&this->base_class_taxi);
    this->base_class_taxi.base_class.vptr = Special_Taxi_Vtable;
    printf("Special_Taxi::CCtor()\n");
}

//dtor
void Special_Taxi_Dtor(void* this)
{
    Special_Taxi* self = (Special_Taxi*)this;
    printf("Special_Taxi::dtor()\n");
    Taxi_Dtor(&self->base_class_taxi);
}

//display
void Special_Taxi_Display(void* this)
{
    Special_Taxi* self = (Special_Taxi*)this;
    printf("Special_Taxi::Display() ID: %d\n", Public_Transport_GetId(&self->base_class_taxi.base_class));
}

VirtualMethod* Special_Taxi_Get_Vptr(Special_Taxi* this)
{
    return (this->base_class_taxi.base_class.vptr);
}

//------------------------------------------------------------------------------
//public convoy
/*typedef struct 
{
    Public_Transport* m_pt1;
    Public_Transport* m_pt2;
    Minibus m_m;
    Taxi m_t;
}Public_Convoy;*/
//------------------------------------------------------------------------------
int main(int argc, char** argv, char** envp)
{
   Minibus pt;

    Minibus_Ctor(&pt);
    Minibus_Get_Vptr(&pt)[DISPLAY_IDX](&pt);
    Minibus_Get_Vptr(&pt)[WASH_IDX](&pt);
    Minibus_Get_Vptr(&pt)[DTOR_IDX](&pt);

    Special_Taxi st;

    Special_Taxi_Ctor(&st);
    Special_Taxi_Get_Vptr(&st)[DISPLAY_IDX](&st);
    Special_Taxi_Get_Vptr(&st)[DTOR_IDX](&st);
    





    return 0;
}
