
#pragma once
#include "ofMain.h"
#include "Timer.h"

#define GELUIDEN 9
#define buttons 9

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit(); 
		ofImage afbeelding; 
		
		int fasevolumezachterzetten[GELUIDEN]; 
		float volumeisnu[GELUIDEN];
		bool geluidblijftzo;

		ofSoundPlayer voicePlayer[GELUIDEN];
		bool kerst_aan_zetten[GELUIDEN];

		
		bool arduino_opstarten; 
		ofArduino myArduino; 
		float volume[buttons]; 
		
	
		int fase[buttons]; 
		Timer tijd[buttons];
		bool tijdis_gestart[buttons];

	private: 
 		void setupArduino(const int & version); 
 		void digitalPinChanged(const int & pinNum); 
};
