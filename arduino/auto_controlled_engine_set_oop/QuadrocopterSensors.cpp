

#include "Quadrocopter.h"

void Quadrocopter::processSensorsData()
{
    if(reactionType == ReactionAngle || reactionType == ReactionAcceleration)
        accelData = Accel->getReadings();
    else accelData = RVector3D();

    if(reactionType != ReactionNone)
    {
        //angular velocity low-pass filter
        double angularVelocityAlpha = dt / (dt + gyroPeriod / (2 * MPI));
        angularVelocity = angularVelocity * (1 - angularVelocityAlpha) + Gyro->getReadings() * angularVelocityAlpha;
    }
    else angularVelocity = RVector3D();

    if(reactionType == ReactionAngle)
    {
        if(DeltaT.getTimeIsset())
        {
            // alpha coefficient for low-pass filter
            double angleAlpha = dt / (dt + anglePeriod / (2 * MPI));
            double accelAlpha = dt / (dt + accelPeriod / (2 * MPI));

            /*angularAcceleration = (angularVelocity - angularVelocityPrev) / dt;
            accelData.x -= (angularAcceleration.y * gyroToAcc.z - angularAcceleration.z * gyroToAcc.y) / g;
            accelData.y -= (angularAcceleration.z * gyroToAcc.x - angularAcceleration.x * gyroToAcc.z) / g;
            accelData.z -= (angularAcceleration.x * gyroToAcc.y - angularAcceleration.y * gyroToAcc.x) / g;*/

            accelDataFiltered = accelDataFiltered * (1 - accelAlpha) + accelData * accelAlpha;

            RVector3D accelAngle = accelDataFiltered.angleFromProjections();

            RVector3D gyroCosines = directionalCosines;
            gyroCosines += (gyroCosines ^ angularVelocity) * dt; // v = [-w, r] = [r, w]

            // low-pass filter
            directionalCosines = gyroCosines * (1 - angleAlpha) + accelDataFiltered * (angleAlpha / accelDataFiltered.module());

            angle = directionalCosines.angleFromProjections();

            // sometimes some stuff happen
            for(unsigned i = 0; i < 2; i++)
            {
                if(fabs(angle.valueByAxisIndex(i)) > angleMaxReset)
                {
                    angle.valueByAxisIndex(i) = accelAngle.valueByAxisIndex(i) > 0 ?
                        fabs(angle.valueByAxisIndex(i)) : -fabs(angle.valueByAxisIndex(i));
                }

                if (!(angle.valueByAxisIndex(i) >= -MPI && angle.valueByAxisIndex(i) <= MPI))
                    angle.valueByAxisIndex(i) = 0;

            }
        }

        angularVelocityPrev = angularVelocity;
    }

    voltage = VSensor->getValue();
}
