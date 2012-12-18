#include "Quadrocopter.h"
#include "ComplementaryFilter.cpp"
#include "LowPassFilter.cpp"

void Quadrocopter::processSensorsData()
{
    static RVector3D accelAngle, gyroCosines;

    if(reactionType == ReactionAngle || reactionType == ReactionAcceleration)
        accelDataRaw = Accel->getReadings();
    else accelDataRaw = RVector3D();

    if(reactionType != ReactionNone)
        angularVelocity = Gyro->getReadings();
    else angularVelocity = RVector3D();

    if(reactionType == ReactionAngle)
    {
        if(DeltaT.getTimeIsset())
        {
            /*angularAcceleration = (angularVelocity - angularVelocityPrev) / dt;
            accelData.x -= (angularAcceleration.y * gyroToAcc.z - angularAcceleration.z * gyroToAcc.y) / g;
            accelData.y -= (angularAcceleration.z * gyroToAcc.x - angularAcceleration.x * gyroToAcc.z) / g;
            accelData.z -= (angularAcceleration.x * gyroToAcc.y - angularAcceleration.y * gyroToAcc.x) / g;*/

            // calculate fast gyro estimate
            gyroCosines = directionalCosines.getValue();
            gyroCosines += (gyroCosines ^ angularVelocity) * dt; // v = [-w, r] = [r, w]

            // calculate slow accel estimate
            accelData.iteration(accelDataRaw, dt);

            // complementary filter
            directionalCosines.iteration(gyroCosines, accelData.getValue().normalize(), dt);

            // converting value to 3 angles
            angle = directionalCosines.getValue().angleFromProjections();

//            // sometimes some stuff happens
//            for(unsigned i = 0; i < 2; i++)
//            {
//                if(fabs(angle.valueByAxisIndex(i)) > angleMaxReset)
//                {
//                    angle.valueByAxisIndex(i) = accelAngle.valueByAxisIndex(i) > 0 ?
//                                fabs(angle.valueByAxisIndex(i)) : -fabs(angle.valueByAxisIndex(i));
//                }

//                if (!(angle.valueByAxisIndex(i) >= -MPI && angle.valueByAxisIndex(i) <= MPI))
//                    angle.valueByAxisIndex(i) = 0;
//            }
        }

        angularVelocityPrev = angularVelocity;
    }

    voltage = VSensor->getValue();
}
