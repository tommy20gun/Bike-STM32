#ifndef __MEMORYMAP
#define __MEMORYMAP

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "main.h"
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

typedef enum DataTable{
  headlightON,
  motorTemp,
  ADCreading72V,
  ADCreading12V,
  battTemp,
  Odometer,
  speed,
  hornON,
  brakeON,
  turningLeft,
  turningRight,
  RPM,
  throttleV
}DataTable;

struct taggedBuffer{ 
    DataTable tag;
    uint32_t data;
};


#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization

