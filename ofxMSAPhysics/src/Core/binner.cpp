#include "binner.h"



//------------------------------------------------------
static ofPoint		nBinDivisions;		// must be odd numbers
static ofPoint		binDivisionSize;
static ofPoint		binDivisionSizeInv;
static ofPoint		binDivisionSubBoundaryMin;
static ofPoint		binDivisionSubBoundaryMax;
static ofPoint		binWorldMin;
static ofPoint		binWorldMax;
static ofPoint		binWorldSize;


//static int 			nBinDivisions.x; // must be an odd number
//static int 			nBinDivisions.y; // must be an odd number
//static int			nBinDivisionsZ;
//
//static float 		binDivisionXSize;
//static float		binDivisionSizeInv.x;
//static float 		binDivisionSubBoundaryMin.x, binDivisionSubBoundaryMax.x;
//static float 		binDivisionYSize;
//static float 		binDivisionSizeInv.y;
//static float 		binDivisionSubBoundaryMin.y, binDivisionSubBoundaryMax.y;
//static float 		binDivisionZSize;
//static float 		binDivisionSizeInv.z;
//static float 		binDivisionSubBoundaryMin.z, binDivisionSubBoundaryMax.z;


// MEMO: changed x,y,z, to float
void computeBinPosition(ofPoint &pos, unsigned int *bitFlagX, unsigned int *bitFlagY, unsigned int *bitFlagZ) {
	int posBin1, posBin2;
	*bitFlagX = 0x00000000;
	*bitFlagY = 0x00000000;
	*bitFlagZ = 0x00000000;

	float xpos = pos.x - binWorldMin.x;
	float ypos = pos.y - binWorldMin.y;
	float zpos = pos.z - binWorldMin.z;


	if(xpos < 0) xpos = 0;
	else if (xpos > binWorldSize.x) xpos = binWorldSize.x;

	if(ypos < 0) ypos = 0;
	else if (ypos > binWorldSize.y) ypos = binWorldSize.y;

	if(zpos < 0) zpos = 0;
	else if (zpos > binWorldSize.z) zpos = binWorldSize.z;

	// compute for X
	posBin1 = (int)(xpos * binDivisionSizeInv.x);
	*bitFlagX |= (0x00000001 << (posBin1 * 2));

	if (xpos > binDivisionSubBoundaryMin.x && xpos < binDivisionSubBoundaryMax.x){
		posBin2 = (int)((xpos - binDivisionSubBoundaryMin.x) * binDivisionSizeInv.x);
		*bitFlagX |=  (0x00000001 << (1 + posBin2 * 2));
	}

	// compute for Y
	posBin1 = (int)(ypos * binDivisionSizeInv.y);
	*bitFlagY |= (0x00000001 << ( posBin1 * 2));

	if (ypos > binDivisionSubBoundaryMin.y && ypos < binDivisionSubBoundaryMax.y){
		posBin2 = (int)((ypos - binDivisionSubBoundaryMin.y) * binDivisionSizeInv.y);
		*bitFlagY |=  (0x00000001 << (1 + posBin2 * 2));
	}

	// compute for Z
	posBin1 = (int)(zpos * binDivisionSizeInv.z);
	*bitFlagZ |= (0x00000001 << ( posBin1 * 2));

	if (zpos > binDivisionSubBoundaryMin.z && zpos < binDivisionSubBoundaryMax.z){
		posBin2 = (int)((zpos - binDivisionSubBoundaryMin.z) * binDivisionSizeInv.z);
		*bitFlagZ |=  (0x00000001 << (1 + posBin2 * 2));
	}
}

//-------------------------------------
// MEMO: added min, max instead of world size
// MEMO: uses ofPoint
void setupBins(ofPoint &min, ofPoint &max, ofPoint binSize) {
	binWorldMin			= min;
	binWorldMax			= max;
	binWorldSize		= binWorldMax - binWorldMin;

	// if binsize passed is 0, just make it the size of the world in that dimension (so 1 bin)
	if(binSize.x == 0) binSize.x = binWorldSize.x;
	if(binSize.y == 0) binSize.y = binWorldSize.y;
	if(binSize.z == 0) binSize.z = binWorldSize.z;

	binDivisionSize		= binSize;//orldSize / nBinDivisions;
	binDivisionSizeInv	= ofPoint(1, 1, 1) / binDivisionSize;
	nBinDivisions		= binWorldSize / binDivisionSize;

	// TODO: these look dodgy
	binDivisionSubBoundaryMin = binDivisionSize / 2.0f;
	binDivisionSubBoundaryMax = binWorldSize - binDivisionSize / 2.0f;
}

//void setupBins(int x_size, int y_size, int z_size, int nDivisionsX, int nDivisionsY, int nDivisionsZ){
//
//	nBinDivisions.x = nDivisionsX;
//	nBinDivisions.y = nDivisionsY;
//	nBinDivisions.z = nDivisionsZ;
//
//	binDivisionXSize = x_size / (float)nBinDivisions.x;
//	binDivisionYSize = y_size / (float)nBinDivisions.y;
//	binDivisionZSize = z_size / (float)nBinDivisions.z;
//
//	binDivisionSizeInv.x = 1.0f/binDivisionXSize;
//	binDivisionSizeInv.y = 1.0f/binDivisionYSize;
//	binDivisionSizeInv.z = 1.0f/binDivisionZSize;
//
//	binDivisionSubBoundaryMin.x = binDivisionXSize / 2.0f;
//	binDivisionSubBoundaryMax.x = x_size - binDivisionXSize / 2.0f;
//
//	binDivisionSubBoundaryMin.y = binDivisionYSize / 2.0f;
//	binDivisionSubBoundaryMax.y = y_size - binDivisionYSize / 2.0f;
//
//	binDivisionSubBoundaryMin.z = binDivisionZSize / 2.0f;
//	binDivisionSubBoundaryMax.z = z_size - binDivisionZSize / 2.0f;
//}

//-------------------------------------
