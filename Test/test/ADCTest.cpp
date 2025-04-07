#include <gtest/gtest.h>
#include "../Testable_Code.h"
#include <gmock/gmock.h>

using namespace ::testing;

template <typename T>
auto IsInRange(T lo, T hi) {
    return AllOf(Ge((lo)), Le((hi)));
}

TEST(ADCTest, Test_battery_percentage){
    float voltageTable[21] = {4.2, 4.15,4.111,4.06,4.023,3.983,3.953,3.913,3.875,3.855,3.835,3.815,3.795,3.786,3.766,3.746,3.726,3.706,3.686,3.608,3.5};
    float range = 2.5;
    for (int i = 0; i < sizeof(voltageTable)/4; i++){
        float ADCReading = voltageTable[i] *3 / 3.33 / 4 * 4096;
        EXPECT_THAT(ADCToBatteryPercent(ADCReading,4), IsInRange((float)100-(i*5)-range, (float)100-(i*5)+range));
    }
}