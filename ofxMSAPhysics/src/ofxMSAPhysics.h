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

/*
 based on principles mentioned at
  http://www.gamasutra.com/resource_guide/20030121/jacobson_01.shtml


fast inverse square root mentioned at
 http://en.wikipedia.org/wiki/Fast_inverse_square_root
 attributed to John Carmack but apparently much older
 */

#pragma once

#define OF_ADDON_USING_OFXMSAPHYSICS

#include "ofMain.h"

#include "ofxObjCPointer.h"

#include "ofxMSAParticle.h"
#include "ofxMSAConstraint.h"
#include "ofxMSASpring.h"
#include "ofxMSAAttraction.h"

#include "ofxMSAPhysicsUtils.h"
#include "ofxMSAPhysicsParams.h"
#include "ofxMSAPhysicsCallbacks.h"

#include "ofxMSAPhysicsSector.h"

#ifdef MSAPHYSICS_USE_RECORDER
#include "ofxMSADataRecorder.h"
#endif



class ofxMSAPhysics : public ofxMSAParticleUpdatable {

public:
	friend class ofxMSAParticle;

	bool				verbose;

	ofxMSAPhysics();
	~ofxMSAPhysics();

	ofxMSAParticle*		makeParticle(ofPoint pos, float m = 1.0f, float d = 1.0f);
	ofxMSASpring*		makeSpring(ofxMSAParticle *a, ofxMSAParticle *b, float _strength, float _restLength);
	ofxMSAAttraction*	makeAttraction(ofxMSAParticle *a, ofxMSAParticle *b, float _strength);

	// this method retains the particle, so you should release() it after adding (obj-c style)
	ofxMSAParticle*		addParticle(ofxMSAParticle *p);

	// this method retains the constraint, so you should release it after adding (obj-c style)
	ofxMSAConstraint*	addConstraint(ofxMSAConstraint *c);

	ofxMSAParticle*		getParticle(uint i);
	ofxMSAConstraint*	getConstraint(uint i);			// generally you wouldn't use this but use the ones below
	ofxMSASpring*		getSpring(uint i);
	ofxMSAAttraction*	getAttraction(uint i);

	uint				numberOfParticles();
	uint				numberOfConstraints();		// all constraints: springs, attractions and user created
	uint				numberOfSprings();			// only springs
	uint				numberOfAttractions();		// only attractions

	ofxMSAPhysics*		setDrag(float drag = 0.99);					// set the drag. 1: no drag at all, 0.9: quite a lot of drag, 0: particles can't even move
	ofxMSAPhysics*		setGravity(float gy = 0);					// set gravity (y component only)
	ofxMSAPhysics*		setGravity(ofPoint g);						// set gravity (full vector)
	ofPoint&			getGravity();
	ofxMSAPhysics*		setTimeStep(float timeStep);
	ofxMSAPhysics*		setNumIterations(float numIterations = 20);	// default value

	// for optimized collision, set world dimensions first
	ofxMSAPhysics*		setWorldMin(ofPoint worldMin);
	ofxMSAPhysics*		setWorldMax(ofPoint worldMax);
	ofxMSAPhysics*		setWorldSize(ofPoint worldMin, ofPoint worldMax);
	ofxMSAPhysics*		clearWorldSize();

	// and then set sector size (or count)
	ofxMSAPhysics*		enableCollision();
	ofxMSAPhysics*		disableCollision();
	bool				isCollisionEnabled();
    ofxMSAPhysics*      setSectorCount(int count);		// set the number of sectors (will be equal in each axis)
    ofxMSAPhysics*      setSectorCount(ofPoint vCount);	// set the number of sectors in each axis
	
	// preallocate buffers if you know how big they need to be (they grow automatically if need be)
	ofxMSAPhysics*		setParticleCount(uint i);
	ofxMSAPhysics*		setConstraintCount(uint i);
	ofxMSAPhysics*		setSpringCount(uint i);
	ofxMSAPhysics*		setAttractionCount(uint i);


	void clear();
	void update(int frameNum = -1);
	void draw();
	void debugDraw();

#ifdef MSAPHYSICS_USE_RECORDER
	ofxMSAPhysics*		setReplayMode(int i, float playbackScaler = 1.0f);		// when playing back recorded data, optionally scale positions up (so you can record in lores, playback at highres)
	ofxMSAPhysics*		setReplayFilename(string f);
#endif

protected:
	vector<ofxMSAParticle*>		_particles;
	vector<ofxMSAConstraint*>	_constraints[OFX_MSA_CONSTRAINT_TYPE_COUNT];
	vector<ofxMSAPhysicsSector> _sectors;
	
	ofxMSAPhysicsParams			params;

	void						updateParticles();
	void						updateConstraints();
	void						updateConstraintsByType(vector<ofxMSAConstraint*> constraints);
//	void						updateWorldSize();
	
	
	void						checkAllCollisions();

	ofxMSAConstraint*			getConstraint(ofxMSAParticle *a, int constraintType);
	ofxMSAConstraint*			getConstraint(ofxMSAParticle *a, ofxMSAParticle *b, int constraintType);


#ifdef MSAPHYSICS_USE_RECORDER
	ofxMSADataRecorder<ofPoint>_recorder;
	uint						_frameCounter;
	uint						_replayMode;
	float						_playbackScaler;
	void load(uint frameNum);
#endif
};
