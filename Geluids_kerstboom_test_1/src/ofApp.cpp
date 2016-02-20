

// Nog te doen: 

// wil een knop dat je kan beslissen dat je op dit volume minsten 2 min wilt lopen + die liedjes die op dat moment hebt
// Wanneer je weer op de knoppen gaat drukken wordt het verbroken. 




#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	afbeelding.loadImage("sneeuw3.jpg");

	// Geluiden inladen 
		for (int i = 0; i < GELUIDEN; i++)
		{
			ofLogNotice() << "Loading voice" << i << endl; 
			voicePlayer[i].loadSound("geluid" + ofToString(i+1)+".mp3"); 
			voicePlayer[i].setLoop(true);

			// Alle geluiden staan uit
			kerst_aan_zetten[i] = false;

			fase[i] = 0; 
			volume[i] = 0;
			voicePlayer[i].setVolume(volume[i]);	
			fasevolumezachterzetten[i] = 0;
			volumeisnu[i] = 0;
		} 


	// Looping functie is false 
		 geluidblijftzo = false; 

	// Arduino 
		ofAddListener(myArduino.EInitialized, this, &ofApp::setupArduino); 
		arduino_opstarten = false; 
		myArduino.connect("COM3", 57600); // aan deze poort zit de arduino 
		myArduino.sendFirmwareVersionRequest();

}


void ofApp::setupArduino(const int& version) 
{
	ofRemoveListener(myArduino.EInitialized, this, &ofApp::setupArduino);
	arduino_opstarten = true; 

	for (int i = 0; i < buttons; i++)
	{
			myArduino.sendDigitalPinMode(i+2, ARD_INPUT);
			cout << i+2 << endl;
	}

			// looping knop
			myArduino.sendDigitalPinMode(11, ARD_INPUT);
			cout << "Looping knop 11" << endl;

	ofAddListener(myArduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
}



//--------------------------------------------------------------
void ofApp::update()
{
	myArduino.update();


	// Zolang de looping knop niet gedrukt is wordt het geluid gewoon zachter 
	if (geluidblijftzo == false)
	{
		for (int i = 0; i < GELUIDEN; i++)
		{

			if (tijdis_gestart[i] == true)
			{

				if (tijd[i].passedTime >= tijd[i].eindtijd/4)
				{	
					if (fasevolumezachterzetten[i] == 0 )
					{
						volumeisnu[i] = voicePlayer[i].getVolume(); 
						fasevolumezachterzetten[i] = 1; 
					}

					if (volume[i] > (volumeisnu[i]/4)*3) // volumeisnu = 100%  die moet groter zijn 75% dat is die dus gaat er 25% af 
					{
						volume[i] -= 0.1; 
						voicePlayer[i].setVolume(volume[i]); 
					}
				}  
		


			    if (tijd[i].passedTime >= tijd[i].eindtijd/2)
				{				
					fasevolumezachterzetten[i] = 2; 

					if (volume[i] > volumeisnu[i]/2) // volumeisnu = 75% die moet groter zijn dan 50% dat is die dus gaat er 25% af  
					{
						volume[i] -= 0.1; 
						voicePlayer[i].setVolume(volume[i]); 
					}  	
				}


			    if (tijd[i].passedTime >= (tijd[i].eindtijd/4)*3)
				{
					fasevolumezachterzetten[i] = 3; 

					if (volume[i] > volumeisnu[i]/4)  // volumeisnu = 50% die moet groter zijn dan 25% dat is die dus gaat er 25% af  
					{
						volume[i] -= 0.1; 
						voicePlayer[i].setVolume(volume[i]); 
					}  
				}

				
				if (tijd[i].passedTime >= tijd[i].eindtijd)
				{
					fasevolumezachterzetten[i] = 4; 

					if (volume[i] > 0.1) // gaat naar 0 
					{
						volume[i] -= 0.1; 
						voicePlayer[i].setVolume(volume[i]); 
					}  

				} else { 
					tijd[i].tikking(); 
				}

			} // end if statemant
		} // end for loop 
	}
	
	ofSoundUpdate();




}


void ofApp::digitalPinChanged(const int& pinNum) 
{

	if (myArduino.getDigital(11) == ARD_HIGH)
	{
		geluidblijftzo = true; 
		cout << "Looping functie is aan" << endl; 
	}


	for (int i = 0; i < buttons; i++)
	{
		
		if (myArduino.getDigital(i+2) == ARD_HIGH)
		{

			if (kerst_aan_zetten[i] == false)
			{
				voicePlayer[i].play();
				cout << i << endl;
				kerst_aan_zetten[i] = true; 
			}
			

			if (fase[i] == 0)
			{
				if (voicePlayer[i].getVolume() <= 1)
				{
					volume[i] += 0.1; 
					voicePlayer[i].setVolume(volume[i]);
				}
			
				tijd[i] = Timer (60000); // 1 min
				tijd[i].start();
				tijdis_gestart[i] = true; 
				fase[i] = 1; 
				fasevolumezachterzetten[i] = 0; 

				if (geluidblijftzo == true)
				{
					for (int i = 0; i < buttons; i++)
					{
						
							tijd[i].start();
							tijd[i].savetime += (i * ofRandom(0, tijd[i].eindtijd/8))- (i*10); 
					}
						geluidblijftzo = false; // wanneer er op andere knopt wordt gedrukt verbreekt dat het looping functie
				}
			}

		}  else if (myArduino.getDigital(i+2) == ARD_LOW) { 

			fase[i] = 0; 
		}
	}
}



//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255,255,255);
	afbeelding.draw(0,0,ofGetWidth(),ofGetHeight());
	
	for (int i = 0; i < GELUIDEN; i++)
	{
			ofSetColor(128+i*20,i*80,i*10+60);
			ofRect(85,i*60+150,voicePlayer[i].getVolume()*850,50);
		
	}
}



void ofApp:: exit() 
{
	myArduino.disconnect();
}