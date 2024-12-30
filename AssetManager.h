#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include"Model.h"
#include"Animator.h"
#include"SkyBox.h"
#include<map>

namespace AssetManager {
		void load();
		Model* get_model(unsigned int id);
		SkyBox* get_sky_box();
};

#endif // !ASSET_MANAGER_H