#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>


class Sonar {
  public:
  
	class interruptsGuard{
	public:
		interruptsGuard(){
			noInterrupts();
		}
		~interruptsGuard(){
			interrupts();
		}
	};

    typedef enum {
       INVALID,
       PULSE_SENT,
       ECHO_RISE,
       ECHO_FALL,
       TIMEOUT
    }State_t;

    int init();
    int getDistance();
    void sendTrig();
    static Sonar* getInstance(int pinT = 0, int pinE = 0);

  protected:
    static void handleEcho();
    void setState(State_t s);
    int getState();
    void handleEcho_impl();

    Sonar(int pinT, int pinE);
    volatile int rising = 0;
    volatile State_t state = ECHO_FALL;
    int pinTrigger;
    int pinEcho;
    volatile int distance = -1;
    
};

void handleEcho();
void setState(int s);
int getState();
int loopSonar();

#endif //SONAR_H
