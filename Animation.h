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
	std::vector<AssimpNodeData> children;
};

class Animation
{
	public:
		Animation() = default;
		Animation(char* animationPath, Model* model);
		~Animation();

		Bone* find_bone(const std::string& name);

		float get_ticks_per_second() const;
		float get_duration() const;
		const AssimpNodeData& get_root_node();
		const std::map<std::string, BoneInfo>& get_bone_id_map();
		glm::mat4 get_global_inverse() const;

	private:
		float m_Duration;
		int m_TicksPerSecond;
		std::vector<Bone> m_Bones;
		AssimpNodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		glm::mat4 m_GlobalInverse;

		void read_missing_bones(const aiAnimation* animation, Model& model);
		void read_heirarchy_data(AssimpNodeData& dest, const aiNode* src);
};

#endif // !ANIMATION_H
