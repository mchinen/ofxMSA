/*
 *  ofxMSABulletPhysicsUtils.h
 *  ofxMSABulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "ofTypes.h"
#include "btBulletDynamicsCommon.h"

#ifndef MSA_CHECK_DELETE
#define MSA_CHECK_DELETE(p)		if(p != NULL) { delete p; p = NULL; }
#endif


inline btVector3 ofPoint_To_btVector3(ofPoint &p) {
	return btVector3(p.x, p.y, p.z);
}


inline ofPoint btVector3_To_ofPoint(btVector3 &p) {
	return ofPoint(p.m_floats[0], p.m_floats[1], p.m_floats[2]);
}