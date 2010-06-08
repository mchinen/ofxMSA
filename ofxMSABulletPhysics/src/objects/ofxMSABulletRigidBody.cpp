/*
 *  ofxMSABulletRigidBody.cpp
 *  ofxMSABulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ofxMSABulletRigidBody.h"
#include "ofxMSABulletPhysicsUtils.h"

ofxMSABulletRigidBody::ofxMSABulletRigidBody() {
	bulletBody			= NULL;
	collisionShape		= NULL;
}

ofxMSABulletRigidBody::~ofxMSABulletRigidBody() {
	MSA_CHECK_DELETE(bulletBody);					// object owns bulletBody, so delete when done
	if(ownsCollisionShape) MSA_CHECK_DELETE(collisionShape);
}



void ofxMSABulletRigidBody::setup(ofPoint &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass) {
	this->ownsCollisionShape = ownsCollisionShape;
	this->collisionShape = collisionShape;
	
	btTransform startTransform;
	startTransform.setIdentity();
	bool isDynamic = (mass != 0.f);
	
	btVector3 localInertia(0,0,0);
	if (isDynamic) collisionShape->calculateLocalInertia(mass,localInertia);
	
	startTransform.setOrigin(ofPoint_To_btVector3(startPosition));	
	
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	bulletBody = new btRigidBody(rbInfo);
	//body->setContactProcessingThreshold(collisionShape->getContactBreakingThreshold());
	bulletBody->setActivationState(ISLAND_SLEEPING);
	
}
