#include <stdio.h>  /*printf(),  */
#include <stdlib.h> /*malloc(), free() */
//-------------------------------------------------
// Vtable Index Maps
#define DTOR_IDX        0   
#define DISPLAY_IDX     1   
#define WASH_IDX        2   
//-------------------------------------------------
#define maxFunc(a, b) ((a) > (b) ? (a) : (b))
//-------------------------------------------------
static int count = 0;
//-------------------------------------------------
typedef void (*VirtualMethod)(void*);
//-------------------------------------------------
//auxillary  func for prinitng 
static void PrintSepperator();
//------------------------------------------------------------------
//Public_Transport
typedef struct 
{
    VirtualMethod* m_vptr;
    int m_license_plate;
    
} Public_Transport;

//FORWARD DECLARATIONS------------------------------------------------
void Public_Transport_Ctor(Public_Transport* this);
void Public_Transport_Cctor(Public_Transport* this, const Public_Transport* other);
void Public_Transport_Dtor(void* this);
void Public_Transport_Display(void* this);
void Public_Transport_PrintCount();
int Public_Transport_GetId(Public_Transport* this);
VirtualMethod* Public_Transport_Get_m_vptr(Public_Transport* this);
static void Public_Transport_Set_m_vptr(Public_Transport* this);

//VTABLE----------------------------------------------------------------
static VirtualMethod Public_Transport_Vtable[] =
{
    Public_Transport_Dtor, 
    Public_Transport_Display
};
//IMPLEMENTATION-------------------------------------------------------
//CTOR
void Public_Transport_Ctor(Public_Transport* this)
{
    this->m_license_plate = ++count;
    Public_Transport_Set_m_vptr(this);
    printf("Public_Transport::Ctor() %d\n", this->m_license_plate);
}
//-----------------------------------------------------------------
//CCTOR
void Public_Transport_Cctor(Public_Transport* this, const Public_Transport* other)
{
    this->m_license_plate = ++count; 
    Public_Transport_Set_m_vptr(this);
    printf("Public_Transport::CCtor() %d\n", this->m_license_plate);
}
//-----------------------------------------------------------------
//DTOR
void Public_Transport_Dtor(void* this)
{
    Public_Transport* self = (Public_Transport*)this;
    --count;
    printf("Public_Transport::Dtor() %d\n", self->m_license_plate);
}
//-----------------------------------------------------------------
//DISPLAY VIRTUAL
void Public_Transport_Display(void* this)
{
    Public_Transport* self = (Public_Transport*)this;
    printf("Public_Transport::Display(): %d\n", self->m_license_plate);
}
//-----------------------------------------------------------------
//PRINT COUNT STATIC FUNC
void Public_Transport_PrintCount()
{
    printf("s_count: %d\n", count);
}
//-----------------------------------------------------------------
//GET ID
int Public_Transport_GetId(Public_Transport* this)
{
    return (this->m_license_plate);
}
//-----------------------------------------------------------------
//HELPER
VirtualMethod* Public_Transport_Get_m_vptr(Public_Transport* this)
{
    return (this->m_vptr);
}
//-----------------------------------------------------------------
//HELPER
static void Public_Transport_Set_m_vptr(Public_Transport* this)
{
    this->m_vptr = Public_Transport_Vtable;
}
//------------------------------------------------------------------------------
// minibus
typedef struct
{
    Public_Transport m_public_transport;
    int m_numSeats;
}Minibus;

//FORWARD DECLARATIONS------------------------------------------------
void Minibus_Ctor(Minibus* this);
void Minibus_Cctor(Minibus* this, Minibus* other);
void  Minibus_Dtor(void* this);
void Minibus_Display(void* this);
void Minibus_Wash(void* this);
VirtualMethod* Minibus_Get_m_vptr(Minibus* this);
static void Minibus_Set_m_vptr(Minibus* this);

