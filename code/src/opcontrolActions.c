#include "main.h"

void opcontrolTankDrive(){
  tankDrive(abs(joystickGetAnalog(1,3))>20 ? joystickGetAnalog(1,3) : 0,
          abs(joystickGetAnalog(1,2))>20 ? joystickGetAnalog(1,2) : 0);
}

void opcontrolMobileGoal(){
  int speed;
/*
  if(abs(encoderGet(encoderChainB)>20){   //test and recheck for values
    (//maybe speed = 20)
    target = 50; //check values
  
  }
  else{ 
  */

    if(joystickGetDigital(1, 5, JOY_UP)){
      speed = 75;
    }
    else if(joystickGetDigital(1, 5, JOY_DOWN)){
      speed = -75;
    }
    else if(joystickGetDigital(1, 6, JOY_UP)){
      speed = 100;
    }
    else if(joystickGetDigital(1, 6, JOY_DOWN)){
      speed = -100;
    }
    else {
      speed = 0;
    }  
  mobileGoal(speed);
}

void opcontrolLinearGear(){   //will be reconfigured for cascade lift
  int speed;
  if(joystickGetDigital(2, 6, JOY_UP)){
    speed = 127;
  }
  else if(joystickGetDigital(2, 6, JOY_DOWN)){
    speed = -127;
  }
  else {
    speed = 0;
  }
  linearGear(speed);
}

void opcontrolConeGrabber(){
  int speed;
  if(joystickGetDigital(2, 7, JOY_DOWN)){
    speed = 90;
  }
  else if(joystickGetDigital(2, 8, JOY_DOWN)){
    speed = -90;
  }
  else {
    speed = 0;
  }
  coneGrabber(speed);
}


void opcontrolChainBar(){
   const int positionVertical = 0; //needs to be changed/adjusted
   int speed;
   int target;
   int current = encoderGet(encoderChainB);
   
   if((current > positionVertical && (joystickGetDigital(2, 5, JOY_DOWN) || target > current)) || (current < positionVertical && joystickGetDigital(2, 5, JOY_UP))){
      

   }
   else {
     
   }
   

  


   
   if(joystickGetDigital(2, 5, JOY_UP)){
     //target = current;
     speed = 75;
   }
   else if(joystickGetDigital(2, 5, JOY_DOWN)){
     //target = current;
     speed = -75;
   } 
   else if(joystickGetDigital(2, 8, JOY_LEFT)){                                           
     speed = -min(60, max(-60, PID(current, -60, 1, 0.8, 0, 0)));    //holds chainbar right above mobile goal lifter
     //target = current;
   }  
   else if(joystickGetDigital(2, 8, JOY_RIGHT)){
     speed = -min(60, max(-60, PID(current, -180, 1, 0.8, 0, 0)));  //holds chainbar to driver load level
   }
   else {
    speed=0;
    //chainbarPID(current, cbTarget, 0.7, 0.7);
   } 
   chainBar(speed);
 }

void opcontrolDebug(){
  if(joystickGetDigital(2, 7, JOY_UP)){
  encoderReset(encoderChainB);
  encoderReset(leftDriveShaft);
  encoderReset(rightDriveShaft);
  encoderReset(shaftLinearGear);
  }
}

static float linearGearStack[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //these two arrays need to be updated with real target values
static float chainBarStack[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //each float in the array corresponds to the number of cones in stack
int coneCount = 0;                                                //each float in the array is a target value for each mechanism
void opcontrolStack(){
	int x = 0;
	int chainbarSpeed;
	int linearGearSpeed;
	if(joystickGetDigital(1, 7, JOY_UP)){ //reset cone stack
		coneCount = 0;
	}
	else if(joystickGetDigital(1, 7, JOY_LEFT)){ //hold this to auto stack
		chainbarSpeed = -min(60, max(-60, PID(current, -180, 1, 0.8, 0, 0)));
		while (smartMotorGet(MOTORS_CHAINB) > 20) {
			chainbarSpeed = -min(60, max(-60, PID(current, -180, 1, 0.8, 0, 0)));
		}
		//coneGrabber(); this needs speed to pick up cone
		coneCount += 1;
	}
	else if (joystickGetDigital(1, 7, JOY_DOWN)) { //remove a cone from stack (need to find a way to only happen once for one button press
		for (x = 0; x < 1;  x += 1) {
			coneCount -= 1;
		}
	}
	linearGear(linearGearSpeed);
	chainBar(chainbarSpeed);
}



