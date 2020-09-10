#pragma once
#define PRODUCT "FamilyPortrait"
#define VERSION "0.1"

#include "ofMain.h"
#include "ModuleControl.h"

#include "ofxModuleCanon.h"
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
