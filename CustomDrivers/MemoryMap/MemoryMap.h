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
    struct PackVoltage{
        double cell1; double cell2;
        double cell3; double cell4;
        double cell5; double cell6;
        double cell7; double cell8;
        double cell9; double cell10;
        double cell11; double cell12;
        double cell13; double cell14;
        double cell15; double cell16;
        double cell17; double cell18;
        double cell19; double cell20;
    };
    double Odometer;
    //fast data
    double speed;
    bool hornON;
    bool brakeON;
    bool turningLeft;
    bool turningRight;
    int RPM;
    double current;
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

