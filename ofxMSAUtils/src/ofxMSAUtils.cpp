/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

#include "ofxMSAUtils.h"

void msaClear() {
	glClearColor(ofBgColorPtr()[0], ofBgColorPtr()[1], ofBgColorPtr()[2], ofBgColorPtr()[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void msaDrawFPS() {
	msaDrawFPS(10, ofGetHeight() - 10);
}

void msaDrawFPS(int x, int y) {
	ofDisableAlphaBlending();
	ofFill();
	glColor4f(0, 0, 0, 1);
	ofRect(x - 10, y - 20, x + 100, y + 10);
	glColor4f(0.9, 0.9, 0.9, 1);
	ofDrawBitmapString("FPS: "+ofToString(ofGetFrameRate(), 2), x, y);
}

void msaDrawFPS(int color) {
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofFill();
	ofSetColor(color);
	string fpsStr = "FPS: "+ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapString(fpsStr, 20, ofGetHeight()-20);
}



void msaDrawFPSBar(int fpsMult) {
	static float lastTime = 0;
	float nowTime = ofGetElapsedTimef();
	float fps = 1.0f/(nowTime - lastTime);
	lastTime = nowTime;
	
	// draw a green bar width of fps
	ofEnableAlphaBlending();
	glColor4f(0, 1, 0, 0.5);
	ofRect(0, 0, fps * fpsMult, 20);
	glColor4f(0, 0, 0, 1);
	// draw a line every 10 pixels
	for(int x=(fpsMult * 10); x<ofGetWidth(); x+=(fpsMult * 10)) {
		ofLine(x, 0, x, x % (fpsMult * 30) == 0 ? 30 : 20);		// draw a taller line every 30fps
	}	
}

void msaDumpFPS(float seconds) {
	static float lastTimeDumped = 0;
	float nowTime = ofGetElapsedTimef();
	if(nowTime - lastTimeDumped > seconds) {
		printf("FPS: %f\n", ofGetFrameRate());
		lastTimeDumped = nowTime;
	}
	
}


void msaSetCursor(bool forceOn) {
	if(forceOn || ofGetWindowMode() == OF_WINDOW) ofShowCursor();
	else ofHideCursor();
}




void msaConstrain(float &pos, float &vel, float min, float max, float bounce) {
	if(pos < min) {
		pos = min;
		vel = fabs(vel) * bounce;
	} else if(pos > max) {
		pos = max;
		vel = -fabs(vel) * bounce;
	}
}	

void msaConstrain(ofPoint &pos, ofPoint &vel, ofPoint &min, ofPoint &max, float bounce) {
	msaConstrain(pos.x, vel.x, min.x, max.x, bounce);
	msaConstrain(pos.y, vel.y, min.y, max.y, bounce);
	msaConstrain(pos.z, vel.z, min.z, max.z, bounce);
}



static GLfloat tex_coords[] = {
0, 0,
1, 0,
1, 1,
0, 1
};

static GLfloat verts[] = {
-0.5f, -0.5f,
0.5f, -0.5f,
0.5f, 0.5f,
-0.5f, 0.5f
};


void msaDrawQuadAtCorner() {
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0, verts );
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void msaDrawQuadAtCenter() {
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, 0, verts );
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0, verts );
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}


void msaDrawTexture(GLuint texId, GLenum textureTarget ) {
	glEnable(textureTarget);
	glBindTexture(textureTarget, texId);
	msaDrawQuadAtCenter();
	glDisable(textureTarget);
}
