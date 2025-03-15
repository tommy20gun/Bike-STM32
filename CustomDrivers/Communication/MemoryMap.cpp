#include "MemoryMap.h"

MemoryMap* receiveTaggedData(taggedBuffer* buff, MemoryMap* map){
  switch (buff->tag)
  {
  case 0:
    map->headlightON = (int) buff->data;
    break;
  case 1:
    map->motorTemp = (float) buff->data;
    break;
  case 2:
    map->ADCreading72V = (float)buff->data;
    break;
  case 3:
    map->ADCreading12V = (float) buff->data;
    break;
  case 4:
    map->battTemp = (float) buff->data;
    break;
  case 5:
    map->Odometer = (float) buff->data;
    break;
  case 6:
    map->speed = (float) buff->data;
    break;
  case 7:
    map->hornON = (bool) buff->data;
    break;
  case 8:
    map->brakeON = (bool) buff->data;
    break;
  case 9:
    map->turningLeft = (bool) buff->data;
    break;
  case 10:
    map->turningRight = (bool) buff->data;
    break;
  case 11:
    map->RPM = (int) buff->data;
    break;
  case 12:
    map->throttleV = (float) buff->data;
    break;
  }
  return map;
}

