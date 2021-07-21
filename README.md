# FamilyPortrait

Greenscreen photo tool that draws new images on top of older images (using a greenscreen to chromakey).

## Features
Can use either an SLR Camera or a webcam to capture photos.

Photos are directly saved on harddrive.

## Compatibility and building

Compatible with openFrameworks 0.11.0+.

* Windows: webcam or Canon camera using EDSDK
* Linux: webcam or SLR camera using gPhoto
* MacOS: untested (EDSDK should work)

Rename the ```addons_<system you are compiling on>.make``` to ```addons.make```. Generate the project files using the openFrameworks Project Generator.

## Required Addons

* [ofxAnimatable](https://github.com/armadillu/ofxAnimatable)
* [ofxAssetManager](https://github.com/reddoLabs/ofxAssetManager)
* [ofxCv](https://github.com/kylemcdonald/ofxCv)
* [ofxEasing](https://github.com/arturoc/ofxEasing)
* [ofxModule](https://github.com/reddoLabs/ofxModule)
* [ofxModuleGreenscreen](https://github.com/reddoLabs/ofxModuleGreenscreen)
* [ofxModulePictureExporter](https://github.com/reddoLabs/ofxModulePictureExporter)
* [ofxModuleGreenscreen](https://github.com/reddoLabs/ofxModuleGreenscreen)
* [ofxVideoRecorder](https://github.com/brinoausrino/ofxVideoRecorder)
* [ofxTextureRecorder](https://github.com/brinoausrino/ofxTextureRecorder)
* on Windows : [ofxCanon](https://github.com/brinoausrino/ofxCanon)
* on Windows : [ofxModuleCanon](https://github.com/reddoLabs/ofxModuleCanon)
* on Linux : [ofxGphoto](https://github.com/brinoausrino/ofxGphoto)
* on Linux : [ofxModuleCanon](https://github.com/reddoLabs/ofxModuleGphoto)

## Getting started

* *p* : take photo
* *f* : toggle fullscreen
* *b* : startup camera
