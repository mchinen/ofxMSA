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

#pragma once

#include "ofMain.h"
#include "ofxObjCPointer.h"

#include "ofxMSAPhysicsUtils.h"
#include "ofxMSAParticle.h"
#include "ofxMSAConstraintTypes.h"

class ofxMSAConstraint : public ofxObjCPointer {
public:
	friend class ofxMSAPhysics;

	ofxMSAConstraint() {
		_isOn = true;
		_type = -1;
		_isDead = false;
		verbose = true;
		_params = NULL;

		setClassName("ofxMSAConstraint");
	}

	virtual ~ofxMSAConstraint() {
		_a->release();
		_b->release();
	}

	inline int type() {
		return _type;
	}

	inline ofxMSAParticle* getOneEnd() {
		return _a;
	}

	inline ofxMSAParticle* getTheOtherEnd() {
		return _b;
	}

	inline ofxMSAParticle* getA() {
		return _a;
	}

	inline ofxMSAParticle* getB() {
		return _b;
	}

	inline void turnOff() {
		_isOn = false;
	}

	inline void turnOn() {
		_isOn = true;
	}

	inline bool isOn() {
		return (_isOn == true);
	}

	inline bool isOff(){
		return (_isOn == false);
	}

	inline void kill() {
		_isDead = true;
	}

	inline bool isDead() {
		return _isDead;
	}

	// only worth solving the constraint if its on, and at least one end is free
	inline bool shouldSolve() {
		return _isOn && (_a->isFree() || _b->isFree());
	}

	virtual void update() {}
	virtual void draw() {}



protected:
	int				_type;
	bool			_isOn;
	bool			_isDead;
	ofxMSAParticle	*_a, *_b;
	ofxMSAPhysicsParams *_params;
	virtual void solve() = 0;

	virtual void debugDraw() {
	   //ofLine(_a->x, _a->y, _b->x, _b->y);
	    /*
		ofPoint vec = (*_b - *_a);
		float dist = msaLength(vec);
		float angle = acos( vec.z / dist ) * RAD_TO_DEG;
		if(vec.z <= 0 ) angle = -angle;
		float rx = -vec.y * vec.z;
		float ry =  vec.x * vec.z;

		glPushMatrix();
		glTranslatef(_a->x, _a->y, _a->z);
		glRotatef(angle, rx, ry, 0.0);
		glScalef(1, 1, dist);
		glTranslatef(0, 0, 0.5);
#ifndef TARGET_OF_IPHONE
		glutSolidCube(1);
#endif
		glPopMatrix();
		*/
	}
};



