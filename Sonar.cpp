#include "Sonar.h"
#include <Arduino.h>


Sonar::Sonar(int pinT, int pinE){
  pinTrigger = pinT;
  pinEcho = pinE;
}

int Sonar::init(){
  pinMode(pinTrigger, OUTPUT);
  digitalWrite(pinTrigger, LOW);
  pinMode(pinEcho, INPUT);

  if (digitalPinToInterrupt(pinEcho) == -1){
    return 1;
  }
  attachInterrupt(digitalPinToInterrupt(pinEcho), handleEcho, CHANGE );
  return 0;
}

void Sonar::handleEcho_impl(){
  if(digitalRead(pinEcho))
	setState(ECHO_RISE);
  else
    setState(ECHO_FALL);
}

void Sonar::handleEcho() {
  auto sonar = getInstance();
  sonar->handleEcho_impl();
}

Sonar * Sonar::getInstance(int pinT, int pinE){
  static Sonar * instance = NULL;
  if (instance == NULL){
    instance = new Sonar(pinT, pinE);
  }
  return instance;
}

void Sonar::setState(State_t s) {
	interruptsGuard ig;
	int duration;
	
	auto t = micros();
	
	if(state == PULSE_SENT && s == ECHO_RISE){
      rising = t;
      state = ECHO_RISE;
      return;
	}
	
	if(state == ECHO_RISE && s == ECHO_FALL){
      duration = (t - rising);
      distance = duration / 58;
      state = ECHO_FALL;
      /*if(distance < 1){
		  distance = -1;
		  //state = INVALID;
	  }*/
      return;
	}
	
	if(s == PULSE_SENT){
		if(/*state == INVALID ||*/ state == ECHO_FALL || state == PULSE_SENT){
		  state = PULSE_SENT;
		  digitalWrite(pinTrigger, HIGH);
		  delayMicroseconds(10);
		  digitalWrite(pinTrigger, LOW);
		  return;
	    }
	    if ((t - rising) >300) {
			state = ECHO_FALL;
			distance = -1;
			return;
		}
	}
	
	//state = INVALID;
	distance = -1;
	return;
}

void Sonar::sendTrig() {
  setState(PULSE_SENT);
}

int Sonar::getDistance() {
  return (int)distance;
}
