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
  smartbase.turnToRotation(45, rotationUnits::deg, 90, velocityUnits::pct);
}

void blueAuton()
{

}

int countballs()
{
  int numBalls=0;
  if(ballDetectIntake.value(percentUnits::pct) == 100)
  {
    numBalls = numBalls +1;
  }
  //if sensor sees a ball, add 1
  return numBalls;
}

//The whole skills auto
void skills()
{

}

//example how to write a function that gives back a number -- Credit David Choi from 2145Z Pink Sparklee Unicorns
int calculatePain(int howManyTimes)
{
  int pain= 0;
  for(int i=0; i<howManyTimes; i++)
  {
    pain +=1;
  }
  return pain;
}

 void togglePiston()
 {
   clawPiston.set(!clawPiston.value());
   wait(500, msec);
 }

//Function that converts "power" to volts
double power2Volts(double powerVal)
{
  const double MAX_VOLT = 12.00;
  const double MAX_POWER = 127.00;
  
  double volts = (powerVal * MAX_VOLT) / MAX_POWER;

  return volts;
}

//example function for flywheel PID
void flywheelPID(double target)
 {
  // Constants
  const double MAX_FLYWHEEL_RPM = 3000.00; // Set this to whatever the maximum RPM your flywhee is going to run
  const double MAX_VELOCITY = 100.00; // This is the max velocity/output you are going to set.
  const double kP = 0.3;//.1 This is a value you tune though trial and error
  const double kV = MAX_VELOCITY/MAX_FLYWHEEL_RPM; //feed forward constant is tpyically equal to max velocity/max flywheel rpm
  const double threshold = 350.00; // The threshold of when you want your PID to work. You can play with this a little bit to make it smaller or bigger
                                  // too small means your PID will never start or it wont be active very often cuz most of the time your flywheel will be at 127 or 0 power 
                                  //too big = PID running all the time and might have trouble zeroing to the right speed. 
  const double gearRatio = 5.00; // set this to the gear ratio between your motor and the flywheel.  
  const double marginOfError =15.00; // set this to the margin of error you are ok shooting with. (i.e if your margin of error is 10 and the target is 500, then you will shoot at anywhere from 490 to 510 rpm)
  double error = 0.00;  // The variable that will hold how far off your current RPM is from the target RPM
  double prevError = 0.00; // the variable that will hold how far your RPM was off last time you checked

  double output = 0.00;   // Variable that holds the output that you are setting the motor too

  //reset sensors


  while (true) {

    // Proportional
    error = target - launcher.velocity(velocityUnits::rpm)*gearRatio;
    // Set speed of flywheel to max if the error is bigger than your threshold, if the error is <-threshold (i.e your flywheel rpm is faster than the target) then set the flywheel to 0, otherwise, use the PID to control the RPM
    if (error > threshold){
      output = 127.00;
    }
    else if (error < -threshold){
      output =0.00;
    }
    else{
      output = (kV * target) + (kP * error);
    }

    // if the output is too big (bigger than the max of 127.00) then set it to the max of 127.00, if the output is negative then make it 0
    
    if(output > 127.00){
      output = 127.00;
    }
    else if(output < 0.00){
      output = 0.00; 
    }
    //Convert power to voltage and contorl the motor with that
    launcher.spin(directionType::fwd, power2Volts(output), voltageUnits::volt);
    mainController.Screen.print(launcher.velocity(velocityUnits::rpm)*gearRatio);
    mainController.Screen.setCursor(0, 0);
    //If the current error is within the acceptable margin of error, then fire the disc
    if (error < marginOfError && error > -marginOfError)
    {
      //This is where you would tell your indexer to push the disc into the flywheel, for the sake of demo this currently just does a print of the error
      mainController.Screen.print(error);
      mainController.Screen.print("test1");
    }

    //printf("flywheel: %f", flywheel.get_actual_velocity()*gearRatio);
    //printf("\n");

    //set the previous error to the current error
    prevError = error;

    //Wait a little bit to update the values and run the loop again
    wait(5, timeUnits::msec);

  }
  
 }
/// all variables for TBH
long targetVelocity;
long currentVelocity; 
long lastVelocity;
float error;
float last_error;
float gain;
float Drive; 
float driveAtZero;
long firstCross;
float drive_approx;
long motor_drive;

// function that sets flywheel speed
void setFlywheelMotor(double powerVal)
{
  double flywheelSpeed = power2Volts( powerVal);
  launcher.spin(fwd,flywheelSpeed, voltageUnits::mV);
}

void updateFlywheelVelocity()
{
  error= targetVelocity-currentVelocity;
  Drive= Drive + (error *gain);
  if (Drive >1)
  {
    Drive=1;
  }
  if (Drive<0)
  {
    Drive=0;
  }

  //check for zero crossing
  if (__signbitf(error) != __signbitf(last_error))
  {
    if (firstCross)
    {
      Drive = drive_approx;
      firstCross=0;
    }
    else
    {
      Drive = (0.5 * Drive + driveAtZero);
    }
    driveAtZero = Drive;
  }
  last_error = error;
}

void flywheelTBH(double target)
{
  gain= 0.00025;
  while (true)
  {
    currentVelocity = launcher.velocity(velocityUnits::rpm)*6;
    updateFlywheelVelocity();
    motor_drive = (Drive*127.00) +0.5;

    if(motor_drive >127)
    {
      motor_drive =127;
    }
    if (motor_drive <-127)
    {
      motor_drive=-127;
    }
    setFlywheelMotor(motor_drive);
  }
}
