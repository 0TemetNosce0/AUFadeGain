/**************************************************************************
**   Created by TemetNosce 2020/5/13
**************************************************************************/
#pragma once


//Reverse crossfader (Hamster Style)线性,反向平滑转换(hamster类型)
#define MIXXX_XFADER_ADDITIVE   0.0//Constant power,Slow fade/Fast cut (additive)缓慢淡出/快速切入(附加)
#define MIXXX_XFADER_CONSTPWR   1.0//Constant power恒定输出功率

class EngineXfader {
public:
    static double getPowerCalibration(double transform);
    static void getXfadeGains(
        double xfadePosition, double transform, double powerCalibration,
        double curve, bool reverse, double* gain1);

    static const char* kXfaderConfigKey;
    static const double kTransformDefault;
    static const double kTransformMax;
    static const double kTransformMin;
};
