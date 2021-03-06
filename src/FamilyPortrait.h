#pragma once

#include "ofMain.h"
#include "ModuleDrawable.h"
#include "ofxEasing.h"

namespace ofxModule {
	struct ExportOption {
		string purpose = "save";
		ofJson exportFolder;
		string style = "image";
		bool multiLayer = false;
		string format = "png";
		bool metaData = false;
	};

	class FamilyPortrait: public ModuleDrawable
	{
	public:
		FamilyPortrait(string moduleName = "FamilyPortrait", string settingsPath = "settings.json");
		void update();
		void draw();
		void stopModule();

		void initGui();
		void initPictureExporter();
		void keyPressed(ofKeyEventArgs &e);

	protected:
		void proceedModuleEvent(ModuleEvent& e);


		void stopStream();
		void startStream();
		void startCapture();

		void exportImage(shared_ptr<ofFbo> fbo, string purpose, string filename = "", ofJson metaData = {});

	private:

		std::deque<ofFbo> textures;
		ofFbo camTexture;
		
		ofFbo imageFbo;

		bool isStartCapture = false;
		bool isCaptureRunning = false;

		vector<ExportOption> exportOptions;
		
		long lastphoto = 0;

	};
}

