#pragma once
#define PRODUCT "FamilyPortrait"
#define VERSION "0.1"

#include "ofMain.h"
#include "ModuleControl.h"

#ifdef TARGET_WIN32
		#include "ofxModuleCanon.h"
#elif defined( TARGET_OSX ) || defined( TARGET_LINUX )
		#include "ofxModuleGphoto.h"
#endif

#include "ofxModuleWebcam.h"
#include "ofxModuleGreenscreen.h"
#include "ofxModulePictureExporter.h"
#include "FamilyPortrait.h"

using namespace ofxModule;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ModuleControl webModule;
};
