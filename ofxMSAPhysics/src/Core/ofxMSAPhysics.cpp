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

#include "ofxMSAPhysics.h"
#include "binner.h"

ofxMSAPhysics::ofxMSAPhysics() {
	verbose = false;
	setTimeStep(0.000010);
	setDrag();
	setNumIterations();
	enableCollision();
	setCollisionBinSize(10);
	setGravity();
	clearWorldSize();

#ifdef MSAPHYSICS_USE_RECORDER
	_frameCounter = 0;
	setReplayMode(OFX_MSA_DATA_IDLE);
	setReplayFilename("recordedData/physics/physics");
#endif
}

ofxMSAPhysics::~ofxMSAPhysics() {
	clear();
}



ofxMSAParticle* ofxMSAPhysics::makeParticle(ofPoint pos, float m, float d) {
	ofxMSAParticle *p = new ofxMSAParticle(pos, m, d);
	addParticle(p);
	p->release();	// cos addParticle(p) retains it
	return p;
}

ofxMSASpring* ofxMSAPhysics::makeSpring(ofxMSAParticle *a, ofxMSAParticle *b, float _strength, float _restLength) {
	if(a==b) return NULL;
	ofxMSASpring* c = new ofxMSASpring(a, b, _strength, _restLength);
	addConstraint(c);
	c->release();	// cos addConstraint(c) retains it
	return c;
}

ofxMSAAttraction* ofxMSAPhysics::makeAttraction(ofxMSAParticle *a, ofxMSAParticle *b, float _strength, float _minimumDistance) {
	if(a==b) return NULL;
	ofxMSAAttraction* c = new ofxMSAAttraction(a, b, _strength, _minimumDistance);
	addConstraint(c);
	c->release();	// cos addConstraint(c) retains it
	return c;
}

ofxMSACollision* ofxMSAPhysics::makeCollision(ofxMSAParticle *a, ofxMSAParticle *b) {
	if(a==b) return NULL;
	if(getConstraint(a, b, OFX_MSA_CONSTRAINT_TYPE_COLLISION) != NULL) return NULL;
	ofxMSACollision* c = new ofxMSACollision(a, b);
	addConstraint(c);
	c->release();	// cos addConstraint(c) retains it
	return c;
}




ofxMSAParticle* ofxMSAPhysics::addParticle(ofxMSAParticle *p) {
	p->verbose = verbose;
	_particles.push_back(p);
	p->setInstanceName(string("particle ") + ofToString(_particles.size(), 0));
	p->_params = &params;
	p->_physics = this;
	if(isCollisionEnabled()) p->enableCollision();

#ifdef MSAPHYSICS_USE_RECORDER
	if(_replayMode == OFX_MSA_DATA_SAVE)
		_recorder.setSize(numberOfParticles());
#endif
	p->retain();
	return p;		// so you can configure the particle or use for creating constraints
}

ofxMSAConstraint* ofxMSAPhysics::addConstraint(ofxMSAConstraint *c) {
	c->verbose = verbose;
	_constraints[c->type()].push_back(c);
	c->_params = &params;

	c->retain();
	(c->_a)->retain();
	(c->_b)->retain();

	switch(c->type()) {
		case OFX_MSA_CONSTRAINT_TYPE_CUSTOM:
			c->setInstanceName(string("constraint ") + ofToString(_constraints[OFX_MSA_CONSTRAINT_TYPE_CUSTOM].size(), 0));
			break;

		case OFX_MSA_CONSTRAINT_TYPE_SPRING:
			c->setInstanceName(string("spring ") + ofToString(_constraints[OFX_MSA_CONSTRAINT_TYPE_SPRING].size(), 0));
			break;

		case OFX_MSA_CONSTRAINT_TYPE_ATTRACTION:
			c->setInstanceName(string("attraction ") + ofToString(_constraints[OFX_MSA_CONSTRAINT_TYPE_ATTRACTION].size(), 0));
			break;

		case OFX_MSA_CONSTRAINT_TYPE_COLLISION:
			c->setInstanceName(string("collision ") + ofToString(_constraints[OFX_MSA_CONSTRAINT_TYPE_COLLISION].size(), 0));
			break;
	}

	return c;
}


