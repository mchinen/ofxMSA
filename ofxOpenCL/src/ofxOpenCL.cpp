/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#include "ofxOpenCL.h"
#include "ofxOpenCLProgram.h"
#include "ofxOpenCLKernel.h"


ofxOpenCL *ofxOpenCL::currentOpenCL = NULL;


ofxOpenCL::ofxOpenCL() {
	ofLog(OF_LOG_VERBOSE, "ofxOpenCL::ofxOpenCL");
	isSetup		= false;
	clContext	= NULL;
	clDevice	= NULL;
	clQueue		= NULL;
}

ofxOpenCL::~ofxOpenCL() {
	ofLog(OF_LOG_VERBOSE, "ofxOpenCL::~ofxOpenCL");
	
	clFinish(clQueue);
	
	for(int i=0; i<memObjects.size(); i++) delete memObjects[i];	// FIX
	for(map<string, ofxOpenCLKernel*>::iterator it = kernels.begin(); it !=kernels.end(); ++it) delete (ofxOpenCLKernel*)it->second;
	for(int i=0; i<programs.size(); i++) delete programs[i];
	clReleaseCommandQueue(clQueue);
    clReleaseContext(clContext);
}


void ofxOpenCL::setup(int clDeviceType, int numDevices) {
	ofLog(OF_LOG_VERBOSE, "ofxOpenCL::setup " + ofToString(clDeviceType) + ", " + ofToString(numDevices));
	
	if(isSetup) {
		ofLog(OF_LOG_VERBOSE, "... already setup. returning");
		return;
	}
	
	cl_int err;
	
	int numDevicesToUse = createDevice(clDeviceType, numDevices);
	clContext = clCreateContext(NULL, numDevicesToUse, &clDevice, NULL, NULL, &err);
	if(clContext == NULL) {
        ofLog(OF_LOG_ERROR, "Error creating clContext.");
		assert(err != CL_INVALID_PLATFORM);
		assert(err != CL_INVALID_VALUE);
		assert(err != CL_INVALID_DEVICE);
		assert(err != CL_INVALID_DEVICE_TYPE);
		assert(err != CL_DEVICE_NOT_AVAILABLE);
		assert(err != CL_DEVICE_NOT_FOUND);
		assert(err != CL_OUT_OF_HOST_MEMORY);
		assert(false);
    }
	
	
	createQueue();
}	


void ofxOpenCL::setupFromOpenGL() {
	ofLog(OF_LOG_VERBOSE, "ofxOpenCL::setupFromOpenGL ");
	
	if(isSetup) {
		ofLog(OF_LOG_VERBOSE, "... already setup. returning");
		return;
	}
	
	cl_int err;
	
	createDevice(CL_DEVICE_TYPE_GPU, 1);
	
#ifdef TARGET_OSX	
	CGLContextObj kCGLContext = CGLGetCurrentContext();
	CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
	cl_context_properties properties[] = { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup, 0 };
#else
	ofLog(OF_LOG_ERROR, "ofxOpenCL::setupFromOpenGL() only implemented for mac osx at the moment.\nIf you know how to do this for windows/linux please go ahead and implement it here.");
	assert(false);
#endif

	clContext = clCreateContext(properties, 0, 0, NULL, NULL, &err);
	if(clContext == NULL) {
        ofLog(OF_LOG_ERROR, "Error creating clContext.");
		assert(err != CL_INVALID_PLATFORM);
		assert(err != CL_INVALID_VALUE);
		assert(err != CL_INVALID_DEVICE);
		assert(err != CL_INVALID_DEVICE_TYPE);
		assert(err != CL_DEVICE_NOT_AVAILABLE);
		assert(err != CL_DEVICE_NOT_FOUND);
		assert(err != CL_OUT_OF_HOST_MEMORY);
		assert(false);
    }

	createQueue();
}	


cl_device_id& ofxOpenCL::getDevice() {
	return clDevice;
}


cl_context& ofxOpenCL::getContext() {
	return clContext;
}

cl_command_queue& ofxOpenCL::getQueue() {
	return clQueue;
}



ofxOpenCLProgram* ofxOpenCL::loadProgramFromFile(string filename, bool isBinary) { 
	ofLog(OF_LOG_VERBOSE, "ofxOpenCL::loadProgramFromFile");
	ofxOpenCLProgram *p = new ofxOpenCLProgram();
	p->loadFromFile(filename, isBinary);
	programs.push_back(p);
	return p;
}


ofxOpenCLProgram* ofxOpenCL::loadProgramFromSource(string source) {
	ofLog(OF_LOG_VERBOSE, "ofxOpenCL::loadProgramFromSource");
	ofxOpenCLProgram *p = new ofxOpenCLProgram();
	p->loadFromSource(source);
	programs.push_back(p);
	return p;
} 


