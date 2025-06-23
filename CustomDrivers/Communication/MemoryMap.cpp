#include "MemoryMap.h"

void receiveTaggedData(uint32_t_Buffer* buff, MemoryMap* map){
  //TODO this float needs to be translated
  switch (buff->tag)
  {
  case 0:
    map->headlightON = buff->data;
    break;
  case 1:
    map->motorTemp = buff->data;
    break;
  case 2:
    map->ADCreading72V = buff->data;
    break;
  case 3:
    map->ADCreading12V = buff->data;
    break;
  case 4:
    map->battTemp = buff->data;
    break;
  case 5:
    map->Odometer = buff->data;
    break;
  case 6:
    map->speed = buff->data;
    break;
  case 7:
    map->hornON = buff->data;
    break;
  case 8:
    map->brakeON = buff->data;
    break;
  case 9:
    map->turningLeft = buff->data;
    break;
  case 10:
    map->turningRight = buff->data;
    break;
  case 11:
    map->RPM = buff->data;
    break;
  case 12:
    map->throttleV = buff->data;
    break;
  case 13:
    map->StateMachineStatus = buff->data;
  }
}