//VTABLE----------------------------------------------------------------
static VirtualMethod Minibus_Vtable[] =
{
    Minibus_Dtor, 
    Minibus_Display,
    Minibus_Wash
};
//IMPLEMENTATION-------------------------------------------------------
// ctor
void Minibus_Ctor(Minibus* this)
{
    Public_Transport_Ctor(&this->m_public_transport); 
    Minibus_Set_m_vptr(this);
    this->m_numSeats= 20;
    printf("Minibus::Ctor()\n");
}
//------------------------------------------------------------------------------
//CCTOR
void Minibus_Cctor(Minibus* this, Minibus* other)
{
    Public_Transport_Cctor(&this->m_public_transport, &other->m_public_transport);
    Minibus_Set_m_vptr(this);
    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}
//------------------------------------------------------------------------------
//DTOR
void  Minibus_Dtor(void* this)
{
    Minibus* self = (Minibus*)this;
    printf("Minibus::dtor()\n");
    Public_Transport_Set_m_vptr(&self->m_public_transport);
    Public_Transport_Dtor(&self->m_public_transport);
}
//------------------------------------------------------------------------------
//DISPLAY VIRTUAL
void Minibus_Display(void* this)
{
    Minibus* self = (Minibus*)this;
    printf("MInibus::Display() ID %d", Public_Transport_GetId(&self->m_public_transport));
    printf(" num seats: %d\n", self->m_numSeats);
}
//------------------------------------------------------------------------------
//WASH VIRTUAL
void Minibus_Wash(void* this)
{
    Minibus* self = (Minibus*)this;
    printf("Minibus::Wash() ID: %d\n",Public_Transport_GetId(&self->m_public_transport));
}
//------------------------------------------------------------------------------
//HELPER
VirtualMethod* Minibus_Get_m_vptr(Minibus* this)
{
    //printf("it entered minibus get m_vptr successfully\n");
    return (this->m_public_transport.m_vptr);
}
//------------------------------------------------------------------------------
//HELPER
static void Minibus_Set_m_vptr(Minibus* this)
{
    this->m_public_transport.m_vptr = Minibus_Vtable;
}
//------------------------------------------------------------------------------
//Taxi class

typedef struct 
{
    Public_Transport m_public_transport;
} Taxi;

//FORWARD DECLARATIONS------------------------------------------------
void Taxi_Ctor(Taxi* this);
void Taxi_Cctor(Taxi* this, Taxi* other);
void  Taxi_Dtor(void* this);
void Taxi_Display(void* this);
static VirtualMethod* Taxi_Get_m_vptr(Taxi* this);
static void Taxi_Set_m_vptr(Taxi* this);

//VTABLE----------------------------------------------------------------
static VirtualMethod Taxi_Vtable[] =
{
    Taxi_Dtor, 
    Taxi_Display
};
//IMPLEMENTATION-------------------------------------------------------
// CTOR
void Taxi_Ctor(Taxi* this)
{
    Public_Transport_Ctor(&this->m_public_transport); 
    Taxi_Set_m_vptr(this);
    printf("Taxi::Ctor()\n");
}
//------------------------------------------------------------------------------
//CCTOR
void Taxi_Cctor(Taxi* this, Taxi* other)
{
    Public_Transport_Cctor(&this->m_public_transport, &other->m_public_transport);
    Taxi_Set_m_vptr(this);
    printf("Taxi::CCtor()\n");
}
//------------------------------------------------------------------------------
//DTOR
void  Taxi_Dtor(void* this)
{
    Taxi* self = (Taxi*)this;
    printf("Taxi::dtor()\n");
    Public_Transport_Set_m_vptr(&self->m_public_transport);
    Public_Transport_Dtor(&self->m_public_transport);
}
//------------------------------------------------------------------------------
//DISPLAY VIRTUAL
void Taxi_Display(void* this)
{
    Taxi* self = (Taxi*)this;
    printf("Taxi::Display() ID %d\n", Public_Transport_GetId(&self->m_public_transport));
}
//------------------------------------------------------------------------------
//HELPER
static VirtualMethod* Taxi_Get_m_vptr(Taxi* this)
{
    return (this->m_public_transport.m_vptr);
}

static void Taxi_Set_m_vptr(Taxi* this)
{
    this->m_public_transport.m_vptr = Taxi_Vtable;
}
//------------------------------------------------------------------------------
//special taxi class
typedef struct 
{
    Taxi m_taxi;

} Special_Taxi;

