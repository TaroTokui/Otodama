#include "ofxRealsense.h"

//--------------------------------------------------------------
void ofxRealsense::setup(string serialNumber)
{
	//Create a configuration for configuring the pipeline with a non default profile
	rs2::config cfg;

	// set target device
	cfg.enable_device(serialNumber);

	//Add desired streams to configuration
	//cfg.enable_stream(RS2_STREAM_COLOR, COLOR_W, COLOR_H, RS2_FORMAT_RGB8, FPS_COLOR);
	cfg.enable_stream(RS2_STREAM_DEPTH, DEPTH_W, DEPTH_H, RS2_FORMAT_Z16, FPS_DEPTH);

	// The pipeline profile includes a device and a selection of active streams, with specific profile.
	selection = pipe.start(cfg);

	// Retrieve the device used by the pipeline.
	selected_device = selection.get_device();

	//std::string serial_number(selected_device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	//cout << "serial number: " << serial_number << endl;

	// the list of adjacent devices
	sensors = selected_device.query_sensors();

	// select depth sensor
	auto depth_sensor = selected_device.first<rs2::depth_sensor>();

	// turn off emitter
	if (depth_sensor.supports(RS2_OPTION_EMITTER_ENABLED))
	{
		depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 1.f); // Enable emitter
		//depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0.f); // Disable emitter
	}
	if (depth_sensor.supports(RS2_OPTION_LASER_POWER))
	{
		// Query min and max values:
		auto range = depth_sensor.get_option_range(RS2_OPTION_LASER_POWER);
		depth_sensor.set_option(RS2_OPTION_LASER_POWER, range.max); // Set max power
		//depth_sensor.set_option(RS2_OPTION_LASER_POWER, 0.f); // Disable laser
	}

	// gui params
	depth_params.setName("rgb camera params");
	depth_params.add(option_visual_preset.set("visual preset", 0, 0, 3));
	depth_params.add(option_color_scheme.set("color scheme", 0, 0, 8));
	depth_params.add(option_min_distance.set("min distance", 0, 0, 16));
	depth_params.add(option_max_distance.set("max distance", 0, 0, 16));
	//depth_params.add(option_auto_exposure.set("auto exposure", false));
	//depth_params.add(option_exposure.set("exposure", 30000, 1, 165000));

	if (depth_sensor.supports(RS2_OPTION_EXPOSURE))
	{
		auto range = depth_sensor.get_option_range(RS2_OPTION_EXPOSURE);
		depth_params.add(option_exposure.set("exposure", range.def, range.min, range.max));
	}

	if (depth_sensor.supports(RS2_OPTION_ENABLE_AUTO_EXPOSURE))
	{
		auto range = depth_sensor.get_option_range(RS2_OPTION_ENABLE_AUTO_EXPOSURE);
		depth_params.add(option_auto_exposure.set("enable_auto_exposure", range.def));
	}

	// init camera params
	update_camera_params_depth();
	init_depth_frames();
	update_colorize_params();
}

//--------------------------------------------------------------
void ofxRealsense::update()
{
	data = pipe.wait_for_frames();
	rs2::frame depth_frame = data.get_depth_frame(); //Take the depth frame from the frameset
	rs2::frame filtered = depth_frame; // Does not copy the frame, only adds a reference

	//filtered = dec_filter.process(filtered);
	//filtered = thr_filter.process(filtered);
	//filtered = spat_filter.process(filtered);
	//filtered = temp_filter.process(filtered);
	filtered = hole_filter.process(filtered);

	//bool revert_disparity = false;
	//if (revert_disparity)
	//{
	//	filtered = disparity_to_depth.process(filtered);
	//}

	depth = color_map.process(filtered);
}

//--------------------------------------------------------------
void ofxRealsense::update_camera_params_depth()
{
	// select depth sensor
	auto s = selected_device.first<rs2::depth_sensor>();

	if (s.supports(RS2_OPTION_EXPOSURE) && !option_auto_exposure)
	{
		s.set_option(RS2_OPTION_EXPOSURE, option_exposure);
	}

	if (s.supports(RS2_OPTION_ENABLE_AUTO_EXPOSURE) && option_auto_exposure)
	{
		s.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, option_auto_exposure);
	}
}

//--------------------------------------------------------------
unsigned char* ofxRealsense::getDepthPixels()
{
	return (unsigned char*)depth.get_data();
}

//--------------------------------------------------------------
void ofxRealsense::init_depth_frames()
{
	data = pipe.wait_for_frames();
	//depth_bg = color_map.process(data.get_depth_frame());
}

//--------------------------------------------------------------
void ofxRealsense::update_colorize_params()
{
	// visual Preset -> Fixed
	// Color Scheme -> White to Black
	// Min Distance -> 1.2
	// Max Distance -> 3.0
	color_map.set_option(RS2_OPTION_VISUAL_PRESET, option_visual_preset);
	color_map.set_option(RS2_OPTION_COLOR_SCHEME, option_color_scheme);
	color_map.set_option(RS2_OPTION_MIN_DISTANCE, option_min_distance);
	color_map.set_option(RS2_OPTION_MAX_DISTANCE, option_max_distance);
}