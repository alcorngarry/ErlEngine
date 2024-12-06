#pragma once
#ifndef BONE_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "BoneInfo.h"

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	void update(float animationTime);
	glm::mat4 get_local_transform();
	std::string get_bone_name() const;
	int get_bone_id();
	int get_position_index(float animationTime);
	int get_rotation_index(float animationTime);
	int get_scale_index(float animationTime);

private:
	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;

	float get_scale_factor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 interpolation_position(float animationTime);
	glm::mat4 interpolation_rotation(float animationTime);
	glm::mat4 interpolation_scaling(float animationTime);
};

#endif // !BONE_H
