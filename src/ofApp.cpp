#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	if (!xml_realsense.load("realsense_settings.xml")) {
		ofLogError() << "Couldn't load file";
	}

	auto realsense_settings = xml_realsense.getChild("settings");
	auto rs_serialnumber = xml_realsense.findFirst("//serial").getValue();

	realsense = new ofxRealsense();
	realsense->setup(rs_serialnumber);

	// allocate ofImage
	depth_img.allocate(DEPTH_W, DEPTH_H, OF_IMAGE_GRAYSCALE);

	params.add(realsense->depth_params);

	gui.setup(params, "depth_camera_settings.xml");
	gui.loadFromFile("depth_camera_settings.xml");
	showGui = true;

	realsense->update_colorize_params();
}

//--------------------------------------------------------------
void ofApp::update(){

	realsense->update();

	unsigned char* value = realsense->getDepthPixels();

	srcImage.setFromPixels(value, DEPTH_W, DEPTH_H);

	// convert to gray scale
	grayImage = srcImage;

	unsigned char* pixels = grayImage.getPixels().getPixels();

	//depth_img.setFromPixels((unsigned char*)depth.get_data(), DEPTH_W, DEPTH_H, OF_IMAGE_COLOR);
	depth_img.setFromPixels(pixels, DEPTH_W, DEPTH_H, OF_IMAGE_GRAYSCALE);
	depth_img.update();

}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0);
	depth_img.draw(0, 0);

	if (showGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
	case 'b':
		//updateBgImage();
		break;
	case 'u':
		realsense->update_colorize_params();
		realsense->update_camera_params_depth();
		break;
	case 'g':
		showGui = !showGui;
		break;

	case 's':
		//saveHomography();
		break;

	case ' ':
		//calcHomography();
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
