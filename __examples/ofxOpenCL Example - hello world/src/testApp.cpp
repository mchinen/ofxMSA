#include "testApp.h"

#include "ofxOpenCL.h"

#include <mach/mach_time.h>


#define SIZE	(1024*1024)
#define REPS	200

ofxOpenCL			openCL;
float				buf[2][SIZE];
ofxOpenCLBuffer		clBuf[2];
float				scalerMultipler;

float				testBuffer[SIZE];

//--------------------------------------------------------------
void testApp::setup(){
	
	// dump everything to console
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// variables for precise timing (only works on linux + osx)
	uint64 startTime;
	mach_timebase_info_data_t info;
	mach_timebase_info(&info); 
	

	// initialize input data
	for(int i=0;i<SIZE; i++){
		buf[0][i] = i+1;
	}
	
		
	// setup openCL, load program and init kernels
	openCL.setup(CL_DEVICE_TYPE_GPU, 2);
	openCL.loadProgramFromFile("MyProgram.cl", false);
	ofxOpenCLKernel *kernelSquare	= openCL.loadKernel("square");
	ofxOpenCLKernel *kernelInverse	= openCL.loadKernel("inverse");
	ofxOpenCLKernel *kernelMultiplyScaler = openCL.loadKernel("multiplyScaler");
	

	// create openCL buffers and upload initial data
	startTime = mach_absolute_time();
	clBuf[0].initBuffer(SIZE * sizeof(float), CL_MEM_READ_WRITE, buf[0]);
	clBuf[1].initBuffer(SIZE * sizeof(float), CL_MEM_READ_WRITE);

	kernelSquare->setArg(0, clBuf[0].getCLMem());
	kernelSquare->setArg(1, clBuf[1].getCLMem());
	
	kernelInverse->setArg(0, clBuf[1].getCLMem());
	kernelInverse->setArg(1, clBuf[0].getCLMem());
	
	kernelMultiplyScaler->setArg(0, clBuf[0].getCLMem());
	kernelMultiplyScaler->setArg(1, scalerMultipler);
	kernelMultiplyScaler->setArg(2, clBuf[1].getCLMem());

	openCL.finish();	// not normally needed, but here for more precise time measurement
	printf("\nTime elapsed for openCL preparing buffers is %f milliseconds\n", (mach_absolute_time() - startTime) * info.numer / info.denom * 0.000001);
	
	
	
	// run kernels
	startTime = mach_absolute_time();
	
	size_t sizes[1] = { SIZE };
	for(int r=0; r<REPS; r++) {
		// run these kernels passing in a sizes array
		kernelSquare->run(1, sizes);
		kernelInverse->run(1, sizes);
		
		// running this one with the run1D wrapper, just to show how it works
		// actualy it does the same as the above run method (except it internally creates the array everytime its run)
		kernelMultiplyScaler->run1D(SIZE);		
	}
	openCL.finish();	// not normally needed, but here for more precise time measurement
	printf("\nTime elapsed for openCL runKernel is %f milliseconds\n", (mach_absolute_time() - startTime) * info.numer / info.denom * 0.000001);
	
	
	
	// perform operation on CPU as well to compare results
	startTime = mach_absolute_time();
	for(int r=0; r<REPS; r++) {
		for(int i=0; i<SIZE; i++) testBuffer[i] = (buf[0][i] * buf[0][i]);
		for(int i=0; i<SIZE; i++) testBuffer[i] = 1.0f/testBuffer[i];
		for(int i=0; i<SIZE; i++) testBuffer[i] = testBuffer[i] * scalerMultipler;
	}
	openCL.finish();	// not normally needed, but here for more precise time measurement
	printf("\nTime elapsed for normal operation is %f milliseconds\n", (mach_absolute_time() - startTime) * info.numer / info.denom * 0.000001);

	
	
	// read results back from openCL device
	startTime = mach_absolute_time();
	clBuf[0].read(buf[1], 0, SIZE * sizeof(float));
	printf("\nTime elapsed for read back is %f milliseconds\n", (mach_absolute_time() - startTime) * info.numer / info.denom * 0.000001);
	
	
	
	// compare results
	float diffSum = 0;
	for(int i=0; i<SIZE; i++) {
		float diff = testBuffer[i] - buf[1][i];
//		printf("input:%f outputCL:%f outputTest:%f diff:%f\n", buf[0][i], buf[1][i], testBuffer[i], diff);
	}
	printf("\n\noutput diffSum: %f\n\n", diffSum);
	
	std::exit(0);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::exit() {
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

