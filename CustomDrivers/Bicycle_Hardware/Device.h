#ifndef __DEVICE
#define __DEVICE

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "MemoryMap.h"

typedef struct GPIOPin_Struct{
    GPIO_TypeDef *GPIOx;
    uint32_t PinMask;
} GPIOPin_Struct;

class Device{
    public:
    TaskHandle_t getTaskHandle();
    protected:
    QueueHandle_t messenger;
    DataTable queueTag;

    TaskHandle_t TaskHandle;

    virtual void initPeripherals() = 0;
    virtual void initTasks() = 0;
    virtual void RTOSImplementation() = 0;
    virtual void QueueSend() = 0;
};


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__DEVICE