ofxOpenCLKernel* ofxOpenCL::loadKernel(string kernelName, ofxOpenCLProgram *program) {
	ofLog(OF_LOG_VERBOSE, "ofxOpenCL::loadKernel " + kernelName + ", " + ofToString((int)program));
	if(program == NULL) program = programs[programs.size() - 1];
	ofxOpenCLKernel *k = program->loadKernel(kernelName);
	kernels[kernelName] = k;
	return k;
}


ofxOpenCLBuffer* ofxOpenCL::createBuffer(int numberOfBytes, cl_mem_flags memFlags, void *dataPtr, bool blockingWrite) {
	ofxOpenCLBuffer *clBuffer = new ofxOpenCLBuffer();
	clBuffer->initBuffer(numberOfBytes, memFlags, dataPtr, blockingWrite);
	memObjects.push_back(clBuffer);
	return clBuffer;
}


ofxOpenCLBuffer* ofxOpenCL::createBufferFromGLObject(GLuint glBufferObject, cl_mem_flags memFlags) {
	ofxOpenCLBuffer *clBuffer = new ofxOpenCLBuffer();
	clBuffer->initFromGLObject(glBufferObject, memFlags);
	memObjects.push_back(clBuffer);
	return clBuffer;
}


ofxOpenCLImage* ofxOpenCL::createImage2D(int width, int height, cl_channel_order imageChannelOrder, cl_channel_type imageChannelDataType, cl_mem_flags memFlags, void *dataPtr, bool blockingWrite) {
	return createImage3D(width, height, 1, imageChannelOrder, imageChannelDataType, memFlags, dataPtr, blockingWrite);
}


ofxOpenCLImage* ofxOpenCL::createImageFromTexture(ofTexture &tex, cl_mem_flags memFlags, int mipLevel) {
	ofxOpenCLImage *clImage = new ofxOpenCLImage();
	clImage->initFromTexture(tex, memFlags, mipLevel);
	memObjects.push_back(clImage);
	return clImage;
}

ofxOpenCLImage* ofxOpenCL::createImageWithTexture(int width, int height, int glType, cl_mem_flags memFlags) {
	ofxOpenCLImage *clImage = new ofxOpenCLImage();
	clImage->initWithTexture(width, height, glType, memFlags);
	memObjects.push_back(clImage);
	return clImage;
}


ofxOpenCLImage* ofxOpenCL::createImage3D(int width, int height, int depth, cl_channel_order imageChannelOrder, cl_channel_type imageChannelDataType, cl_mem_flags memFlags, void *dataPtr, bool blockingWrite) {
	ofxOpenCLImage *clImage = new ofxOpenCLImage();
	clImage->initWithoutTexture(width, height, height, imageChannelOrder, imageChannelDataType, memFlags, dataPtr, blockingWrite);
	memObjects.push_back(clImage);
	return clImage;
}



ofxOpenCLKernel* ofxOpenCL::kernel(string kernelName) {
	return kernels[kernelName];
}

void ofxOpenCL::flush() {
	clFlush(clQueue);
}


void ofxOpenCL::finish() {
	clFinish(clQueue);
}



int ofxOpenCL::createDevice(int clDeviceType, int numDevices) {
	cl_int err;
	cl_uint numDevicesFound;
	
    err = clGetDeviceIDs(NULL, clDeviceType, numDevices, &clDevice, &numDevicesFound);
    if(err != CL_SUCCESS) {
        ofLog(OF_LOG_ERROR, "Error creating clDevice.");
		assert(false);
    }	
	
	ofLog(OF_LOG_VERBOSE, ofToString(numDevicesFound, 0) + " devices found\n");
	
	int numDevicesToUse = min((int)numDevicesFound, numDevices);
	
	for(int i=0; i<numDevicesToUse; i++) {
		size_t	size;
		cl_device_id &d = (&clDevice)[i];
		err = clGetDeviceInfo(d, CL_DEVICE_VENDOR, sizeof(info.vendorName), info.vendorName, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_NAME, sizeof(info.deviceName), info.deviceName, &size);
		err |= clGetDeviceInfo(d, CL_DRIVER_VERSION, sizeof(info.driverVersion), info.driverVersion, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_VERSION, sizeof(info.deviceVersion), info.deviceVersion, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(info.maxComputeUnits), &info.maxComputeUnits, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(info.maxWorkItemDimensions), &info.maxWorkItemDimensions, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(info.maxWorkItemSizes), &info.maxWorkItemSizes, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(info.maxWorkGroupSize), &info.maxWorkGroupSize, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(info.maxClockFrequency), &info.maxClockFrequency, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(info.maxMemAllocSize), &info.maxMemAllocSize, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_IMAGE_SUPPORT, sizeof(info.imageSupport), &info.imageSupport, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(info.maxReadImageArgs), &info.maxReadImageArgs, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(info.maxWriteImageArgs), &info.maxWriteImageArgs, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(info.image2dMaxWidth), &info.image2dMaxWidth, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(info.image2dMaxHeight), &info.image2dMaxHeight, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(info.image3dMaxWidth), &info.image3dMaxWidth, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(info.image3dMaxHeight), &info.image3dMaxHeight, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(info.image3dMaxDepth), &info.image3dMaxDepth, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_SAMPLERS, sizeof(info.maxSamplers), &info.maxSamplers, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(info.maxParameterSize), &info.maxParameterSize, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(info.globalMemCacheSize), &info.globalMemCacheSize, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(info.globalMemSize), &info.globalMemSize, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(info.maxConstantBufferSize), &info.maxConstantBufferSize, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(info.maxConstantArgs), &info.maxConstantArgs, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(info.localMemSize), &info.localMemSize, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(info.errorCorrectionSupport), &info.errorCorrectionSupport, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(info.profilingTimerResolution), &info.profilingTimerResolution, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_ENDIAN_LITTLE, sizeof(info.endianLittle), &info.endianLittle, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_PROFILE, sizeof(info.profile), info.profile, &size);
		err |= clGetDeviceInfo(d, CL_DEVICE_EXTENSIONS, sizeof(info.extensions), info.extensions, &size);
		
		if(err != CL_SUCCESS) {
			ofLog(OF_LOG_ERROR, "Error getting clDevice information.");
			assert(false);
		}
		
		ofLog(OF_LOG_VERBOSE, getInfoAsString());
	}
	
	
	return numDevicesToUse;
}

