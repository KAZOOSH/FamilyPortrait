#include "FamilyPortrait.h"


namespace ofxModule {
	FamilyPortrait::FamilyPortrait(string moduleName, string settingsPath) :ModuleDrawable(moduleName, moduleName, settingsPath)
	{
		ofSetWindowPosition(settings["general"]["windowPosition"][0], settings["general"]["windowPosition"][1]);
		ofSetWindowShape(settings["general"]["dimensions"][0], settings["general"]["dimensions"][1]);
		ofSetFullscreen(settings["general"]["fullscreen"].get<bool>());

		initPictureExporter();

		ofAddListener(ofEvents().keyPressed, this, &FamilyPortrait::keyPressed);

		
	}
	void FamilyPortrait::update()
	{
	}
	void FamilyPortrait::draw()
	{
		ofRectangle out = ofRectangle(0,0,ofGetWidth(),ofGetHeight());
		if(textures.size()>0){
			ofRectangle in = ofRectangle(0,0, textures.front().getWidth(),textures.front().getHeight());

			in.scaleTo(out);

			for(auto& tex:textures){
				tex.draw(in);
			}

		}

		ofRectangle inc = ofRectangle(0,0, camTexture.getWidth(),camTexture.getHeight());
		inc.scaleTo(out);
		camTexture.draw(inc);


		if(isStartCapture){
			isStartCapture = false;
			ofSetColor(255,210);
			ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
			startCapture();
			lastphoto = ofGetElapsedTimeMillis();
		}

		if(ofGetElapsedTimeMillis() - lastphoto < 500){
			int v = ofGetElapsedTimeMillis() - lastphoto;
			ofSetColor(255,ofxeasing::map_clamp(v,0,500,210,0,ofxeasing::cubic::easeOut));
			ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
			ofSetColor(255);
		}

	}
	void FamilyPortrait::stopModule()
	{
	}

	void FamilyPortrait::initPictureExporter()
	{
		auto exportSettings = settings["PictureExporter"];

		for (auto& s : settings["general"]["exportFolder"]) {
			ExportOption option;
			if (!s["purpose"].is_null() && s["purpose"].get<string>() != "") {
				option.purpose = s["purpose"].get<string>();
			}
			if (!s["multiLayer"].is_null()) {
				option.multiLayer = s["multiLayer"].get<bool>();
			}
			if (!s["folder"].is_null()) {
				option.exportFolder = s["folder"];
			}
			if (!s["metaData"].is_null()) {
				option.metaData = s["metaData"];
			}

			if (s["style"] != nullptr) {
				option.style = s["style"].get<string>();
			}

			for (auto& style : exportSettings["styles"]) {
				if (style["id"].get<string>() == option.style) {
					option.format = style["outputType"].get<string>();
				}
			}

			exportOptions.push_back(option);
		}
	}
	void FamilyPortrait::keyPressed(ofKeyEventArgs & e)
	{
		if (e.key == 'p' && ofGetElapsedTimeMillis() - lastphoto > 500 && !isCaptureRunning) {
			isStartCapture = true;

		}

		if (e.key == 'f') {
			ofToggleFullscreen();
		}

		if (e.key == 'b') {
			ofJson msg;
			msg["value"] = false;
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, "setIdle", msg);
			e.destId = "Capture";
			notifyEvent(e);
		}
	}
	void FamilyPortrait::proceedModuleEvent(ModuleEvent & e)
	{


			//camTexture = *e.texture;
				if (isCaptureRunning) {
							if (textures.size() > settings["general"]["nLayers"].get<int>()){
								textures.pop_front();
							}

							int h = settings["general"]["dimensions"][1];
							int w = h*e.texture->getWidth()/e.texture->getHeight();

							textures.push_back(ofFbo());
							textures.back().allocate(w,h);
							textures.back().begin();
							ofClear(0, 0);
							e.texture->draw(0,0,w,h);
							textures.back().end();

					exportImage(make_shared<ofFbo>( textures.back()), "save");

					//flash->deactivate();
					isCaptureRunning = false;
					lastphoto = ofGetElapsedTimeMillis();

		}else{
					if(!camTexture.isAllocated()){
						camTexture.allocate(e.texture->getWidth()- settings["general"]["liveViewBorder"][0].get<int>()*2,e.texture->getHeight()- settings["general"]["liveViewBorder"][1].get<int>()*2);
					}
					camTexture.begin();
					ofClear(0,0);
					e.texture->draw(- settings["general"]["liveViewBorder"][0].get<int>(),- settings["general"]["liveViewBorder"][1].get<int>());
					camTexture.end();
				}
	}

	void FamilyPortrait::stopStream()
	{
		ofJson msg;
		msg["value"] = true;
		ModuleEvent e = ModuleEvent(moduleClass, moduleName, "setIdle", msg);
		e.destId = "Capture";
		notifyEvent(e);
	}
	void FamilyPortrait::startStream()
	{
		ofJson msg;
		msg["value"] = false;
		ModuleEvent e = ModuleEvent(moduleClass, moduleName, "setIdle", msg);
		e.destId = "Capture";
		notifyEvent(e);
	}
	void FamilyPortrait::startCapture()
	{

		isCaptureRunning = true;
		//flash->activate();
		if(settings["general"]["photoCapture"].get<bool>()){
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, "capture", ofJson());
			e.destId = "Capture";
			notifyEvent(e);
		}

	}
	void FamilyPortrait::exportImage(shared_ptr<ofFbo> fbo, string purpose, string filename, ofJson metaData)
	{
		for (auto& option : exportOptions) {
			if ((option.purpose) == purpose) {
				ofJson renderSettings;
				renderSettings["paths"] = option.exportFolder;
				if (option.style != "") {
					renderSettings["style"] = option.style;
				}
				else {
					renderSettings["style"] = "image";
				}
				if (filename != "") {
					renderSettings["filename"] = filename;
				}
				else {
					renderSettings["filename"] = "[yy]-[mm]-[dd]-[s]";
				}
				if (option.metaData) {
					renderSettings["metaData"] = metaData;
				}
				notifyEvent(fbo, "exportImage", renderSettings);
			}
		}
	}
}
