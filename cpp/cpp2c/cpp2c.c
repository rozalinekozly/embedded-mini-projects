#include <stdio.h>

//-------------------------------------------------
// Vtable Index Maps
#define DTOR_IDX        0   
#define DISPLAY_IDX     1   
#define WASH_IDX        2   
#define DELETE_IDX      3  

static int count = 0;

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
//------------------------------------------------------------------------------
int main(int argc, char** argv, char** envp)
{
    printf("--- 1. Testing Standalone Base Class ---\n");
    Public_Transport pt;
    Public_Transport_Ctor(&pt);
    pt.vptr[DISPLAY_IDX](&pt); // Virtual call to Display (Index 1)
    printf("\n");

    printf("--- 2. Testing Derived Class (Minibus) ---\n");
    Minibus m;
    Minibus_Ctor(&m); // Automatically runs Public_Transport_Ctor first
    
    // Virtual call to Minibus Display (Index 1) via the base_class pointer
    m.base_class.vptr[DISPLAY_IDX](&m); 
    
    // Virtual call to Minibus Wash (Index 2)
    m.base_class.vptr[WASH_IDX](&m);
    printf("\n");

    printf("--- 3. Testing Global Count Tracking ---\n");
    Public_Transport_PrintCount();
    printf("\n");

    printf("--- 4. Testing Polymorphic Base Pointer Pass ---\n");
    // You can safely cast a Minibus pointer to a Public_Transport pointer
    Public_Transport* poly_ptr = (Public_Transport*)&m;
    
    // This executes Minibus_Display because poly_ptr->vptr points to Minibus_Vtable
    poly_ptr->vptr[DISPLAY_IDX](poly_ptr);
    printf("\n");

    printf("--- 5. Testing Manual Copy Constructor (CCtor) ---\n");
    Public_Transport pt_copy;
    Public_Transport_Cctor(&pt_copy, &pt);
    pt_copy.vptr[DISPLAY_IDX](&pt_copy);
    printf("\n");

    printf("--- 6. Explicit Cleanup (Destructors) ---\n");
    // C does not call destructors automatically, we must do it manually
    m.base_class.vptr[DTOR_IDX](&m); // Calls Minibus_Dtor
    pt.vptr[DTOR_IDX](&pt);          // Calls Public_Transport_Dtor
    pt_copy.vptr[DTOR_IDX](&pt_copy);

    return 0;
}
