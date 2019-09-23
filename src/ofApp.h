#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxOsc.h"

#include "ofxRealsense.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
private:

	ofxRealsense* realsense;
	ofImage depth_img;

	// cv variables
	ofxCvColorImage srcImage;
	ofxCvGrayscaleImage 	grayImage;

	// xml
	ofXml xml_realsense;

	//gui settings--------------------
	ofxPanel gui;
	ofParameterGroup params;
	bool showGui;

};
