#include <stdio.h>
#include <stdlib.h>
//-------------------------------------------------
// Vtable Index Maps
#define DTOR_IDX        0   
#define DISPLAY_IDX     1   
#define WASH_IDX        2   

static int count = 0;
// template 
#define maxFunc(a, b) ((a) > (b) ? (a) : (b))

typedef void (*VirtualMethod)(void*);

//aux for prinitng 
static void PrintSepperator();
//---------------------------------------------------------------
//Public_Transport
typedef struct 
{
    VirtualMethod* vptr;
    int m_license_plate;
    
} Public_Transport;

// forward declaration of the relevant functions for this struct
void Public_Transport_Ctor(Public_Transport* this);
void Public_Transport_Cctor(Public_Transport* this, const Public_Transport* other);
void Public_Transport_Dtor(void* this);
void Public_Transport_Display(void* this);
void Public_Transport_PrintCount();
int Public_Transport_GetId(Public_Transport* this);
VirtualMethod* Public_Transport_Get_Vptr(Public_Transport* this);
static void Public_Transport_Set_Vptr(Public_Transport* this);


static VirtualMethod Public_Transport_Vtable[] =
{
    Public_Transport_Dtor, 
    Public_Transport_Display
};

// ctor
void Public_Transport_Ctor(Public_Transport* this)
{
    this->m_license_plate = ++count;
    Public_Transport_Set_Vptr(this);
    printf("Public_Transport::Ctor() %d\n", this->m_license_plate);
}

void Public_Transport_Cctor(Public_Transport* this, const Public_Transport* other)
{
    this->m_license_plate = ++count; 
    Public_Transport_Set_Vptr(this);
    printf("Public_Transport::CCtor() %d\n", this->m_license_plate);
}

