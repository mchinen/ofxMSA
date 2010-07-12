#include "ofxSimpleGuiTextInput.h"


ofxSimpleGuiTextInput::ofxSimpleGuiTextInput(string name, string &value) : ofxSimpleGuiControl(name) {
	this->value	= &value;
	controlType = "TextInput";
	setup();
}

void ofxSimpleGuiTextInput::setup() {
	setSize(config->gridSize.x - config->padding.x, config->buttonHeight);
}

void ofxSimpleGuiTextInput::loadFromXML(ofxXmlSettings &XML) {
	set(XML.getValue("controls:" + controlType + "_" + key + ":value", ":value"));
}

void ofxSimpleGuiTextInput::saveToXML(ofxXmlSettings &XML) {
	XML.addTag(controlType + "_" + key);
	XML.pushTag(controlType + "_" + key);
	XML.addValue("name", name);
	XML.addValue("value", getValue());
	XML.popTag();
}

void ofxSimpleGuiTextInput::keyPressed( int key ) {
   if(key >= 32 || key == '\n') {
      *value += key;
   }
}


string ofxSimpleGuiTextInput::getValue() {
	return (*value);
}

void ofxSimpleGuiTextInput::set(string b) {
	(*value) = b;
}

void ofxSimpleGuiTextInput::onPress(int x, int y, int button) {
   focused = true;
}

void ofxSimpleGuiTextInput::onRelease(int x, int y, int button) {
}

void ofxSimpleGuiTextInput::draw(float x, float y) {
	setPos(x, y);
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	ofEnableAlphaBlending();
	ofFill();
	setTextBGColor();
	ofRect(0, 0, width, height);
	
	setTextColor();
	ofDrawBitmapString(name, 3, 15);
   
   ofDrawBitmapString(*value, 3, 15 + 20);
	
	ofDisableAlphaBlending();
	
	glPopMatrix();
}
