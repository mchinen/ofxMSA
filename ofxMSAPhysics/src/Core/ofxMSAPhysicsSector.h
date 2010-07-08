/*
 *  ofxMSAPhysicsSector.h
 *  ofxMSAPhysics demo
 *
 *  Created by Mehmet Akten on 09/06/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxMSAParticle.h"
#include <vector>

class ofxMSAPhysicsSector {
public:
	void	checkSectorCollisions();
	
	void	addParticle(ofxMSAParticle *p) {
		_particles.push_back(p);
	}

	void	clear() {
		_particles.clear();
	}

	//	void	checkParticle(ofxMSAParticle *p);

protected:
	bool	checkCollisionBetween(ofxMSAParticle *a, ofxMSAParticle* b);
	std::vector<ofxMSAParticle*>	_particles;
};
