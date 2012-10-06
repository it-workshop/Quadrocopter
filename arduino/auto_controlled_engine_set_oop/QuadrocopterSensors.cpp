#include "Quadrocopter.h"

void Quadrocopter::processSensorsData()
{
    if(reactionType == ReactionAngle || reactionType == ReactionAcceleration)
        accelData = Accel->getReadings();
    else accelData = RVector3D();

    if(reactionType != ReactionNone)
    {
        //angular velocity low-pass filter
        double angularVelocityAlpha = dt / (dt + angularVelocityPeriod / (2 * MPI));
        angularVelocity = angularVelocity * (1 - angularVelocityAlpha) + Gyro->getReadings() * angularVelocityAlpha;
    }
    else angularVelocity = RVector3D();

    if(reactionType == ReactionAngle)
    {
        if(DeltaT.getTimeIsset())
        {
            /*angularAcceleration = (angularVelocity - angularVelocityPrev) / dt;
            accelData.x -= (angularAcceleration.y * gyroToAcc.z - angularAcceleration.z * gyroToAcc.y) / g;
            accelData.y -= (angularAcceleration.z * gyroToAcc.x - angularAcceleration.x * gyroToAcc.z) / g;
            accelData.z -= (angularAcceleration.x * gyroToAcc.y - angularAcceleration.y * gyroToAcc.x) / g;*/

            RVector3D accelAngle = accelData.angleFromProjections();

            // alpha coefficient for low-pass filter
            double angleAlpha = dt / (dt + anglePeriod / (2 * MPI));

            // low-pass filter
            angle.x = (angle.x + angularVelocity.x * dt) * (1 - angleAlpha) + accelAngle.x * angleAlpha;
            angle.y = (angle.y + angularVelocity.y * dt) * (1 - angleAlpha) + accelAngle.y * angleAlpha;


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
