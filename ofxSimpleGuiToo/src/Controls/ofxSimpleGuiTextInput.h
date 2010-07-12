#pragma once

#include "ofxSimpleGuiControl.h"


class ofxSimpleGuiTextInput : public ofxSimpleGuiControl {
public:
   string*      value;
		
	ofxSimpleGuiTextInput(string name, string &value);	
	void setup();
	void loadFromXML(ofxXmlSettings &XML);	
	void saveToXML(ofxXmlSettings &XML);
	void keyPressed( int key );
	string getValue();
	void set(string b);

	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void draw(float x, float y);
};
