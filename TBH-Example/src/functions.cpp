#include "vex.h"
//Function that converts "power" to volts can be used in the TBH if you want to use voltage instead of percent but not required
double power2Volts(double powerVal)
{
  const double MAX_VOLT = 12.00;
  const double MAX_POWER = 100.00;
  
  double volts = (powerVal * MAX_VOLT) / MAX_POWER;

  return volts;
}

// All TBH Variables
double targetVelocity;    // How fast we want to go
double currentVelocity;   // The current speed of the flywheel
double lastVelocity;      // The last measured speed of the flywheel
float velocityError;      // How far off the current speed is from the desired speed
float lastError;          // How far off the last measured speed was from the desired spreed
float gain = 0.0025;      // VAIRABLE YOU WILL BE TUNING, This controls how fast/agressive the TBH algorithm and values that go to your flywheel are
float TBHdrive;           // The output that the TBH recommends you set your motors to
float driveAtZero;        // This is the value your flywheel gets set to if you overshoot the target RPM
long firstCross;          // A boolean that checks if this is the first time the flywheel overshot its target
float driveApprox;        // This is the value your flywheel gets set to if you overshoot the target RPM
double  motorDrive;       // The percent speed that your motor spins based on the outputs from the TBH algorithm
const int GEARRATIO=1;    // Gear Ratio you are using on your flywheel. If you are using a rotation sensor on your flywheel axle keep this at 1, if you are using the built in motor encoders then set this to the gear ratio you use


// function that updates the speed the flywheel should be moving at based on the error.
void updateVelTBH()
{
  /*
    Calculate the error and set the TBHdrive recommendation based on the error, if the TBHdrive recommendation is too big or negative (i.e. telling you to spin backwards) then you cap/limit the 
      recommended value. If the current error is not the same sign as the previous error (meaning we passed the target velocity) then drop the TBHdrive recommendation to the driveApprox prediction
      you originally gave when you called the flywheelSetTBH function. If the flywheel velocity overshoots the target twice then set the TBHdrive recommendation to half the driveApprox prediction 
      Update the last error after every loop so the algorithm knows if its getting closer or not.
  */
  velocityError= targetVelocity - currentVelocity;   // calculate the error 
  TBHdrive = TBHdrive + (velocityError * gain);   // get the TBHdrive recommendation

  if(TBHdrive > 1) // If the recommendation from the TBH is too high, cap it at 1
  {
    TBHdrive =1;
  }
  if (TBHdrive <0) // if the recommendation from the TBH tells you to go backwards, cap it at 0
  {
    TBHdrive=0;
  }
  if(__signbitf(velocityError) != __signbitf(lastError) ) // If we are passing the target speed then adjust the TBHdrive recommendation based on if this is the 1st time crossing or not
  {
    if (firstCross) // if this is the first time crossing the target speed then set the TBHdrive to the approximation we made
    {
      TBHdrive = driveApprox;
      firstCross =0;
    }
    else // If this is the 2nd time crossing or more, set the target speed to half the approximation we made last time
    {
      TBHdrive = 0.5 * (TBHdrive + driveAtZero);
    }
    driveAtZero=TBHdrive; //save the value we set the TBHdrive recommendation to
  }
  lastError = velocityError; // the error from the last time we updated the TBHdrive recommendation

}

// The function that is running in a separate task to control the flywheel
int flywheelTBHManager()
{
  int count=0; // count for debugging purposes. Should print on your brain screen every time the function runs. This proves the function is running in the background

  /* 
    while true, Get the current velocity, update the velocity the flywheel should be running at based on the error, and if the number the flywheel should be running at
      is too big, then limit it to the max value.
  */
  while(true)
  {
    currentVelocity=FlywheelMotors.velocity(velocityUnits::rpm)*GEARRATIO;  // Get the speed of the flywheel
    mainController.Screen.print("velocity %.2f",currentVelocity);   //Print out flywheel speed for debugging and so you can get a feel for how well the rpm is being managed
    mainController.Screen.setCursor(0,0);
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1,1);
    Brain.Screen.print("count: %d", count); // print the number of times the function has run on the brain so we know the function is doing work 

    updateVelTBH(); // Calculate the speed the flywheel should be running at based on the error
    
    motorDrive= (TBHdrive * 100) +0.5; // scale up the TBHDrive recommendation by multuplying it by 100 and then add 0.5 for friction

    if (motorDrive >100)  // If your scaled up TBHDrive is bigger than the max, cap it at the max
    {
      motorDrive =100;
    }
    if (motorDrive< 0) // if you scaled up TBHDrive is less than the minimum, cap it at the min
    {
      motorDrive= 0;
    }

    FlywheelMotors.spin(fwd,motorDrive, percentUnits::pct); //spin the motor at the scaled percent
    wait(25,msec);
    count++;
  }
  return 0;
}


// Function that sets the target velocity and predicted_drive for the TBH alg
void flywheelTBHSet(int vel, float predicted_drive)
{
  // Set the variables that the TBH uses
  targetVelocity = vel;  // Set the target velocity
  velocityError = targetVelocity - currentVelocity; // calculate the error in velocity
  lastError = velocityError;  //udpate the last velocity error we had

  driveApprox = predicted_drive;  //Set the predicted drive (this is what the flywheel will turn at if it overshoots the target)

  firstCross = 1; // sets the first cross to true (meaning if the flywheel overshoots it will know this is the first time it overshot)
  driveAtZero =0; // sets the driveAtZero to 0 to clear out whatever the last value was (this prevents us from using the value from the previous flywheelSet on accident)

}