ofxMSAParticle*		ofxMSAPhysics::getParticle(uint i) {
	return i < numberOfParticles() ? _particles[i] : NULL;
}

ofxMSAConstraint*	ofxMSAPhysics::getConstraint(uint i) {
	return i < numberOfConstraints() ? _constraints[OFX_MSA_CONSTRAINT_TYPE_CUSTOM][i] : NULL;
}

ofxMSASpring*		ofxMSAPhysics::getSpring(uint i) {
	return i < numberOfSprings() ? (ofxMSASpring*)_constraints[OFX_MSA_CONSTRAINT_TYPE_SPRING][i] : NULL;
}

ofxMSAAttraction*	ofxMSAPhysics::getAttraction(uint i) {
	return i < numberOfAttractions() ? (ofxMSAAttraction*)_constraints[OFX_MSA_CONSTRAINT_TYPE_ATTRACTION][i] : NULL;
}

ofxMSACollision*	ofxMSAPhysics::getCollision(uint i) {
	return i < numberOfCollisions() ? (ofxMSACollision*)_constraints[OFX_MSA_CONSTRAINT_TYPE_COLLISION][i] : NULL;
}



uint ofxMSAPhysics::numberOfParticles() {
	return _particles.size();
}

uint ofxMSAPhysics::numberOfConstraints() {
	return _constraints[OFX_MSA_CONSTRAINT_TYPE_CUSTOM].size();
}

uint ofxMSAPhysics::numberOfSprings() {
	return _constraints[OFX_MSA_CONSTRAINT_TYPE_SPRING].size();
}

uint ofxMSAPhysics::numberOfAttractions() {
	return _constraints[OFX_MSA_CONSTRAINT_TYPE_ATTRACTION].size();
}


uint ofxMSAPhysics::numberOfCollisions() {
	return _constraints[OFX_MSA_CONSTRAINT_TYPE_COLLISION].size();
}


