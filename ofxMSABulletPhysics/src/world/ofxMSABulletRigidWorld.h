/*
 *  ofxMSABulletRigidWorld.h
 *  ofxMSABulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

// container for rigid body objects only

#pragma once

#include "ofxMSABulletWorldBase.h"

class ofxMSABulletRigidWorld : public ofxMSABulletWorldBase {
public:

	
protected:
	btDynamicsWorld* createBulletWorld() {
		return new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	}
};