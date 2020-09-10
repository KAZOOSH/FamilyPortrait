#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofLogToConsole();

	string settingsPath = "settings.json";

	if (webModule.getModuleLoader()->getCountModulesConfig("Webcam")) {
		webModule.getModuleLoader()->addModule(new ofxModuleWebcam("Capture", settingsPath));
	}
	if (webModule.getModuleLoader()->getCountModulesConfig("Canon")) {
		webModule.getModuleLoader()->addModule(new ofxModuleCanon("Capture", settingsPath));
	}
	if (webModule.getModuleLoader()->getCountModulesConfig("FamilyPortrait")) {
		webModule.getModuleLoader()->addModule(new FamilyPortrait("FamilyPortrait", settingsPath));
	}
	if (webModule.getModuleLoader()->getCountModulesConfig("Greenscreen")) {
		webModule.getModuleLoader()->addModule(new ofxModuleGreenscreen("Greenscreen", settingsPath));
	}
	if (webModule.getModuleLoader()->getCountModulesConfig("PictureExporter")) {
		webModule.getModuleLoader()->addModule(new ofxModulePictureExporter("PictureExporter", settingsPath));
	}


	webModule.getModuleLoader()->initModuleCommunication();
}

//--------------------------------------------------------------
void ofApp::update(){
	webModule.getModuleRunner()->updateModules();
}

//--------------------------------------------------------------
void ofApp::draw(){
	webModule.getModuleRunner()->drawModules();
}
