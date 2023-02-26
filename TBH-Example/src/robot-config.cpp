#include "vex.h"

using namespace vex;

// configurations of the hardware
brain Brain;  // 1 brain
motor FlywheelMotorA = motor(PORT3,gearSetting::ratio18_1,false); // a flywheel motor that works together with another flyhweel motor
motor FlywheelMotorB = motor(PORT7, gearSetting::ratio18_1, true);// a flywheel motor that works together with another flyhweel motor
motor_group FlywheelMotors(FlywheelMotorB, FlywheelMotorA);   // Motor group of both flywheels
controller mainController = controller(controllerType::primary); // main controller
rotation flywheelRotationSensor = rotation(PORT10); // rotation sensor to get more accurate readings straight from the flywheel

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  FlywheelMotors.setStopping(brakeType::coast);
  // Nothing to initialize
}