#ifndef __SWITCHACTIVATEDDEVICE
#define __SWITCHACTIVATEDDEVICE

#ifdef __cplusplus
extern "C" {
#endif

class SwitchActivatedDevice{
    protected:
    virtual void SwitchActivation(void* pvParameters);
}

class TestA : public SwitchActivatedDevice{
    void SwitchActivation();
}


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SWITCHACTIVATEDDEVICE
