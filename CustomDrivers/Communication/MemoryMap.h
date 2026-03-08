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
  uint32_t headlightON;
  float motorTemp;
  float ADCreading72V;
  float ADCreading12V;
  float battTemp;
  float Odometer;
  //fast data
  float speed;
  uint32_t hornON;
  uint32_t brakeON;
  uint32_t turningLeft;
  uint32_t turningRight;
  float throttleV;
  uint32_t StateMachineStatus;
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
  RPM_data,
  throttleV,
  StateMachineStatus
}DataTable;

struct uint32_t_Buffer{ 
  DataTable tag;
  uint32_t data;
};

struct floatBuffer{
  DataTable tag;
  float data;
};

struct int32_t_Buffer{ 
  DataTable tag;
  int32_t data;
};

void receiveTaggedData(uint32_t_Buffer* buff, MemoryMap* map);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization

