#ifndef __MEMORYMAP
#define __MEMORYMAP

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "main.h"
typedef struct MemoryMap{
  uint32_t magicNumber;
  uint32_t headlightON;
  uint32_t hornON;
  uint32_t brakeON;
  uint32_t turningLeft;
  uint32_t turningRight;
  uint32_t state_machine_status;
  uint32_t LED_status;

  float throttleV; //V
  float percent12VBatt; //percent
  float voltage12VBatt; //V
  float bike_speed; //mph

  uint32_t CRC32;
}MemoryMap;

typedef enum DataTable{
  HEADLIGHT_ON = 0u,
  HORN_ON, 
  BRAKE_ON,
  TURNING_LEFT,
  TURNING_RIGHT,
  STATE_MACHINE_STATUS,
  LED_STATUS,
  THROTTLE_V,
  PERCENT_12V_BATT,
  VOLTAGE_12V_BATT,
  BIKE_SPEED,
  DATATABLE_MAX
}DataTable;

struct uint32_t_Buffer{ 
  DataTable tag;
  uint32_t data;
};

struct floatBuffer{
  DataTable tag;
  float data;
};

// struct int32_t_Buffer{ 
//   DataTable tag;
//   int32_t data;
// };

void receiveTaggedData(uint32_t_Buffer* buff, MemoryMap* map);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization

