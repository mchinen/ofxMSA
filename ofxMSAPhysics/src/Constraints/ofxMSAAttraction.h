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
#include "ofxMSAConstraint.h"

class ofxMSAAttraction : public ofxMSAConstraint {
public:
	friend class ofxMSAPhysics;

	ofxMSAAttraction(ofxMSAParticle *a, ofxMSAParticle *b, float strength) {
		this->_a = a;
		this->_b = b;
		setStrength(strength);

		_type = OFX_MSA_CONSTRAINT_TYPE_ATTRACTION;
	}

	// set strength of attraction (+ve
	void setStrength(float newStrength);
	
	// get strength of attraction
	float getStrength();

protected:
	float _strength;

	void solve() {
		ofPoint delta = _b->getPosition() - _a->getPosition();
		float deltaLength2 = msaLengthSquared(delta);
		
		float force = _strength * (_b->getMass()) * (_a->getMass()) / (deltaLength2 + 0.001);		// to avoid divide by zero
		
		ofPoint deltaForce = delta * force;
		
		if (_a->isFree()) _a->moveBy(deltaForce * _a->getInvMass(), false);
		if (_b->isFree()) _b->moveBy(deltaForce * -_b->getInvMass(), false);
	}

    void debugDraw() {
        ofSetColor(0, 0, 255);
        ofxMSAConstraint::debugDraw();
	}
};

// set strength of attraction (+ve
inline void ofxMSAAttraction::setStrength(float newStrength) {
	_strength = newStrength;
}

inline float ofxMSAAttraction::getStrength() {
	return _strength;
}


