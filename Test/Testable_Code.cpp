#include "Testable_Code.h"

float ADCToBatteryPercent(float ADCReading,float scale){
    float vdd = 3.33;
    float resolution = 4096;
    float cellsInSeries = 3;
    float voltage = ADCReading/resolution*vdd*scale; //assume 12bit resolution, 0-3.3V ADC range, yields 0-4.2V range
    float voltagePerCell = voltage/cellsInSeries;

    //coeff
    //voltage > 3.63
    float a = 2118.84909f;
    float b = -33020.48373f;
    float c = 192575.77552f;
    float d = -497932.21476f;
    float e = 481479.72081;

    //voltage < 3.63
    float a1 = 21.0769f;
    float b1 = -3.5f;

    //x = -500.86y^3 + 5720y^2 -21557y+26840 (y in range of 3.63-4.2V)
    //x = 21.0769*(y-3.5) (y < 3.63)
    float batteryPercentage;
    if (voltagePerCell > 3.63f){ //testing
        //voltagePerCell > 3.63f && voltagePerCell < 4.25f real
        batteryPercentage = (((((a*voltagePerCell+b)*voltagePerCell) + c)*voltagePerCell + d) * voltagePerCell) + e;
    }
    else if (voltagePerCell <= 3.63f && voltagePerCell > 3.45f){
        batteryPercentage = a1*(voltagePerCell+b1);
    }
    else {
        batteryPercentage = -1.0f;//Error bc battery% cant be negative
    }
    return batteryPercentage;
}