string ofxOpenCL::getInfoAsString() {
	return string("\n\n*********\nofxOpenCL Device information:") + 
	"\n vendorName.................." + string((char*)info.vendorName) + 
	"\n deviceName.................." + string((char*)info.deviceName) + 
	"\n driverVersion..............." + string((char*)info.driverVersion) +
	"\n deviceVersion..............." + string((char*)info.deviceVersion) +
	"\n maxComputeUnits............." + ofToString(info.maxComputeUnits, 0) +
	"\n maxWorkItemDimensions......." + ofToString(info.maxWorkItemDimensions, 0) +
	"\n maxWorkItemSizes[0]........." + ofToString(info.maxWorkItemSizes[0], 0) + 
	"\n maxWorkGroupSize............" + ofToString(info.maxWorkGroupSize, 0) +
	"\n maxClockFrequency..........." + ofToString(info.maxClockFrequency, 0) +
	"\n maxMemAllocSize............." + ofToString(info.maxMemAllocSize/1024.0f/1024.0f, 3) + " MB" + 
	"\n imageSupport................" + (info.imageSupport ? "YES" : "NO") +
	"\n maxReadImageArgs............" + ofToString(info.maxReadImageArgs, 0) +
	"\n maxWriteImageArgs..........." + ofToString(info.maxWriteImageArgs, 0) +
	"\n image2dMaxWidth............." + ofToString(info.image2dMaxWidth, 0) +
	"\n image2dMaxHeight............" + ofToString(info.image2dMaxHeight, 0) +
	"\n image3dMaxWidth............." + ofToString(info.image3dMaxWidth, 0) +
	"\n image3dMaxHeight............" + ofToString(info.image3dMaxHeight, 0) +
	"\n image3dMaxDepth............." + ofToString(info.image3dMaxDepth, 0) +
	"\n maxSamplers................." + ofToString(info.maxSamplers, 0) +
	"\n maxParameterSize............" + ofToString(info.maxParameterSize, 0) +
	"\n globalMemCacheSize.........." + ofToString(info.globalMemCacheSize/1024.0f/1024.0f, 3) + " MB" + 
	"\n globalMemSize..............." + ofToString(info.globalMemSize/1024.0f/1024.0f, 3) + " MB" +
	"\n maxConstantBufferSize......." + ofToString(info.maxConstantBufferSize/1024.0f, 3) + " KB"
	"\n maxConstantArgs............." + ofToString(info.maxConstantArgs, 0) +
	"\n localMemSize................" + ofToString(info.localMemSize/1024.0f, 3) + " KB"
	"\n errorCorrectionSupport......" + (info.errorCorrectionSupport ? "YES" : "NO") +
	"\n profilingTimerResolution...." + ofToString(info.profilingTimerResolution, 0) +
	"\n endianLittle................" + ofToString(info.endianLittle, 0) +
	"\n profile....................." + string((char*)info.profile) +
	"\n extensions.................." + string((char*)info.extensions) +
	"\n*********\n\n";
}


void ofxOpenCL::createQueue() {
	clQueue = clCreateCommandQueue(clContext, clDevice, 0, NULL);
    if(clQueue == NULL) {
        ofLog(OF_LOG_ERROR, "Error creating command queue.");
		assert(false);
    }
	
	isSetup = true;
	currentOpenCL = this;
}
