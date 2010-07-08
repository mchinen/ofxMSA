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

#include "ofxMSAPhysicsParams.h"

class ofxMSAPhysics;

class ofxMSAParticle : public ofxObjCPointer {
public:
	friend class ofxMSAPhysics;

	ofxMSAParticle();
	ofxMSAParticle(ofPoint pos, float m = 1.0f, float d = 1.0f);
	ofxMSAParticle(ofxMSAParticle &p);
	
	virtual void	init(ofPoint pos, float m = 1.0f, float d = 1.0f);

	ofxMSAParticle* setMass(float t = 1);
	float			getMass();
	float			getInvMass();

	ofxMSAParticle* setDrag(float t = 1);
	float			getDrag();

	ofxMSAParticle* setBounce(float t = 1);
	float			getBounce();

	ofxMSAParticle* setRadius(float t = 15);
	float			getRadius();

	// collision methods
	ofxMSAParticle* enableCollision();
	ofxMSAParticle* disableCollision();
	bool			hasCollision();

	bool			isFixed();
	bool			isFree();
	ofxMSAParticle* makeFixed();
	ofxMSAParticle* makeFree();

	// move the particle
	// if preserveVelocity == true, the particle will move to new position and keep it's old velocity
	// if preserveVelocity == false, the particle will move to new position but gain the velocity of the displacement
	ofxMSAParticle* moveTo(ofPoint targetPos, bool preserveVelocity = true);
    ofxMSAParticle* moveBy(ofPoint offset, bool preserveVelocity = true);

	float			getX();
	float			getY();
	float			getZ();
	ofPoint			getPosition();
	
    ofxMSAParticle* setVelocity(ofPoint vel);
    ofxMSAParticle* addVelocity(ofPoint vel);
	ofPoint			getVelocity();

	// override these functions if you create your own particle type with custom behaviour and/or drawing
	virtual void	update() {}
	virtual void	draw() {}

	void			kill();
	bool			isDead();
	
	// custom void* which you can use to store any kind of custom data
	void			*data;

	ofxMSAPhysicsParams *getParams();

protected:
	ofxMSAPhysicsParams *_params;
	ofxMSAPhysics		*_physics;

	ofPoint			_pos;
	ofPoint			_oldPos;
	float			_mass, _invMass;
	float			_drag;
	float			_bounce;
	float			_radius;
	float			_age;
	bool			_isDead;
	bool			_isFixed;
	bool			_collisionEnabled;

	void			doVerlet();
	void			checkWorldEdges();

	virtual void debugDraw();
};



inline ofxMSAParticle* ofxMSAParticle::setMass(float t) {
	if(t==0) t=0.00001f;
	_mass = t;
	_invMass = 1.0f/t;
	return this;
}

inline float ofxMSAParticle::getMass() {
	return _mass;
}

inline float ofxMSAParticle::getInvMass() {
	return _invMass;
}


inline ofxMSAParticle* ofxMSAParticle::setDrag(float t) {
	_drag = t;
	return this;
}

inline float ofxMSAParticle::getDrag() {
	return _drag;
}

inline ofxMSAParticle* ofxMSAParticle::setBounce(float t) {
	_bounce = t;
	return this;
}

inline float ofxMSAParticle::getBounce() {
	return _bounce;
}


inline ofxMSAParticle* ofxMSAParticle::setRadius(float t) {
	_radius = t;
	return this;
}

inline float ofxMSAParticle::getRadius() {
	return _radius;
}

inline bool ofxMSAParticle::isFixed() {
	return (_isFixed == true);
}

inline bool ofxMSAParticle::isFree() {
	return (_isFixed == false);
}

inline ofxMSAParticle* ofxMSAParticle::makeFixed() {
	_isFixed = true;
	return this;
}

inline ofxMSAParticle* ofxMSAParticle::makeFree() {
	_oldPos = _pos;
	_isFixed = false;
	return this;
}

inline ofxMSAParticle* ofxMSAParticle::moveTo(ofPoint targetPos, bool preserveVelocity) {
	ofPoint diff = targetPos - _pos;
	moveBy(diff, preserveVelocity);
	return this;
}

inline ofxMSAParticle* ofxMSAParticle::moveBy(ofPoint offset, bool preserveVelocity) {
	_pos += offset;
	if(preserveVelocity) _oldPos += offset;
	return this;
}

inline float ofxMSAParticle::getX() {
	return _pos.x;
}

inline float ofxMSAParticle::getY() {
	return _pos.y;
}

inline float ofxMSAParticle::getZ() {
	return _pos.z;
}

inline ofPoint ofxMSAParticle::getPosition() {
	return _pos;
}

inline ofxMSAParticle* ofxMSAParticle::setVelocity(ofPoint vel) {
	_oldPos = _pos - vel;
	return this;
}

inline ofxMSAParticle* ofxMSAParticle::addVelocity(ofPoint vel) {
	_oldPos -= vel;
	return this;
}

inline ofPoint ofxMSAParticle::getVelocity() {
	return _pos - _oldPos;
}

inline void ofxMSAParticle::kill() {
	_isDead = true;
}


inline bool ofxMSAParticle::isDead() {
	return _isDead;
}



