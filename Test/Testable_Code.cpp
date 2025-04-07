#include "Testable_Code.h"

float ADCToBatteryPercent(float ADCReading,float scale){
    float vdd = 3.33;
    float resolution = 4096;
    float cellsInSeries = 3;
    float voltage = ADCReading/resolution*vdd*scale; //assume 12bit resolution, 0-3.3V ADC range, yields 0-4.2V range
    float voltagePerCell = voltage/cellsInSeries;

    //coeff
    //voltage > 3.63
    float a = 1839.98366f;
    float b = -28678.5498f;
    float c = 167243.8687f;
    float d = -432295.73505f;
    float e = 417750.60866f;

    //voltage < 3.63
    float a1 = 49.2366f;
    float b1 = -3.5f;

    //x = 1839.98366*y^4 - 28678.5498*y^3 + 167243.8687*y^2 - 432295.73505*y + 417750.60866 (y in range of 3.685-4.2V)
    //x = 49.2366*(y-3.5) (y < 3.685)
    float batteryPercentage;
    if (voltagePerCell > 3.685f){
        batteryPercentage = (((((a*voltagePerCell+b)*voltagePerCell) + c)*voltagePerCell + d) * voltagePerCell) + e;
    }
    else if (voltagePerCell <= 3.685f && voltagePerCell > 3.2f){
        batteryPercentage = a1*(voltagePerCell+b1);
    }
    else {
        batteryPercentage = -1.0f;//Error bc battery% cant be negative
    }
    return batteryPercentage;
}