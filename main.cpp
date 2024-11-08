/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// twoBar               motor_group   11, 15          
// IntakeBig            motor         17              
// IntakeFlapping       motor         19              
// Clamp1               digital_out   H               
// Clamp2               digital_out   A               
// Drivetrain           drivetrain    7, 10, 6, 4     
// Controller1          controller                    
// Inertial12           inertial      12              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include <iostream>
#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

void clampIn(){
    Clamp1.set(true);
    Clamp2.set(true);
}

void clampOut(){
    Clamp1.set(false);
    Clamp2.set(false);
}

int handHeight = 0;

void hand(){
    handHeight++;
    handHeight %= 3;

    twoBar.setVelocity(50.0, percent);
    switch(handHeight){
      case 1:
        twoBar.spinToPosition(60.0, degrees, true);
        break;
      case 2:
        twoBar.spinToPosition(120.0, degrees, true);
        break;
      default:
        twoBar.setVelocity(20.0, percent);
        twoBar.spinToPosition(30.0, degrees, true);
        break;
    }

    twoBar.setStopping(hold);
}

enum intakeState{
  STOP,
  FORWARD,
  REVERSE
};

intakeState toggle = STOP;

void intakeIn() {
  if(toggle != STOP)
    toggle = STOP;
  else
    toggle = FORWARD;
}

void intakeOut() {
  if(toggle != STOP)
    toggle = STOP;
  else
    toggle = REVERSE;
}

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

  // stops the coneyor belt
  IntakeBig.stop();
  IntakeFlapping.stop();

  // makes the intake conveyor belt hastier
  IntakeBig.setVelocity(95.0, percent);
  IntakeFlapping.setVelocity(95.0, percent);

  // clamp off
  clampOut();

  // hand down
  handHeight = 2;
  hand();

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
// GUYS ADD THE BUTTONS THINGY REFER TO MAKING A MOTOR GROUP VIDEO 
void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    Controller1.ButtonUp.pressed(clampIn);
    Controller1.ButtonDown.pressed(clampOut);

    Controller1.ButtonR1.pressed(hand);

    Controller1.ButtonL1.pressed(intakeIn);
    Controller1.ButtonL2.pressed(intakeOut);

    if(toggle == STOP){
      IntakeBig.stop();
      IntakeFlapping.stop();
    }
    if(toggle == FORWARD){
      IntakeBig.spin(forward);
      IntakeFlapping.spin(forward);
    }
    if(toggle == REVERSE){
      IntakeBig.spin(reverse);
      IntakeFlapping.spin(reverse);
    }

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // new code i (aka shivika) copied from the user control part 2 video 
    // LDrive_Group.spin(forward, Controller1.Axis3.value(), pct);
    // RDrive_Group.spin(forward, Controller1.Axis2.value(), pct);

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
