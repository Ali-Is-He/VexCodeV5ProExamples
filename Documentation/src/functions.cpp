#include "vex.h"

//function for intaking until the sensor sees a ball
void intakeTilBall()
{
  while(ballDetectIntake.value(analogUnits::pct) <75)
  {
    LowerIntake.spin(directionType::fwd, 85, percentUnits::pct);
    UpperIntake.spin(directionType::fwd, 75, velocityUnits::rpm);
  }
    LowerIntake.spin(directionType::fwd, 0, percentUnits::pct);
    UpperIntake.spin(directionType::fwd, 0, velocityUnits::rpm);
}

//Whole red Auton
void redAuton()
{
  smartbase.driveFor(20, distanceUnits::in, 100, velocityUnits::pct,false);
  intakeTilBall();
  smartbase.turnToRotation(90, rotationUnits::deg, 90, velocityUnits::pct);
}


//The whole skills auto
void skills()
{

}

//example how to write a function that gives back a number
int calculatePain(int howManyTime)
{
  return 0;
}