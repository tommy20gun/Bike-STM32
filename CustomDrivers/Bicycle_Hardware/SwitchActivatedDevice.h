#ifndef __SWITCHACTIVATEDDEVICE
#define __SWITCHACTIVATEDDEVICE

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "Device.h"
#include "MemoryMap.h"

class SwitchActivatedDevice: public Device{
    protected:
    SwitchActivatedDevice(GPIO_TypeDef* GPIOxIn, uint32_t PinMaskIn, GPIO_TypeDef* GPIOxOut, uint32_t PinMaskOut, QueueHandle_t messenger, DataTable queueTag);
    GPIOPin_Struct inputPin;
    GPIOPin_Struct outputPin;

    void initInputPin(GPIOPin_Struct pin, bool PullDown);
    void initOutputPin(GPIOPin_Struct pin, bool OpenDrain);
    virtual void initPeripherals();
    virtual void RTOSImplementation();
    void QueueSend();
    void initializeHWPinState();
};

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SWITCHACTIVATEDDEVICE
