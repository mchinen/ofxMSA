/*
 *  ofxPingPongBuffer.h
 *  OpenCL example - images
 *
 *  Created by Mehmet Akten on 17/11/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#ifndef OFX_DOUBLE_BUFFER

#define OFX_DOUBLE_BUFFER

template<class T>
class ofxDoubleBuffer {
public:
	
	ofxDoubleBuffer() {
		currentIndex = 0;
	}
	
	T& getFront() {
		return objects[currentIndex];
	}
	
	T& getBack() {
		return objects[1-currentIndex];
	}
	
	void swap() {
		currentIndex = 1 - currentIndex;
	}
	
protected:
	T		objects[2];
	int		currentIndex;
	
	
	
};

#endif