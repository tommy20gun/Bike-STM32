#include "MemoryMap.h"

void receiveTaggedData(uint32_t_Buffer* buff, MemoryMap* map){
  //TODO this float needs to be translated
  switch (buff->tag)
  {
  case HEADLIGHT_ON:
    map->headlightON = buff->data;
    break;
  case HORN_ON:
    map->hornON = buff->data;
    break;
  case BRAKE_ON:
    map->brakeON = buff->data;
    break;
  case TURNING_LEFT:
    map->turningLeft = buff->data;
    break;
  case TURNING_RIGHT:
    map->turningRight = buff->data;
    break;
  case STATE_MACHINE_STATUS:
    map->state_machine_status = buff->data;
    break;
  case LED_STATUS:
    map->LED_status = buff->data;
    break;
  case THROTTLE_V:
    map->throttleV = (float) buff->data;
    break;
  case PERCENT_12V_BATT:
    map->percent12VBatt = (float) buff->data;
    break;
  case VOLTAGE_12V_BATT:
    map->voltage12VBatt = (float) buff->data;
    break;
  case BIKE_SPEED:
    map->bike_speed = (float) buff->data;
    break;
  default:
    break;
  }
}

