#include "Quadrocopter.h"
#include "ComplementaryFilter.cpp"
#include "LowPassFilter.cpp"

void Quadrocopter::processSensorsData()
{
    static RVector3D accelAngle, gyroCosines;

    if(reactionType == ReactionAngle || reactionType == ReactionAcceleration)
        accelDataRaw = Accel->getReadings();
    else accelDataRaw = RVector3D();

/*    if(reactionType != ReactionNone)
        angularVelocity = Gyro->getReadings();*/
    /*else*/ angularVelocity = RVector3D();

//    if(reactionType == ReactionAngle)
//    {
//        // calculate fast gyro estimate
//        gyroCosines = directionalCosines.getValue();
//        gyroCosines += (gyroCosines ^ angularVelocity) * dtSensors; // v = [-w, r] = [r, w]

//        // calculate slow accel estimate
        accelData.iteration(accelDataRaw, dtSensors);

//        // complementary filter
//        directionalCosines.iteration(gyroCosines, accelData.getValue().normalize(), dtSensors);
//        directionalCosines.setValue(directionalCosines.getValue().normalize());

//        // converting value to 3 angles
//        angle = directionalCosines.getValue().angleFromProjections();

//        angularVelocityPrev = angularVelocity;
//    }

//    voltage = VSensor->getValue();
}

void Quadrocopter::processSensorsInterrupt()
{
    static bool state = false;
    MSerial->led.setState(state);
    state = state ? 0 : 1;
    processSensorsData();
}
