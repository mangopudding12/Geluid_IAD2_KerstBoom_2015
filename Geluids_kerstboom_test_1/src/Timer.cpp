#include "Timer.h"

Timer :: Timer (int eindtijd_) 
{
		eindtijd = eindtijd_;
}

void Timer :: start () 
{ 
	savetime = ofGetElapsedTimeMillis(); 
} 


void Timer :: tikking () 
{ 
	passedTime = ofGetElapsedTimeMillis() - savetime;				
} 

