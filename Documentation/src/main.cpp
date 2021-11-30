/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Afnan                                                     */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description: Documentation                                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

// This file has all the code you will be using for auton and driver controll 

#include "vex.h" // Including this file gets all the libraries from the vex.h and the hardware we defined in robot-config.h and included in vex.h. Thats how the harware definitions get to this file

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  redAuton();
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    ////// Set the base to the values from the joystick for tank control
    leftDrive.spin(directionType::fwd, mainController.Axis1.value(), percentUnits::pct);
    rightDrive.spin(directionType::fwd, mainController.Axis2.value(), percentUnits::pct);

    // If button A is pressed, then power the launcher
    if(mainController.ButtonA.pressing())
    {
      launcher.spin(directionType::fwd, 100, percentUnits::pct);
    } 
    else{
      launcher.spin(directionType::fwd, 0, percentUnits::pct);
    }

    // If B is pressed intake, if X is pressed outtake, if nothing is pressed and the ballDetector line follower doesnt see a ball, then we intake, other wise we do nothing
    if(secondaryController.ButtonB.pressing())
    {
      UpperIntake.spin(directionType::fwd, 100, percentUnits::pct);
    }
    else if(secondaryController.ButtonX.pressing())
    {
      UpperIntake.spin(directionType::rev, 100, percentUnits::pct);
    }
    else if(ballDetectIntake.value(analogUnits::pct) < 75)
    {
      UpperIntake.spin(directionType::fwd, 50, percentUnits::pct);
      secondaryController.Screen.print("NO BALL FOUND :(");
    }
    else
    {
      UpperIntake.spin(directionType::fwd, 0, percentUnits::pct);
      secondaryController.Screen.print("WE GOT A BALL");
    }

    if(!launcherPullbackDetector.pressing())
    {
      launcher.spin(directionType::fwd, 100, velocityUnits::pct);
    }

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}