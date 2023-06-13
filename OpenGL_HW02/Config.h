#pragma once

#include <string>

namespace Config
{
	const int SCR_W = 800;
	const int SCR_H = 600;

	const float MOUSE_SENSITIVITY = 1.0f;
	const float SCROLL_SENSITIVITY = 0.0005f;

	const std::string ASSET_PATH = "assets/";
	const std::string SHADER_PATH = ASSET_PATH + "shaders/";
	const std::string MODEL_PATH =  ASSET_PATH + "models/" ;
	const std::string TEXTURE_PATH = ASSET_PATH + "textures/";
}