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
    void initPeripherals();
    void RTOSImplementation();
    void QueueSend();
};


class TestA : public SwitchActivatedDevice{
    public:
    TestA(GPIO_TypeDef* GPIOxIn, 
            uint32_t PinMaskIn, 
            GPIO_TypeDef* GPIOxOut, 
            uint32_t PinMaskOut, 
            QueueHandle_t messenger,
            DataTable queueTag):SwitchActivatedDevice(GPIOxIn,
                                                            PinMaskIn,
                                                            GPIOxOut,
                                                            PinMaskOut,
                                                            messenger,
                                                            queueTag)
    {};
    protected:
    virtual void initTasks();
    static void vTaskFunction(void* pvParameters);//this is just 1 line FreeRTOSImplementation()
};

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SWITCHACTIVATEDDEVICE
