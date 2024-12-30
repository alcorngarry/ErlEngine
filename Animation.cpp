#include"Animation.h"

glm::mat4 convert_mat4_to_glm(const aiMatrix4x4& from)
{
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

Animation::Animation(char* animationPath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	
	m_GlobalInverse = inverse(convert_mat4_to_glm(scene->mRootNode->mTransformation));
	read_heirarchy_data(m_RootNode, scene->mRootNode);
	read_missing_bones(animation, *model);
}

Animation::~Animation()
{

}

glm::mat4 Animation::get_global_inverse() const
{
	return m_GlobalInverse;
}

Bone* Animation::find_bone(const std::string & name)
{
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
		[&](const Bone& Bone)
		{
			return Bone.get_bone_name() == name;
		}
	);
	if (iter == m_Bones.end()) return nullptr;
	else return &(*iter);
}

float Animation::get_ticks_per_second() const
{
	return m_TicksPerSecond;
}

float Animation::get_duration() const
{
	return m_Duration;
}

const AssimpNodeData& Animation::get_root_node()
{
	return m_RootNode;
}

const std::map<std::string, BoneInfo>& Animation::get_bone_id_map()
{
	return m_BoneInfoMap;
}

void Animation::read_missing_bones(const aiAnimation* animation, Model &model)
{
	int size = animation->mNumChannels;

	std::map<std::string, BoneInfo> boneInfoMap = model.get_bone_info_map();//getting m_BoneInfoMap from Model class
	int& boneCount = model.get_bone_count(); //getting the m_BoneCounter from Model class

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
	}

	m_BoneInfoMap = boneInfoMap;
}

void Animation::read_heirarchy_data(AssimpNodeData & dest, const aiNode * src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = convert_mat4_to_glm(src->mTransformation);
	dest.children.reserve(src->mNumChildren);

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		read_heirarchy_data(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}