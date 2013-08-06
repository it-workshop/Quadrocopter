#include "Quadrocopter.h"

void Quadrocopter::processSensorsData()
{
    //MPU->iteration();
    angle.parseFloat(MyMPU->getAngleXYZ());
    angularVelocity.parseFloat(MyMPU->getAngularVelocityXYZ());

    voltage = VSensor->getValue();

#ifdef USE_COMPASS
    {
        if(!MyCompass->requested)
        {
            MyCompass->requestHeading();
            MyCompass->readHeading();
        }
        else
        {
            copterHeading = MyCompass->getHeading();
        }

        //it doesn't work
//        Serial.println("compass A");
//        if(!MyCompass->requested)
//        {
//            Serial.println("req");
//            MyCompass->requestRawData();
//        }
//        else if(MyCompass->rawDataReady())
//        {
//            Serial.println("read");
//            MagnetometerRaw raw = MyCompass->readRawAxis();

//            copterHeading = atan2(-raw.XAxis, raw.YAxis);

//            // do not use that
//            //copterHeading = atan2(raw.XAxis, sign(raw.YAxis)*sqrt(sqr(raw.ZAxis) + sqr(raw.YAxis)));

//            if(copterHeading < 0)
//                copterHeading += 2 * PI;

//            if(copterHeading > 2 * PI)
//                copterHeading -= 2 * PI;
//        }
//        Serial.println("compass B");
    }
#endif
}
