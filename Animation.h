#pragma once
#ifndef ANIMATION_H
#include <map>
#include"Bone.h"
#include"BoneInfo.h"
#include"Model.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
	public:
		Animation() = default;
		Animation(char* animationPath, Model* model);
		~Animation();

		Bone* find_bone(const std::string& name);

		float get_ticks_per_second();
		float get_duration();
		const AssimpNodeData& get_root_node();
		const std::map<std::string, BoneInfo>& get_bone_id_map();

	private:
		float m_Duration;
		int m_TicksPerSecond;
		std::vector<Bone> m_Bones;
		AssimpNodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;

		void read_missing_bones(const aiAnimation* animation, Model& model);
		void read_heirarchy_data(AssimpNodeData& dest, const aiNode* src);
};

#endif // !ANIMATION_H
