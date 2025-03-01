#ifndef __MEMORYMAP
#define __MEMORYMAP

#ifdef __cplusplus
extern "C" {
#endif


typedef struct MemoryMap{
    //slow data
    bool headlightON;
    double motorTemp;
    double ADCreading72V;
    double ADCreading12V;
    double battTemp;
    double Odometer;
    //fast data
    double speed;
    bool hornON;
    bool brakeON;
    bool turningLeft;
    bool turningRight;
    int RPM;
    double throttleV;

    //special command
    /*
    state_t* locked;
    state_t* unlocked;
    bool* BMSMOS_On;
    */
}MemoryMap;

#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization

