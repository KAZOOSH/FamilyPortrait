#include "FamilyPortrait.h"

using namespace ofxInterface;

namespace ofxModule {
	FamilyPortrait::FamilyPortrait(string moduleName, string settingsPath) :ModuleDrawable(moduleName, moduleName, settingsPath)
	{
		ofSetWindowPosition(settings["general"]["windowPosition"][0], settings["general"]["windowPosition"][1]);
		ofSetWindowShape(settings["general"]["dimensions"][0], settings["general"]["dimensions"][1]);
		ofSetFullscreen(settings["general"]["fullscreen"].get<bool>());

		initGui();
		initPictureExporter();

		ofAddListener(ofEvents().keyPressed, this, &FamilyPortrait::keyPressed);

		
	}
	void FamilyPortrait::update()
	{
	}
	void FamilyPortrait::draw()
	{
		scene->render();
	}
	void FamilyPortrait::stopModule()
	{
	}
	void FamilyPortrait::initGui()
	{


		// the scene
		NodeSettings s;
		s.name = "scene";
		s.size = ofVec2f(settings["general"]["dimensions"][0], settings["general"]["dimensions"][1]);

		scene = new Node();
		scene->setup(s);

		// the canvas to work with
		ofRectangle rMax(0, 0, settings["picture"]["dimCanvas"][0].get<int>(), settings["picture"]["dimCanvas"][1].get<int>());
		ofRectangle rRef(0, 0, settings["picture"]["pictureFormat"].get<float>() * 100.0f, 100.0f);
		rRef.scaleTo(rMax, OF_SCALEMODE_FIT);

		canvas = new Canvas();
		canvas->setup(round(rRef.width), round(rRef.height));


		CanvasRefNode* ref = new CanvasRefNode();
		ref->setup(scene->getWidth(), scene->getHeight(), canvas);

		scene->addChild(ref);

		// add picture layers to canvas
		Layer* layerPictures = new ofxInterface::Layer();
		LayerSettings lSettings = LayerSettings(canvas->getWidth(), canvas->getHeight(), "pictures");
		layerPictures->setup(lSettings);
		canvas->addLayer(layerPictures);

		Layer* layerCam = new ofxInterface::Layer();
		lSettings.name = "cam";
		layerCam->setup(lSettings);
		canvas->addLayer(layerCam);

		for (size_t i = 0; i < settings["general"]["nLayers"].get<int>(); i++)
		{
			ofFbo fbo;
			fbo.allocate(canvas->getWidth(), canvas->getHeight());
			fbo.begin();
			ofClear(0, 0);
			fbo.end();
			fbos.push_back(fbo);

			TextureNode* n = new TextureNode();
			TextureNodeSettings ns;
			ns.name = ofToString(i);
			ns.plane = i;
			ns.size = canvas->getSize();
			ns.texture = fbos.back().getTexture();
			n->setup(ns);
			layerPictures->addChild(n);
		}

		camTexture = new TextureNode();
		TextureNodeSettings ns;
		ns.name = "pic";
		ns.plane = 300;
		ns.size = canvas->getSize();
		camTexture->setup(ns);
		layerCam->addChild(camTexture);

		// flash
		ColorPanelSettings cps;
		cps.size = canvas->getSize();
		cps.plane = 10000;
		cps.bgColor = ofColor(255);
		cps.isActive = false;

		flash = new ColorPanel();
		flash->setup(cps);
		scene->addChild(flash);

		
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
		if (ofGetElapsedTimeMillis() - lastphoto > 4000) {
			startCapture();
			lastphoto = ofGetElapsedTimeMillis();
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
		if ((e.id == "Capture")) {
			notifyEvent(e);
		}
		else {
			camTexture->setTexture(*e.texture);

				if (isCaptureRunning) {
					// when using a canon slr and singleshot wait for the correct photo
					if (e.moduleClass == "Canon") {
						if (e.address == "photo") {
							fbos[currentLayer].begin();
							ofClear(0, 0);
							camTexture->draw();
							fbos[currentLayer].end();
							
						}
					}
					else {
						fbos[currentLayer].begin();
						ofClear(0, 0);
						camTexture->draw();
						fbos[currentLayer].end();

					}
					
					exportImage(make_shared<ofFbo>( fbos[currentLayer]), "save");

					flash->deactivate();
					updateLayerSorting();
					isCaptureRunning = false;
					currentLayer++;
					currentLayer%settings["general"]["nLayers"].get<int>();

				}
		}
	}
	void FamilyPortrait::updateLayerSorting()
	{
		auto layers = canvas->getChildWithName("pictures")->getChildren();

		int lTemp = settings["general"]["nLayers"].get<int>();

		for (size_t i = currentLayer; i < layers.size(); i++)
		{
			layers[i]->setPlane(lTemp);
			--lTemp;
		}
		for (size_t i = 0; i < currentLayer; i++)
		{
			layers[i]->setPlane(lTemp);
			--lTemp;
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
		flash->activate();

			ModuleEvent e = ModuleEvent(moduleClass, moduleName, "capture", ofJson());
			e.destId = "Capture";
			notifyEvent(e);
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
