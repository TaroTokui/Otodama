#pragma once

#include "ofMain.h"
#include <librealsense2/rs.hpp>

static const int DEPTH_W = 848;
static const int DEPTH_H = 480;
static const int FPS_DEPTH = 90;
static const int IMAGE_SIZE = DEPTH_W * DEPTH_H;


class ofxRealsense
{

public:
	ofxRealsense() {}
	~ofxRealsense() {}

	void setup(string serialNumber);
	void update();

	void update_camera_params_depth();
	void init_depth_frames();
	void update_colorize_params();

	unsigned char* getDepthPixels();

	ofParameterGroup depth_params;

private:

	// realsense params
	rs2::pipeline pipe;
	rs2::colorizer color_map;

	rs2::frameset data;
	rs2::frame depth;// , color;
	//rs2::frame depth_bg;
	//unsigned char* pixels;

	rs2::pipeline_profile selection;
	rs2::device selected_device;
	vector<rs2::sensor> sensors;

	// Declare filters
	rs2::decimation_filter dec_filter;  // Decimation - reduces depth frame density
	rs2::threshold_filter thr_filter;   // Threshold  - removes values outside recommended range
	rs2::spatial_filter spat_filter;    // Spatial    - edge-preserving spatial smoothing
	rs2::temporal_filter temp_filter;   // Temporal   - reduces temporal noise
	rs2::hole_filling_filter hole_filter;

	// gui params
	ofParameter<int> option_visual_preset;
	ofParameter<int> option_color_scheme;
	ofParameter<float> option_min_distance;
	ofParameter<float> option_max_distance;
	ofParameter<bool> option_auto_exposure;
	ofParameter<float> option_exposure;
};
