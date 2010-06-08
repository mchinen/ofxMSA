
#include "ofMain.h"
#include <stdio.h>

#ifndef 	BINNER_H
#define 	BINNER_H

//void			setupBins(int x_size, int y_size, int z_size, int nDivisionsX, int nDivisionsY, int nDivisionsZ);	// do this once! 
//void 			computeBinPosition(float xpos, float ypos, float zpos, unsigned int *bitFlagX, unsigned int *bitFlagY, unsigned int *bitFlagZ);	// MEMO: changed x,y,z to float

// min:		minimum world coordinates
// max:		maximum world coordinates
// binSize:	how big a bin should be
void setupBins(ofPoint &min, ofPoint &max, ofPoint binSize);


void computeBinPosition(ofPoint &pos, unsigned int *bitFlagX, unsigned int *bitFlagY, unsigned int *bitFlagZ);

//------------------------------------------------------


#endif

