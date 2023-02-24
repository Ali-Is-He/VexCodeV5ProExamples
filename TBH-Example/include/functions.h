//File that has all the functions you plan on using

double power2Volts(int powerValue);   // function for converting "power" values [-127,127] to milivolts
void updateVelTBH();            // function that updates the speed the flywheel should be moving at based on the error.
int flywheelTBHManager();       // function that is running in a separate task to control the flywheel
void flywheelTBHSet(int vel, float predicted_drive);  // Function that sets the desired flywheel speed with a prediction 