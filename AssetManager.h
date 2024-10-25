#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include"Model.h"
#include<map>

class AssetManager {
	public:
		AssetManager();
		void load();
		Model* get_model(unsigned int id);
	private:
		std::map<unsigned int, Model*> assetMap;
};

#endif // !ASSET_MANAGER_H