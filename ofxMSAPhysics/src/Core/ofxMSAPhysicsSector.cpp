/*
 *  ofxMSAPhysicsSector.cpp
 *  ofxMSAPhysics demo
 *
 *  Created by Mehmet Akten on 09/06/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofxMSAPhysicsSector.h"
#include "ofxMSAPhysicsUtils.h"

void ofxMSAPhysicsSector::checkSectorCollisions() {
	int s = _particles.size();
	for(int i=0; i<s-1; i++) {
		for(int j=i+1; j<s; j++) {
			checkCollisionBetween(_particles[i], _particles[j]);
		}
	}
}


bool ofxMSAPhysicsSector::checkCollisionBetween(ofxMSAParticle *a, ofxMSAParticle *b) {
	float restLength = b->getRadius() + a->getRadius();
	ofPoint delta = b->getPosition() - a->getPosition();
	float deltaLength2 = msaLengthSquared(delta);
	if(deltaLength2 >restLength * restLength) return false;
	
	float deltaLength = sqrt(deltaLength2);	// TODO: fast approximation of square root (1st order Taylor-expansion at a neighborhood of the rest length r (one Newton-Raphson iteration with initial guess r))
	float force = (deltaLength - restLength) / (deltaLength * (a->getInvMass() + b->getInvMass()));
	
	ofPoint deltaForce = delta * force;
	
	if (a->isFree()) a->moveBy(deltaForce * a->getInvMass(), false);
	if (b->isFree()) b->moveBy(deltaForce * -b->getInvMass(), false);
	
	return true;
}

