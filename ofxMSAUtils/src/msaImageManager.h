/*
 *  ofxMSAImageManager.h
 *  Body Paint 2.0
 *
 *  Created by Mehmet Akten on 05/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxDirList.h"


/***************************************************************/
class msaImage : public ofImage {
public:
    ofPixels &getPixelsData() {
        return myPixels;
    }
};


class msaImageManager {
public:
	vector<msaImage>		images;
	int currentIndex;
	
	void setup(string path, const char *ext = NULL, string* md5 = NULL) {
		currentIndex = 0;
		ofxDirList DIR;
		if(ext) DIR.allowExt(ext);
		int numImages = DIR.listDir(path);
		//		images.reserve(numImages);
		
		ofLog(OF_LOG_VERBOSE, "msaImageManager::setup( " + path + " ) | " + ofToString(numImages) + " files loaded");
		for(int i = 0; i < numImages; i++) {
			if(md5) {
				ofxMSACheckFileMD5(DIR.getPath(i), md5[i], true);
			} else {
				ofxMSACheckFileMD5(DIR.getPath(i), "", false);
			}
			string filename = DIR.getPath(i);
			ofLog(OF_LOG_VERBOSE, "   loading " + filename);
			msaImage img;
			img.loadImage(filename);
			images.push_back(img);
		}
		
		
	}
	
	
	msaImage &getCurrentImageFast() {
		return images[currentIndex];
	}
	
	
	msaImage &getCurrentImage() {
		if(images.size() > 0) {
			currentIndex = (currentIndex + images.size()) % images.size();
			return images[currentIndex];
		} else {
			printf("*** No images yet ***\n");
		}
	}
	
	msaImage &getRandomImage() {
		currentIndex = rand() % images.size();
		return getCurrentImage();
	}
	
	msaImage &getNextImage() {
		currentIndex++;
		return getCurrentImage();
	}
	
	msaImage &getPrevImage() {
		currentIndex--;
		return getCurrentImage();
	}
	
	msaImage &getImageAt(int i) {
		currentIndex = i;
		return getCurrentImage();
	}
	
	int count() {
		return images.size();
	}
	
};