void Public_Transport_Dtor(void* this)
{
    Public_Transport* self = (Public_Transport*)this;
    --count;
    printf("Public_Transport::Dtor() %d\n", self->m_license_plate);
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

static void Public_Transport_Set_Vptr(Public_Transport* this)
{
    this->vptr = Public_Transport_Vtable;
}
//------------------------------------------------------------------------------
// minibus
typedef struct
{
    Public_Transport base_class;
    int m_numSeats;
}Minibus;

//forward declaration of minibus relevant functions
void Minibus_Ctor(Minibus* this);
void Minibus_Cctor(Minibus* this, Minibus* other);
void  Minibus_Dtor(void* this);
void Minibus_Display(void* this);
void Minibus_Wash(void* this);
VirtualMethod* Minibus_Get_Vptr(Minibus* this);
static void Minibus_Set_Vptr(Minibus* this);


static VirtualMethod Minibus_Vtable[] =
{
    Minibus_Dtor, 
    Minibus_Display,
    Minibus_Wash
};

// ctor
void Minibus_Ctor(Minibus* this)
{
    Public_Transport_Ctor(&this->base_class); 
    Minibus_Set_Vptr(this);
    this->m_numSeats= 20;
    printf("Minibus::Ctor()\n");
}

void Minibus_Cctor(Minibus* this, Minibus* other)
{
    Public_Transport_Cctor(&this->base_class, &other->base_class);
    Minibus_Set_Vptr(this);
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
    //printf("it entered minibus get vptr successfully\n");
    return (this->base_class.vptr);
}

static void Minibus_Set_Vptr(Minibus* this)
{
    this->base_class.vptr = Minibus_Vtable;
}
//------------------------------------------------------------------------------
//Taxi class

typedef struct 
{
    Public_Transport base_class;
} Taxi;

void Taxi_Ctor(Taxi* this);
void Taxi_Cctor(Taxi* this, Taxi* other);
void  Taxi_Dtor(void* this);
void Taxi_Display(void* this);
static VirtualMethod* Taxi_Get_Vptr(Taxi* this);
static void Taxi_Set_Vptr(Taxi* this);


static VirtualMethod Taxi_Vtable[] =
{
    Taxi_Dtor, 
    Taxi_Display
};

// ctor
void Taxi_Ctor(Taxi* this)
{
    Public_Transport_Ctor(&this->base_class); 
    Taxi_Set_Vptr(this);
    printf("Taxi::Ctor()\n");
}

void Taxi_Cctor(Taxi* this, Taxi* other)
{
    Public_Transport_Cctor(&this->base_class, &other->base_class);
    Taxi_Set_Vptr(this);
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
    printf("Taxi::Display() ID %d\n", Public_Transport_GetId(&self->base_class));
}

static VirtualMethod* Taxi_Get_Vptr(Taxi* this)
{
    return (this->base_class.vptr);
}

static void Taxi_Set_Vptr(Taxi* this)
{
    this->base_class.vptr = Taxi_Vtable;
}
//------------------------------------------------------------------------------
//special taxi class
typedef struct 
{
    Taxi base_class_taxi;

} Special_Taxi;

void  Special_Taxi_Dtor(void* this);
void Special_Taxi_Display(void* this);
void Special_Taxi_Ctor(Special_Taxi* this);
void Special_Taxi_Cctor(Special_Taxi* this, Special_Taxi* other);
void Special_Taxi_Display(void* this);
VirtualMethod* Special_Taxi_Get_Vptr(Special_Taxi* this);
void Special_Taxi_Set_Vptr(Special_Taxi* this);

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
    Taxi_Cctor(&this->base_class_taxi, &other->base_class_taxi);
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

void Special_Taxi_Set_Vptr(Special_Taxi* this)
{
    this->base_class_taxi.base_class.vptr = Special_Taxi_Vtable;
}

//------------------------------------------------------------------------------
//public convoy
typedef struct 
{
    Public_Transport base_class;
    Public_Transport* m_pt1;
    Public_Transport* m_pt2;
    Minibus m_m;
    Taxi m_t;
}Public_Convoy;

void  Public_Convoy_Dtor(void* this);
void Public_Convoy_Display(void* this);

static VirtualMethod Public_Convoy_Vtable[] =
{
    Public_Convoy_Dtor, 
    Public_Convoy_Display
};
void Public_Convoy_Ctor(Public_Convoy* this)
{
    Public_Transport_Ctor(&this->base_class);

    this->m_pt1 = (Minibus*)malloc(sizeof(Minibus));
    Minibus_Ctor((Minibus*)this->m_pt1);

    this->m_pt2 = (Taxi*)malloc(sizeof(Taxi));
    Taxi_Ctor((Taxi*)this->m_pt2);

    this->base_class.vptr = Public_Convoy_Vtable;

    Minibus_Ctor(&this->m_m);
    Taxi_Ctor(&this->m_t);

    printf("Public Convoy Ctor\n");
}

void Public_Convoy_Cctor(Public_Convoy* this, Public_Convoy* other)
{
    Public_Transport_Cctor(&this->base_class, &other->base_class);

    this->m_pt1 = (Public_Transport*)malloc(sizeof(Minibus));
    Minibus_Cctor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);

    this->m_pt2 = (Public_Transport*)malloc(sizeof(Taxi));
    Taxi_Cctor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);

    Minibus_Cctor(&this->m_m, &other->m_m);
    Taxi_Cctor(&this->m_t, &other->m_t);

    Public_Convoy_Set_Vptr(this);


}
void Public_Convoy_Dtor(void* this)
{
    Public_Convoy* self = (Public_Convoy*)this;
    free(self->m_pt1);
    self->m_pt1 = NULL;
    free(self->m_pt2);
    self->m_pt2 = NULL;

    Minibus_Dtor(&self->m_m);
    Taxi_Dtor(&self->m_t);

    Public_Transport_Dtor(&self->base_class);

    printf("Public_Convoy::dtor()\n");
}

void Public_Convoy_Display(void* this)
{
    Public_Convoy* self = (Public_Convoy*)this;
    Public_Transport_Get_Vptr(self->m_pt1)[DISPLAY_IDX](self->m_pt1);
    Public_Transport_Get_Vptr(self->m_pt2)[DISPLAY_IDX](self->m_pt2);

    Minibus_Get_Vptr(&self->m_m)[DISPLAY_IDX](&self->m_m);
    Taxi_Get_Vptr(&self->m_t)[DISPLAY_IDX](&self->m_t);

}

VirtualMethod* Public_Convoy_Get_Vptr(Public_Convoy* this)
{
    return (this->base_class.vptr);
}

void Public_Convoy_Set_Vptr(Public_Convoy* this)
{
    this->base_class.vptr = Public_Convoy_Vtable;
}
//------------------------------------------------------------------------------
void PrintInfo(Public_Transport* a)
{
    a->vptr[DISPLAY_IDX](a);
}

void PrintInfo_Void()
{
    Public_Transport_PrintCount();
}