void  Special_Taxi_Dtor(void* this);
void Special_Taxi_Display(void* this);
void Special_Taxi_Ctor(Special_Taxi* this);
void Special_Taxi_Cctor(Special_Taxi* this, Special_Taxi* other);
void Special_Taxi_Display(void* this);
VirtualMethod* Special_Taxi_Get_m_vptr(Special_Taxi* this);
void Special_Taxi_Set_m_vptr(Special_Taxi* this);

static VirtualMethod Special_Taxi_Vtable[] =
{
    Special_Taxi_Dtor, 
    Special_Taxi_Display
};

//ctor
void Special_Taxi_Ctor(Special_Taxi* this)
{
    Taxi_Ctor(&this->m_taxi);
    Special_Taxi_Set_m_vptr(this);
    //this->m_taxi.m_public_transport.m_vptr = Special_Taxi_Vtable;
    printf("Special_Taxi::Ctor()\n");
}

//cctor
void Special_Taxi_Cctor(Special_Taxi* this, Special_Taxi* other)
{
    Taxi_Cctor(&this->m_taxi, &other->m_taxi);
    Special_Taxi_Set_m_vptr(this);
    //this->m_taxi.m_public_transport.m_vptr = Special_Taxi_Vtable;
    printf("Special_Taxi::CCtor()\n");
}

//dtor
void Special_Taxi_Dtor(void* this)
{
    Special_Taxi* self = (Special_Taxi*)this;
    printf("Special_Taxi::dtor()\n");
    Taxi_Set_m_vptr(&self->m_taxi);
    Taxi_Dtor(&self->m_taxi);
}

//display
void Special_Taxi_Display(void* this)
{
    Special_Taxi* self = (Special_Taxi*)this;
    printf("Special_Taxi::Display() ID: %d\n", Public_Transport_GetId(&self->m_taxi.m_public_transport));
}

VirtualMethod* Special_Taxi_Get_m_vptr(Special_Taxi* this)
{
    return (this->m_taxi.m_public_transport.m_vptr);
}

void Special_Taxi_Set_m_vptr(Special_Taxi* this)
{
    this->m_taxi.m_public_transport.m_vptr = Special_Taxi_Vtable;
}

//------------------------------------------------------------------------------
//public convoy
typedef struct 
{
    Public_Transport m_public_transport;
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
    Public_Transport_Ctor(&this->m_public_transport);

    this->m_pt1 = (Minibus*)malloc(sizeof(Minibus));
    Minibus_Ctor((Minibus*)this->m_pt1);

    this->m_pt2 = (Taxi*)malloc(sizeof(Taxi));
    Taxi_Ctor((Taxi*)this->m_pt2);

    //this->m_public_transport.m_vptr = Public_Convoy_Vtable;
    Public_Convoy_Set_m_vptr(&this->m_public_transport);

    Minibus_Ctor(&this->m_m);
    Taxi_Ctor(&this->m_t);
}

void Public_Convoy_Cctor(Public_Convoy* this, Public_Convoy* other)
{
    Public_Transport_Cctor(&this->m_public_transport, &other->m_public_transport);

    this->m_pt1 = (Public_Transport*)malloc(sizeof(Minibus));
    Minibus_Cctor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);

    this->m_pt2 = (Public_Transport*)malloc(sizeof(Taxi));
    Taxi_Cctor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);

    Minibus_Cctor(&this->m_m, &other->m_m);
    Taxi_Cctor(&this->m_t, &other->m_t);

    Public_Convoy_Set_m_vptr(&this->m_public_transport);

    printf("Public_Convoy::CCtor()\n");


}
void Public_Convoy_Dtor(void* this)
{
    Public_Convoy* self = (Public_Convoy*)this;

    self->m_pt1->m_vptr[DTOR_IDX](self->m_pt1);  
    free(self->m_pt1);

    self->m_pt2->m_vptr[DTOR_IDX](self->m_pt2);  
    free(self->m_pt2);

    Taxi_Dtor(&self->m_t);
    Minibus_Dtor(&self->m_m);

    Public_Transport_Dtor(&self->m_public_transport);

    Public_Transport_Set_m_vptr(&self->m_public_transport);
   // printf("Public_Convoy::dtor()\n");
}

