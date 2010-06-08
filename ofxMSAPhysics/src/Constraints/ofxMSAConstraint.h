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
		
		setMinDistance(0);
		setMaxDistance(0);
		
		setClassName("ofxMSAConstraint");
	}

	virtual ~ofxMSAConstraint() {
		_a->release();
		_b->release();
	}

	int type();
	
	// getOneEnd is a same as getA and getTheOtherEnd is same as getB
	// just have both methods so you can choose whichever you please
	ofxMSAParticle* getOneEnd();
	ofxMSAParticle* getTheOtherEnd();
	
	ofxMSAParticle* getA();
	ofxMSAParticle* getB();

	void turnOff();
	void turnOn();

	bool isOn();
	bool isOff();

	void kill();
	bool isDead();
	
	// set minimum distance before constraint takes affect
	void setMinDistance(float d);
	
	// get minimum distance
	float getMinDistance();
	
	// set maximum distance before constraint takes affect
	void setMaxDistance(float d);
	
	// get maximum distance
	float getMaxDistance();
	

	// only worth solving the constraint if its on, and at least one end is free
	bool shouldSolve();
	
	virtual void update() {}
	virtual void draw() {}



protected:
	int				_type;
	bool			_isOn;
	bool			_isDead;
	float			_minDist;
	float			_minDist2;
	float			_maxDist;
	float			_maxDist2;
	
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

inline int ofxMSAConstraint::type() {
	return _type;
}

inline ofxMSAParticle* ofxMSAConstraint::getOneEnd() {
	return _a;
}

inline ofxMSAParticle* ofxMSAConstraint::getTheOtherEnd() {
	return _b;
}

inline ofxMSAParticle* ofxMSAConstraint::getA() {
	return _a;
}

inline ofxMSAParticle* ofxMSAConstraint::getB() {
	return _b;
}

inline void ofxMSAConstraint::turnOff() {
	_isOn = false;
}

inline void ofxMSAConstraint::turnOn() {
	_isOn = true;
}

inline bool ofxMSAConstraint::isOn() {
	return (_isOn == true);
}

inline bool ofxMSAConstraint::isOff(){
	return (_isOn == false);
}

inline void ofxMSAConstraint::kill() {
	_isDead = true;
}

inline bool ofxMSAConstraint::isDead() {
	return _isDead;
}


inline void ofxMSAConstraint::setMinDistance(float d) {
	_minDist = d;
	_minDist2 = d*d;
}

inline float ofxMSAConstraint::getMinDistance() {
	return _minDist;
}

inline void ofxMSAConstraint::setMaxDistance(float d) {
	_maxDist = d;
	_maxDist2 = d*d;
}

inline float ofxMSAConstraint::getMaxDistance() {
	return _maxDist;
}

// only worth solving the constraint if its on, and at least one end is free
inline bool ofxMSAConstraint::shouldSolve() {
	
	// if the constraint is off or both sides are fixed then return false
	if(isOff() || (_a->isFixed() && _b->isFixed())) return false;	
	
	// if no length restrictions then return true (by this point we know above condition is false)
	if(_minDist == 0 && _maxDist == 0) return true;
	
	ofPoint delta = _b->getPosition() - _a->getPosition();
	float deltaLength2 = msaLengthSquared(delta);
	
	bool minDistSatisfied;
	if(_minDist) minDistSatisfied = deltaLength2 > _minDist2;
	else minDistSatisfied = true;
	
	bool maxDistSatisfied;
	if(_maxDist) maxDistSatisfied = deltaLength2 < _maxDist2;
	else maxDistSatisfied = true;
	
	return minDistSatisfied && maxDistSatisfied;
}



