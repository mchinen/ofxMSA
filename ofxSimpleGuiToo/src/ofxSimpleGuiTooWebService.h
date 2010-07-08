#ifndef __OFXWEBSIMPLEGUITOO__
#define __OFXWEBSIMPLEGUITOO__

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxWebServer.h"

class ofxSimpleGuiTooWebService: public ofxWSRequestHandler {
public:
	void startServer(int port = 8910) {
		server.start("ofxWebSimpleGuiToo", port);
		server.addHandler(this, "/control");
	}
	
	void stopServer() {
		server.stop();
	}
	
	
	void httpGet(string url) {
		
		string key = getRequestParameter("key");
		if(key=="") {
			string guiDesc = "[";
			for(int i = 0; i < gui->getPages().size(); i++) {
				guiDesc += describePageAsJSON(gui->getPage(i));
				if(i+1<gui->getPages().size()) {
					guiDesc += ",";
				}
			}
			guiDesc += "]";
			httpResponse(guiDesc);
		} else {
			
			string value = getRequestParameter("value");
			printf("value received: %s = %s\n", key.c_str(), value.c_str());
			ofxSimpleGuiControl *control = getControlByName(key);
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
	
private:
	ofxSimpleGuiToo *gui;
	ofxWebServer server;
	
	string describePageAsJSON(ofxSimpleGuiPage *page) {
		string desc = "{\"name\": \""+page->name+"\", \"controls\":[";
		for(int i = 0; i < page->getControls().size(); i++) {
			desc += describeControlAsJSON(page->getControls()[i]);
			if(i+1<page->getControls().size()) {
				desc += ",";
			}
		}
		desc += "]}";
		return desc;
	}
	
	
	string describeControlAsJSON(ofxSimpleGuiControl *control) {
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
};
#endif // __OFXWEBSIMPLEGUITOO__