void Public_Convoy_Display(void* this)
{
    Public_Convoy* self = (Public_Convoy*)this;
    Public_Transport_Get_m_vptr(self->m_pt1)[DISPLAY_IDX](self->m_pt1);
    Public_Transport_Get_m_vptr(self->m_pt2)[DISPLAY_IDX](self->m_pt2);

    Minibus_Get_m_vptr(&self->m_m)[DISPLAY_IDX](&self->m_m);
    Taxi_Get_m_vptr(&self->m_t)[DISPLAY_IDX](&self->m_t);

}

VirtualMethod* Public_Convoy_Get_m_vptr(Public_Convoy* this)
{
    return (this->m_public_transport.m_vptr);
}

void Public_Convoy_Set_m_vptr(Public_Convoy* this)
{
    this->m_public_transport.m_vptr = Public_Convoy_Vtable;
}
//------------------------------------------------------------------------------
void PrintInfo(Public_Transport* a)
{
    a->m_vptr[DISPLAY_IDX](a);
}

void PrintInfo_Void()
{
    Public_Transport_PrintCount();
}

void PrintInfo_Int(Public_Transport* temporary, int i)
{
    Minibus ret;
    Minibus_Ctor(&ret);

    printf("PrintInfo(int i)\n");
    ret.m_public_transport.m_vptr[DISPLAY_IDX](&ret);
    
    Public_Transport_Cctor(temporary, &ret);
    //destroy local
    Minibus_Dtor(&ret);
}

