/*
 *  ofxWebSimpleGuiToo.cpp
 *  ofxWebSimpleGuiTooExample
 *
 *  Created by Marek Bereza on 27/03/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ofxWebSimpleGuiToo.h"



ofxWebSimpleGuiToo::ofxWebSimpleGuiToo() {
}
void ofxWebSimpleGuiToo::startServer(int port) {
	server.start("ofxWebSimpleGuiToo", port);
	server.addHandler(this, "/control");
}

void ofxWebSimpleGuiToo::stopServer(){
	server.stop();
}
void ofxWebSimpleGuiToo::httpGet(string url) {
	
	string key = getRequestParameter("key");
	if(key=="") {
		string guiDesc = "[";
		for(int i = 0; i < pages.size(); i++) {
			guiDesc += describePage(pages[i]);
			if(i+1<pages.size()) {
				guiDesc += ",";
			}
		}
		guiDesc += "]";
		httpResponse(guiDesc);
	} else {

		string value = getRequestParameter("value");
		printf("value received: %s = %s\n", key.c_str(), value.c_str());
		ofxSimpleGuiControl *control = findControlByName(key);
		if(control!=NULL) {
			if(control->controlType=="Toggle") {
				ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)control;
				*(t->value) = (value=="true");
			} else if(control->controlType=="SliderInt") {
				ofxSimpleGuiSliderInt *t = (ofxSimpleGuiSliderInt*)control;
				*(t->value) = atoi(value.c_str());
			} else if(control->controlType=="SliderFloat") {
				ofxSimpleGuiSliderFloat *t = (ofxSimpleGuiSliderFloat*)control;
				*(t->value) = atof(value.c_str());
			} else if(control->controlType=="Button") {
				ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)control;
//				t->onPress(0, 0, 0);
//				t->onRelease(0, 0, 0);
				*(t->value) = true;
// 				printf("Hello here!!\n");
			}
		}
		httpResponse("thanks");
	}
}
ofxSimpleGuiControl *ofxWebSimpleGuiToo::findControlByName(string name) {
	for(int i = 0; i < pages.size(); i++) {
		for(int j = 0; j < pages[i]->getControls().size(); j++) {
			if(name==pages[i]->getControls()[j]->name) {
				return pages[i]->getControls()[j];
			}
		}
	}
	return NULL;
}

string ofxWebSimpleGuiToo::describePage(ofxSimpleGuiPage *page) {
	string desc = "{\"name\": \""+page->name+"\", \"controls\":[";
	for(int i = 0; i < page->getControls().size(); i++) {
		desc += describeControl(page->getControls()[i]);
		if(i+1<page->getControls().size()) {
			desc += ",";
		}
	}
	desc += "]}";
	return desc;
}
string ofxWebSimpleGuiToo::describeControl(ofxSimpleGuiControl *control) {
	string desc = "{\"name\":\""+control->name + "\",";
	desc += "\"type\": \""+control->controlType+"\"";
	
	if(control->controlType=="Toggle") {
		ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)control;
		if(*t->value) desc += ", \"value\":\"true\"";
		else desc += ", \"value\":\"false\"";
	} else if(control->controlType=="SliderInt") {
		ofxSimpleGuiSliderInt *t = (ofxSimpleGuiSliderInt*)control;
		desc += ", \"value\":\""+ofToString(*(t->value))+"\"";
		desc += ", \"min\":\""+ofToString(t->min)+"\"";
		desc += ", \"max\":\""+ofToString(t->max)+"\"";
		
	} else if(control->controlType=="SliderFloat") {
		ofxSimpleGuiSliderFloat *t = (ofxSimpleGuiSliderFloat*)control;
		desc += ", \"value\":\""+ofToString(*(t->value))+"\"";
		desc += ", \"min\":\""+ofToString(t->min)+"\"";
		desc += ", \"max\":\""+ofToString(t->max)+"\"";
		
	}
	desc += "}";
	return desc;
}

