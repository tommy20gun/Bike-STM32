#ifndef __MEMORYMAP
#define __MEMORYMAP

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "main.h"
typedef struct MemoryMap{
  //slow data
  uint32_t magicNumber;
  bool headlightON;
  float motorTemp;
  float ADCreading72V;
  float ADCreading12V;
  float battTemp;
  float Odometer;
  //fast data
  float speed;
  bool hornON;
  bool brakeON;
  bool turningLeft;
  bool turningRight;
  int RPM;
  float throttleV;
  uint32_t CRC32;

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

MemoryMap* receiveTaggedData(taggedBuffer* buff, MemoryMap* map);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization

