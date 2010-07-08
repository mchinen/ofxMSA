/*
 *  ofxMSADirManager.h
 *  Body Paint 2.0
 *
 *  Created by Mehmet Akten on 05/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxDirList.h"
#include "ofxMSAUtils.h"

///***************************************************************/
//class msaImage : public T {
//public:
//    ofPixels &getPixelsData() {
//        return myPixels;
//    }
//};

template <typename T>
class msaDirManager {
public:
	vector<T>		resources;
	int currentIndex;
	
	void setup(string path, const char *ext = NULL, string* md5 = NULL) {
		currentIndex = 0;
		ofxDirList DIR;
		if(ext) DIR.allowExt(ext);
		int numFiles = DIR.listDir(path);
		
		ofLog(OF_LOG_VERBOSE, "msaDirManager::setup( " + path + " ) | " + ofToString(numFiles) + " files loaded");
		for(int i = 0; i < numFiles; i++) {
			if(md5) {
				ofxMSACheckFileMD5(DIR.getPath(i), md5[i], true);
			} else {
				ofxMSACheckFileMD5(DIR.getPath(i), "", false);
			}
			string filename = DIR.getPath(i);
			ofLog(OF_LOG_VERBOSE, "   loading " + filename);
			resources.push_back(T());
			loadResource(filename);
		}
	}
	
	virtual void loadResource(string filename) {
	}
	
	
	T &getCurrentFast() {
		return resources[currentIndex];
	}
	
	
	T &getCurrent() {
		if(resources.size() > 0) {
			currentIndex = (currentIndex + resources.size()) % resources.size();
			return resources[currentIndex];
		} else {
			printf("*** No resources yet ***\n");
		}
	}
	
	T &getRandom() {
		currentIndex = rand() % resources.size();
		return getCurrent();
	}
	
	T &getNext() {
		currentIndex++;
		return getCurrent();
	}
	
	T &getPrev() {
		currentIndex--;
		return getCurrent();
	}
	
	T &getAt(int i) {
		currentIndex = i;
		return getCurrent();
	}
	
	int count() {
		return resources.size();
	}
};



//---------------------------
class msaImageDirManager : public msaDirManager<ofImage> {
public:
	void loadResource(string filename) {
		resources.back().loadImage(filename);
	}
};

//---------------------------
class msaSoundDirManager : public msaDirManager<ofSoundPlayer*> {
public:
	void loadResource(string filename) {
		ofSoundPlayer *s = resources.back() = new ofSoundPlayer();
		s->loadSound(filename);
	}
};

class msaMovieDirManager : public msaDirManager<ofVideoPlayer*> {
public:
	void loadResource(string filename) {
		ofVideoPlayer *s = resources.back() = new ofVideoPlayer();
		s->loadMovie(filename);
	}
};
