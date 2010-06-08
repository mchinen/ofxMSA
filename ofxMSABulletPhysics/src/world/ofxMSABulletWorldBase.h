/*
 *  ofxMSABulletWorldBase.h
 *  ofxMSABulletWorldBase demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */


#pragma once

#include "ofMain.h"
#include "btBulletDynamicsCommon.h"

#include "ofxMSABulletPhysicsUtils.h"
#include "ofxMSABulletRigidBody.h"


class ofxMSABulletWorldBase {
public:
	ofxMSABulletWorldBase();
	virtual ~ofxMSABulletWorldBase();
	
	void setup();
	void setup(ofPoint worldMin, ofPoint worldMax);
	void destroy();
	
	void setGravity(ofPoint &p);
	void setGravity(btScalar gx = 0, btScalar gy = 9.8, btScalar gz = 0);
	
	
	// creates a rigid body and adds to the world (and returns pointer to newly created object)
	// object will be released automatically when world is destroyed
	ofxMSABulletRigidBody *createRigidBox(ofPoint &startPosition, ofPoint &size, float mass = 1);
	ofxMSABulletRigidBody *createRigidSphere(ofPoint &startPosition, float radius, float mass = 1);
	
	// create a rigid body out of given collisionShape and add to the world
	// will be released automatically when world is destroyed
	// if you want to create a rigid body with an existing collision shape use this method
	// (e.g. if you want to use the collisionShape from a previously created box, sphere etc.)
	// (reusing collisionShapes is very much advised if possible)
	// ownsCollisionShape:	if the body owns (and should delete) the collision shape upon destruction set this true
	//						if the body is sharing this collision shape set to false
	ofxMSABulletRigidBody *createRigidBody(ofPoint &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass = 1);
	
	// add an existing rigid body to the world
	// will be released automatically when world is destroyed
	ofxMSABulletRigidBody *addRigidBody(ofxMSABulletRigidBody *body);
	
	// add an existing bullet rigid body to the world (the original bullet class)
	// FOR ADVANCED USE: will NOT be released automatically when world is destroyed
	void addBulletRigidBody(btRigidBody *bulletBody);	
	
	
	
	void update();
	void debugDraw();
	
	// return reference to the bullet world
	// FOR ADVANCED USE
	btDynamicsWorld &getBulletWorld();
	
	
	// FOR ADVANCED USE
	// not making these protected for now so you can create your own versions of these before calling setup()
	btDynamicsWorld								*bulletWorld;	
	btCollisionConfiguration					*collisionConfiguration;
	btCollisionDispatcher						*dispatcher;
	btBroadphaseInterface						*broadphase;
	btConstraintSolver							*solver;
	btAlignedObjectArray<ofxMSABulletRigidBody*>	objects;
	
protected:
	virtual btDynamicsWorld* createBulletWorld() = 0;
};


inline ofxMSABulletRigidBody *ofxMSABulletWorldBase::addRigidBody(ofxMSABulletRigidBody *body) {
	objects.push_back(body);
	addBulletRigidBody(body->getBulletBody());
	return body;
}


inline 	ofxMSABulletRigidBody *ofxMSABulletWorldBase::createRigidBody(ofPoint &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass) {
	ofxMSABulletRigidBody *body = new ofxMSABulletRigidBody();
	body->setup(startPosition, collisionShape, ownsCollisionShape, mass);
	addRigidBody(body);
	
	return body;
}


inline ofxMSABulletRigidBody *ofxMSABulletWorldBase::createRigidSphere(ofPoint &startPosition, float radius, float mass) {
	btCollisionShape *collisionShape = new btSphereShape(btScalar(radius));
	return createRigidBody(startPosition, collisionShape, true, mass); // body will own this collision shape
}	


inline void ofxMSABulletWorldBase::addBulletRigidBody(btRigidBody *bulletBody) {
	bulletWorld->addRigidBody(bulletBody);
}


inline 	btDynamicsWorld& ofxMSABulletWorldBase::getBulletWorld() {
	return *bulletWorld;
}

inline void ofxMSABulletWorldBase::setGravity(ofPoint &p) {
	bulletWorld->setGravity(ofPoint_To_btVector3(p));
}

inline void ofxMSABulletWorldBase::setGravity(btScalar gx, btScalar gy, btScalar gz) {
	bulletWorld->setGravity(btVector3(gx, gy, gz));
}



inline void ofxMSABulletWorldBase::update() {
	bulletWorld->stepSimulation(1.0f/60.0f, 10);
}

inline void ofxMSABulletWorldBase::debugDraw() {
	bulletWorld->debugDrawWorld();
}
