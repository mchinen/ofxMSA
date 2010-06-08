/*
 *  ofxMSABulletWorldBase.cpp
 *  ofxMSABulletWorldBase demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ofxMSABulletWorldBase.h"


ofxMSABulletWorldBase::ofxMSABulletWorldBase() {
	bulletWorld				= NULL;
	solver					= NULL;
	broadphase				= NULL;
	dispatcher				= NULL;
	collisionConfiguration	= NULL;
}

ofxMSABulletWorldBase::~ofxMSABulletWorldBase() {
	destroy();
}


void ofxMSABulletWorldBase::setup() {
	setup(ofPoint(-10000,-10000,-10000), ofPoint(10000, 10000, 10000));
}

void ofxMSABulletWorldBase::setup(ofPoint worldMin, ofPoint worldMax) {
	if(!collisionConfiguration) collisionConfiguration = new btDefaultCollisionConfiguration();
	
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	if(!dispatcher) dispatcher = new btCollisionDispatcher(collisionConfiguration);
	
	///the maximum size of the collision bulletWorld. Make sure objects stay within these boundaries
	///Don't make the bulletWorld AABB size too large, it will harm simulation quality and performance
	if(!broadphase) {
		btVector3 worldAabbMin = ofPoint_To_btVector3(worldMin);
		btVector3 worldAabbMax = ofPoint_To_btVector3(worldMax);
		int	maxProxies = 1024;
		broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
	}
	
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	if(!solver) solver = new btSequentialImpulseConstraintSolver;
	if(!bulletWorld) bulletWorld = createBulletWorld();
	
	setGravity();
}

void ofxMSABulletWorldBase::destroy() {
	//cleanup in the reverse order of creation/initialization	
	
	//remove the rigidbodies from the dynamics bulletWorld and delete them
	for (int i=bulletWorld->getNumCollisionObjects()-1; i>=0 ;i--) {
		btCollisionObject* obj = bulletWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		bulletWorld->removeCollisionObject( obj );
		delete obj;
	}	
	
	//delete objects
	for (int i=0; i<objects.size(); i++) {
		ofxMSABulletRigidBody* o = objects[i];
		objects[i] = 0;
		delete o;		// this line results in "non-page-aligned, non-allocated pointer being freed". not sure why
	}
	objects.clear();

	MSA_CHECK_DELETE(bulletWorld);
	MSA_CHECK_DELETE(solver);
	MSA_CHECK_DELETE(broadphase);
	MSA_CHECK_DELETE(dispatcher);
	MSA_CHECK_DELETE(collisionConfiguration);
}

//
//
//ofxMSABulletObjectBox *createBox(ofPoint &position) {
//	ofxMSABulletObjectBox *box = new ofxMSABulletObjectBox;
//}