static void PrintSepperator()
{
    printf("---------------------------\n");
}
//------------------------------------------------------
int main(int argc, char **argv, char **envp)
{
    PrintSepperator();
//--------------------------------------------------------------------
    //CONSTRUCTING A DERIVED OBJECT
        //compiler responsible for destroying it at the end
    Minibus m;
    Minibus_Ctor(&m);
    PrintInfo((Public_Transport*)&m); 

    PrintSepperator();
//--------------------------------------------------------------------
    //CALLING A FUNCTION THAT RETURNS BY VALUE
        /*
          the callee creates a temporary, change it's argument list
          to include a pointer to the temoray and changes it's return type
          to void, within the function itself, it calls the cctor to copy to
          temporary.
         */
    Public_Transport temporary;
    PrintInfo_Int(&temporary, 3);
    temporary.m_vptr[DISPLAY_IDX](&temporary);
    /* temporary dies as soon as we get to ; (unless it has been catched by ref)*/
    Public_Transport_Dtor(&temporary);

    PrintSepperator();
//--------------------------------------------------------------------
    //CONSTRUCTING A POLYMORPHIC ARRAY
        //dynamically allocated, the compiler is not responsible for destroying them
    Public_Transport* array[3];
    
    array[0] = (Public_Transport*)malloc(sizeof(Minibus));
    Minibus_Ctor((Minibus*)array[0]);
    
    array[1] = (Public_Transport*)malloc(sizeof(Taxi));
    Taxi_Ctor((Taxi*)array[1]);
    
    array[2] = (Public_Transport*)malloc(sizeof(Minibus));
    Minibus_Ctor((Minibus*)array[2]);

    PrintSepperator();
//--------------------------------------------------------------------
    //POLYMORPHISM IN RUN-TIME
        //m_vptr already points to the relevant vtable

    for(int i = 0; i < 3; ++i)
    {
        array[i]->m_vptr[DISPLAY_IDX](array[i]);
    } 
    PrintSepperator();

    for(int i = 0; i < 3; ++i) 
    {
        array[i]->m_vptr[DTOR_IDX](array[i]);
        free(array[i]);
    }
    PrintSepperator();
//--------------------------------------------------------------------
    //STORING DERIVED OBJECTS BY VALUE IN A BASE TYPE ARRAY CAUSES SLICING
    //WITH THE HELP OF THE CCTOR

    Public_Transport arr2[3];
    
    Minibus temp_m;
    Minibus_Ctor(&temp_m);
    Public_Transport_Cctor(&arr2[0], &temp_m);
    
    Taxi temp_t;
    Taxi_Ctor(&temp_t);
    Public_Transport_Cctor(&arr2[1], &temp_t);

    Public_Transport_Ctor(&arr2[2]);

    Taxi_Dtor(&temp_t);
    Minibus_Dtor(&temp_m);
    PrintSepperator();

    for(int i = 0; i < 3; ++i) 
    {
        arr2[i].m_vptr[DISPLAY_IDX](&arr2[i]);
    }
    PrintSepperator();

    Public_Transport_PrintCount();
    PrintSepperator();
//--------------------------------------------------------------------
   //THE COMPILER CALLS THE DTORS MANUALLY FOR AUTOMATICALLY ALLOCATED OBJECTS
    Minibus m2; //compiler handle destroying
    Minibus_Ctor(&m2);
    Public_Transport_PrintCount();
    //Minibus_Dtor(&m2); call at the end
    PrintSepperator();
//--------------------------------------------------------------------
   //ARRAY OF AUTOMATICALLY ALLOCATED OBJECTS 
    Minibus arr3[4]; //compiler handle destroying
    for(int i = 0; i < 4; ++i) Minibus_Ctor(&arr3[i]);
    PrintSepperator();
//--------------------------------------------------------------------
    //ARRAY OF DYNAMICALLY ALLOCATED OBJECTS
        // programmers responsible for destroying them, the compiler does not know how many objects are in the array, so it can't call the dtors for them
    Taxi* arr4 = (Taxi*)malloc(sizeof(Taxi) * 4);
    for(int i = 0; i < 4; ++i) Taxi_Ctor(&arr4[i]);
    for(int i = 3; i >= 0; --i) Taxi_Dtor(&arr4[i]);
    free(arr4);
    PrintSepperator();
//--------------------------------------------------------------------
// MACRO TEMPLATES
    printf("%d\n", maxFunc(1, 2));
//-------------------------------------------------------------------- 
   // SLICING 
   printf("SLICING CHECKING TASK -----------------\n");
    Special_Taxi st; //compiler responsible to destroy at the end of main
    Special_Taxi_Ctor(&st);
    PrintSepperator();

    Taxi arg_slice; 
    Taxi_Cctor(&arg_slice, &st.m_taxi);
    Taxi_Display(&arg_slice);
    Taxi_Dtor(&arg_slice); //destroy here bc it's a temporary object ?
    
    //Special_Taxi_Dtor(&st); end of main
    PrintSepperator();
//--------------------------------------------------------------------
    //DEEP COPY 
    Public_Convoy* ts1 = (Public_Convoy*)malloc(sizeof(Public_Convoy));
    Public_Convoy_Ctor(ts1);
    PrintSepperator();

    Public_Convoy* ts2 = (Public_Convoy*)malloc(sizeof(Public_Convoy));
    Public_Convoy_Cctor(ts2, ts1);
    PrintSepperator();

    ts1->m_public_transport.m_vptr[DISPLAY_IDX](ts1);
    PrintSepperator();

    ts2->m_public_transport.m_vptr[DISPLAY_IDX](ts2);
    PrintSepperator();

    ts1->m_public_transport.m_vptr[DTOR_IDX](ts1);
    free(ts1);
    PrintSepperator();

    ts2->m_public_transport.m_vptr[DISPLAY_IDX](ts2);
    PrintSepperator(); 

    ts2->m_public_transport.m_vptr[DTOR_IDX](ts2);
    free(ts2);
    PrintSepperator();
//--------------------------------------------------------------------
    //COMPILER IMPLICIT DESTRCTORS CALLS IN REVERSE ORDER OF CONSTRUCTION
 Special_Taxi_Dtor(&st);         

for(int i = 3; i >= 0; i--)
{
    Minibus_Dtor(&arr3[i]);     
}

Minibus_Dtor(&m2);              

for(int i = 2; i >= 0; i--)
{
    // polymorphism 
    arr2[i].m_vptr[DTOR_IDX](&arr2[i]);
    //Public_Transport_Dtor(&arr2[i]);  
}

Minibus_Dtor(&m);              
PrintSepperator();
//--------------------------------------------------------------------
    return 0;
}
