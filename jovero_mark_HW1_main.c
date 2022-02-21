/**************************************************************
* Class:  CSC-615
* Name:	  Mark Jovero	
* Student ID:	916691664
* Project:	Assignment 2 - Tapeless Ruler
*
* File: mark_jovero_HW1_main.c
*
* Description: Main method for tapeless ruler.
*
**************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>
#include "echo.h"

#define ECHO 0 // echo pin
#define TRIG 7 // trigger pin

#define MIN_DIST 2
#define MAX_DIST 200

#define V_SOUND 340 // m/s

int main(int argc, char *argv[]) {
	test2();
	
	struct timeval start, stop, timeout;

	int no_read = 0;
	double pulse_time_sec, dist, dist_centi;
	unsigned long elapsed_time;

	printf("STARTING....\n");
	
	wiringPiSetup();
	
	pinMode(TRIG, OUTPUT);
  	pinMode(ECHO, INPUT);

	digitalWrite(TRIG, LOW);
	delay(2000);

	for (;;) {
		
		digitalWrite(TRIG, HIGH);
		delay(0.1);	// 0.1 millis
		digitalWrite(TRIG, LOW);
		
		gettimeofday(&timeout, NULL);	// tracks how long echo pin takes to recieve a signal
	
		while (0 == digitalRead(ECHO)) { 	// wait until echo recieves a signal
				gettimeofday(&start, NULL);	// at which point, the start time will record time it recieves signal
				if (start.tv_usec - timeout.tv_usec < 1000000) {	// 1 second has elapsed, no read, sound never reaches sensor
					no_read = 1;
					break;
				}
			}

		if (no_read == 1) {
			no_read = 0;
			continue;
		}

		while (1 == digitalRead(ECHO)) 	// sensor continues to return HIGH over the duration of pulse
			gettimeofday(&stop, NULL);	// record time when it switches back to LOW 

		elapsed_time = stop.tv_usec - start.tv_usec;		// microseconds
		pulse_time_sec = (double) elapsed_time / 1000000;	// converted from microseconds to seconds
		dist = (pulse_time_sec * V_SOUND) / 2;	// meters
		dist_centi = dist * 100;
		
		if (dist_centi < MIN_DIST || dist_centi > MAX_DIST) {
			printf("ERROR: can only measure distance from %d to %d centimeters.\n", MIN_DIST, MAX_DIST);
			continue;
		}
			
		printf("%.2f\tcm\n",  dist * 100);	
		sleep(1);
	}
	return 0;
}