ofxMSAPhysics* ofxMSAPhysics::enableCollision() {
	params.isCollisionEnabled = true;
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::disableCollision() {
	params.isCollisionEnabled = false;
	return this;
}

bool ofxMSAPhysics::isCollisionEnabled() {
	return params.isCollisionEnabled;
}


ofxMSAPhysics* ofxMSAPhysics::setCollisionBinSize(float f) {
	setCollisionBinSize(ofPoint(f, f, f));
	return this;
}


ofxMSAPhysics* ofxMSAPhysics::setCollisionBinSize(ofPoint p) {
	params.binSize = p;
	setupBins(params.worldMin, params.worldMax, params.binSize);
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::setCollisionBinCount(float f) {
    setCollisionBinCount(ofPoint(f, f, f));
    return this;
}

ofxMSAPhysics* ofxMSAPhysics::setCollisionBinCount(ofPoint p) {
    setCollisionBinSize(params.worldSize/ p);
    return this;
}


ofxMSAPhysics* ofxMSAPhysics::addToCollision(ofxMSAParticle* p) {
	for(int i=0; i< numberOfParticles(); i++) {
		ofxMSAParticle *target = getParticle(i);
		if(target->hasCollision()) makeCollision(target, p);
	}
	return this;
}


ofxMSAPhysics* ofxMSAPhysics::removeFromCollision(ofxMSAParticle* p) {
	for(int i=0; i<numberOfCollisions(); i++) {
		ofxMSAConstraint *c = getCollision(i);
		if(c->_a == p || c->_b == p) c->kill();
	}
	return this;
}



ofxMSAPhysics*  ofxMSAPhysics::setDrag(float drag) {
	params.drag = drag;
	return this;
}

ofxMSAPhysics*  ofxMSAPhysics::setGravity(float gy) {
	setGravity(ofPoint(0, gy, 0));
	return this;
}

ofxMSAPhysics*  ofxMSAPhysics::setGravity(ofPoint g) {
	params.gravity= g;
	params.doGravity = msaLengthSquared(params.gravity) > 0;
	return this;
}

ofPoint& ofxMSAPhysics::getGravity() {
	return params.gravity;
}

ofxMSAPhysics*  ofxMSAPhysics::setTimeStep(float timeStep) {
	params.timeStep = timeStep;
	params.timeStep2 = timeStep * timeStep;
	return this;
}

ofxMSAPhysics*  ofxMSAPhysics::setNumIterations(float numIterations) {
	params.numIterations = numIterations;
	return this;
}


ofxMSAPhysics* ofxMSAPhysics::setWorldMin(ofPoint worldMin) {
	params.worldMin = worldMin;
	updateWorldSize();
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::setWorldMax(ofPoint worldMax) {
	params.worldMax = worldMax;
	updateWorldSize();
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::setWorldSize(ofPoint worldMin, ofPoint worldMax) {
	setWorldMin(worldMin);
	setWorldMax(worldMax);
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::clearWorldSize() {
	params.doWorldEdges = false;
	return this;
}


void ofxMSAPhysics::updateWorldSize() {
    params.doWorldEdges = true;
    params.worldSize = params.worldMax - params.worldMin;
    params.worldSizeInv = ofPoint(1, 1, 1)/params.worldSize;
    setCollisionBinCount(ofPoint(10, 10, 10));
    setupBins(params.worldMin, params.worldMax, params.binSize);
}


ofxMSAPhysics*  ofxMSAPhysics::setParticleCount(uint i) {
	_particles.reserve(i);
#ifdef MSAPHYSICS_USE_RECORDER
	//	if(_replayMode == OFX_MSA_DATA_SAVE)
	_recorder.setSize(i);
#endif
	return this;
}


ofxMSAPhysics* ofxMSAPhysics::setConstraintCount(uint i){
	_constraints[OFX_MSA_CONSTRAINT_TYPE_CUSTOM].reserve(i);
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::setSpringCount(uint i){
	_constraints[OFX_MSA_CONSTRAINT_TYPE_SPRING].reserve(i);
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::setAttractionCount(uint i){
	_constraints[OFX_MSA_CONSTRAINT_TYPE_ATTRACTION].reserve(i);
	return this;
}

ofxMSAPhysics* ofxMSAPhysics::setCollisionCount(uint i){
	_constraints[OFX_MSA_CONSTRAINT_TYPE_COLLISION].reserve(i);
	return this;
}


void ofxMSAPhysics::clear() {
	for(vector<ofxMSAParticle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
		ofxMSAParticle* particle = *it;
		particle->release();
	}
	_particles.clear();


	for(int i=0; i<OFX_MSA_CONSTRAINT_TYPE_COUNT; i++) {
		for (vector<ofxMSAConstraint*>::iterator it = _constraints[i].begin(); it != _constraints[i].end(); it++) {
			ofxMSAConstraint* constraint = *it;
			constraint->release();
		}
		_constraints[i].clear();
	}
}





void ofxMSAPhysics::update(int frameNum) {
#ifdef MSAPHYSICS_USE_RECORDER
	if(frameNum < 0) frameNum = _frameCounter;
	if(_replayMode == OFX_MSA_DATA_LOAD) {
		load(frameNum);
	} else {
		updateParticles();
		updateAllConstraints();
		if(_replayMode == OFX_MSA_DATA_SAVE) _recorder.save(frameNum);
	}
	_frameCounter++;
#else
	updateParticles();
	updateAllConstraints();
#endif
}


void ofxMSAPhysics::draw() {
	for(int i=0; i<OFX_MSA_CONSTRAINT_TYPE_COUNT; i++) {
		for (vector<ofxMSAConstraint*>::iterator it = _constraints[i].begin(); it != _constraints[i].end(); it++) {
			ofxMSAConstraint* constraint = *it;
			constraint->draw();
		}
	}

	for(vector<ofxMSAParticle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
		ofxMSAParticle* particle = *it;
		particle->draw();
	}
}

void ofxMSAPhysics::debugDraw() {
	for(int i=0; i<OFX_MSA_CONSTRAINT_TYPE_COUNT; i++) {
		for (vector<ofxMSAConstraint*>::iterator it = _constraints[i].begin(); it != _constraints[i].end(); it++) {
			ofxMSAConstraint* constraint = *it;
			constraint->debugDraw();
		}
	}

	for(vector<ofxMSAParticle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
		ofxMSAParticle* particle = *it;
		particle->debugDraw();
	}
}

#ifdef MSAPHYSICS_USE_RECORDER
void ofxMSAPhysics::load(uint frameNum) {
	_recorder.load(frameNum);
	for(vector<ofxMSAParticle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
		ofxMSAParticle* particle = *it;
		particle->set(_recorder.get());// * _playbackScaler);
	}
}
#endif

void ofxMSAPhysics::updateParticles() {
	vector<ofxMSAParticle*>::iterator it = _particles.begin();
	int num = 0;
	while(it != _particles.end()) {
		ofxMSAParticle* particle = *it;
		if(particle->_isDead) {							// if particle is dead
			it = _particles.erase(it);
			particle->release();
		} else {
			num++;
			particle->doVerlet();
			particle->update();
			applyUpdaters(particle);
			if(params.doWorldEdges) {
//				if(particle->isFree()) 
					particle->checkWorldEdges();
				particle->computeBinFlags();
			}
#ifdef MSAPHYSICS_USE_RECORDER
			if(_replayMode == OFX_MSA_DATA_SAVE) _recorder.add(*particle);
#endif
			it++;
		}
	}
}


void ofxMSAPhysics::updateConstraintsByType(vector<ofxMSAConstraint*> constraints) {
}

void ofxMSAPhysics::updateAllConstraints() {
	// iterate all constraints and update
	for (int i = 0; i < params.numIterations; i++) {
		for(int i=0; i<OFX_MSA_CONSTRAINT_TYPE_COUNT; i++) {

			// if doing collision constraints and collision is disabled skip them
			if(i == OFX_MSA_CONSTRAINT_TYPE_COLLISION && !params.isCollisionEnabled) continue;

			vector<ofxMSAConstraint*>::iterator it = _constraints[i].begin();
			while(it != _constraints[i].end()) {
				ofxMSAConstraint* constraint = *it;
				if(constraint->_isDead || constraint->_a->_isDead || constraint->_b->_isDead) {
					constraint->kill();
					it = _constraints[i].erase(it);
					constraint->release();
				} else {
					if(constraint->shouldSolve()) constraint->solve();
					it++;
				}
			}

		}
	}
}


#ifdef MSAPHYSICS_USE_RECORDER
ofxMSAPhysics*  ofxMSAPhysics::setReplayMode(uint i, float playbackScaler) {
	_replayMode = i;
	_playbackScaler = playbackScaler;
	//	if(_replayMode == OFX_MSA_DATA_SAVE)		// NEW
	_recorder.setSize(i);
	return this;
}


ofxMSAPhysics*  ofxMSAPhysics::setReplayFilename(string f) {
	_recorder.setFilename(f);
	return this;
}
#endif




ofxMSAConstraint* ofxMSAPhysics::getConstraint(ofxMSAParticle *a, ofxMSAParticle *b, int constraintType) {
	for (vector<ofxMSAConstraint*>::iterator it = _constraints[constraintType].begin(); it != _constraints[constraintType].end(); it++) {
		ofxMSAConstraint* s = *it;
		if(((s->_a == a && s->_b == b) || (s->_a == b && s->_b == a)) && !s->_isDead) {
			return s;
		}
	}
	return NULL;
}


ofxMSAConstraint* ofxMSAPhysics::getConstraint(ofxMSAParticle *a, int constraintType) {
	for (vector<ofxMSAConstraint*>::iterator it = _constraints[constraintType].begin(); it != _constraints[constraintType].end(); it++) {
		ofxMSAConstraint* s = *it;
		if (((s->_a == a ) || (s->_b == a)) && !s->_isDead) {
			return s;
		}
	}
	return NULL;
}