Public_Transport PrintInfo_Int(int i)
{
    Minibus ret;
    Minibus_Ctor(&ret);
    printf("PrintInfo(int i)\n");
    ret.base_class.vptr[DISPLAY_IDX](&ret);
    
    Public_Transport sliced_copy;
    Public_Transport_Cctor(&sliced_copy, &ret.base_class);
    
    Minibus_Dtor(&ret);
    return sliced_copy;
}

void TaxiDisplay(Taxi s)
{
    s.base_class.vptr[DISPLAY_IDX](&s);
}

static void PrintSepperator()
{
    printf("---------------------------\n");
}
//------------------------------------------------------
int main(int argc, char **argv, char **envp)
{
    Minibus m;
    Minibus_Ctor(&m);
    PrintInfo((Public_Transport*)&m);
    Public_Transport sliced_return = PrintInfo_Int(3);
    sliced_return.vptr[DISPLAY_IDX](&sliced_return);

    Public_Transport_Dtor(&sliced_return);

    /*Public_Transport* array[3];
    
    array[0] = (Public_Transport*)malloc(sizeof(Minibus));
    Minibus_Ctor((Minibus*)array[0]);
    
    array[1] = (Public_Transport*)malloc(sizeof(Taxi));
    Taxi_Ctor((Taxi*)array[1]);
    
    array[2] = (Public_Transport*)malloc(sizeof(Minibus));
    Minibus_Ctor((Minibus*)array[2]);

    PrintSepperator();

    for(int i = 0; i < 3; ++i) array[i]->vptr[DISPLAY_IDX](array[i]);
    for(int i = 0; i < 3; ++i) 
    {
        array[i]->vptr[DTOR_IDX](array[i]);
        free(array[i]);
    }
    PrintSepperator();

    Public_Transport arr2[3];
    
    Minibus temp_m;
    Minibus_Ctor(&temp_m);
    Public_Transport_Cctor(&arr2[0], &temp_m.base_class);
    Minibus_Dtor(&temp_m);

    Taxi temp_t;
    Taxi_Ctor(&temp_t);
    Public_Transport_Cctor(&arr2[1], &temp_t.base_class);
    Taxi_Dtor(&temp_t);

    Public_Transport_Ctor(&arr2[2]);
    PrintSepperator();

    for(int i = 0; i < 3; ++i) 
    {
        arr2[i].vptr[DISPLAY_IDX](&arr2[i]);
    }
    PrintSepperator();
    
    Public_Transport_PrintCount();
    Minibus m2;
    Minibus_Ctor(&m2);
    Public_Transport_PrintCount();
    Minibus_Dtor(&m2);
    PrintSepperator();

    Minibus arr3[4];
    for(int i = 0; i < 4; ++i) Minibus_Ctor(&arr3[i]);
    PrintSepperator();

    Taxi* arr4 = (Taxi*)malloc(sizeof(Taxi) * 4);
    for(int i = 0; i < 4; ++i) Taxi_Ctor(&arr4[i]);
    for(int i = 3; i >= 0; --i) Taxi_Dtor(&arr4[i]);
    free(arr4);
    PrintSepperator();

    printf("print the max\n");
    printf("%d\n", maxFunc(1, 2));
    
    Special_Taxi st;
    Special_Taxi_Ctor(&st);
    
    Taxi arg_slice;
    Taxi_Cctor(&arg_slice, &st.base_class_taxi);
    TaxiDisplay(arg_slice);
    Taxi_Dtor(&arg_slice);
    
    Special_Taxi_Dtor(&st);
    PrintSepperator();

    Public_Convoy* ts1 = (Public_Convoy*)malloc(sizeof(Public_Convoy));
    Public_Convoy_Ctor(ts1);
    
    Public_Convoy* ts2 = (Public_Convoy*)malloc(sizeof(Public_Convoy));
    Public_Convoy_Cctor(ts2, ts1);
    PrintSepperator();

    ts1->base_class.vptr[DISPLAY_IDX](ts1);
    ts2->base_class.vptr[DISPLAY_IDX](ts2);
    
    ts1->base_class.vptr[DTOR_IDX](ts1);
    free(ts1);

    ts2->base_class.vptr[DISPLAY_IDX](ts2);
    ts2->base_class.vptr[DTOR_IDX](ts2);
    free(ts2);

    PrintSepperator();

    for(int i = 2; i >= 0; --i) 
    {
        Public_Transport_Dtor(&arr2[i]);
    }
    for(int i = 3; i >= 0; --i)
    {
      Minibus_Dtor(&arr3[i]);  
    } 
    Minibus_Dtor(&m);

    PrintSepperator();*/

    return 0;
}
