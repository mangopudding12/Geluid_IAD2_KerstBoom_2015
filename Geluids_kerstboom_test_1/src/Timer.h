#pragma once 
#include "ofMain.h"

class Timer 
{
public: 
	Timer () { }
	Timer(int eindtijd_); 

	void start();
	void tikking(); 

	// variable 
	float savetime; 
	float passedTime;
	int eindtijd; 

	// boolean 
	bool klaar; 

private: 

}; 