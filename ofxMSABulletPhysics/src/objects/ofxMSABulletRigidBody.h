/*
 *  ofxMSABulletObject.h
 *  ofxMSABulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

/* TODO
 add ownership for collision shape, so it can delete or not
 */

#include "ofTypes.h"
#include "btBulletDynamicsCommon.h"

class ofxMSABulletRigidBody {
public:
	ofxMSABulletRigidBody();
	virtual	~ofxMSABulletRigidBody();

	// initialize the rigidbody with the given collisionShape and startPosition
	// ownsCollisionShape is for advanced (and internal use) only
	void					setup(ofPoint &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass = 1);
	btCollisionShape		*getCollisionShape();	
	btRigidBody				*getBulletBody();
	
//	void					setMass(float m = 1);
//	float					getMass();	
	
	bool					isFixed();
	bool					isFree();
	void					makeFixed();
	void					makeFree();	
	
protected:
//	float					mass;
	bool					ownsCollisionShape;		// whether the object owns (and can delete) the collision shape
	btRigidBody				*bulletBody;
	btCollisionShape		*collisionShape;
};


inline btCollisionShape		*ofxMSABulletRigidBody::getCollisionShape() {
	return collisionShape;
}

inline btRigidBody			*ofxMSABulletRigidBody::getBulletBody() {
	return bulletBody;
}
//
//inline void					ofxMSABulletRigidBody::setMass(float m) {
//	mass = m;
//}
//
//inline float				ofxMSABulletRigidBody::getMass() {
//	return mass;
//}
//

inline bool					ofxMSABulletRigidBody::isFixed() {
//	return (mass == 0);
}

inline bool					ofxMSABulletRigidBody::isFree() {
}

inline void					ofxMSABulletRigidBody::makeFixed() {
}

inline void					ofxMSABulletRigidBody::makeFree() {
}