#ifndef __DEVICE
#define ____DEVICE

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "MemoryMap.h"

class Device{
    protected:
    QueueHandle_t messenger;
    DataTable queueTag;
    typedef struct GPIOPin_Struct{
        GPIO_TypeDef *GPIOx;
        uint32_t PinMask;
    } GPIOPin_Struct;
    TaskHandle_t TaskHandle;

    virtual void initPeripherals();
    virtual void initTasks();
    virtual void RTOSImplementation();
    virtual void Queuesend();

};


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__DEVICE
