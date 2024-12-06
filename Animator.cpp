#include"Animator.h"

Animator::Animator(Animation * animation)
{
	m_CurrentTime = 0.0;
	m_CurrentAnimation = animation;

	m_FinalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

Animator::Animator()
{
}

void Animator::update_animation(float dt)
{
	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->get_ticks_per_second() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->get_duration());
		calculate_bone_transform(&m_CurrentAnimation->get_root_node(), glm::mat4(1.0f));
	}
}

void Animator::play_animation(Animation * pAnimation)
{
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
}

void Animator::calculate_bone_transform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* Bone = m_CurrentAnimation->find_bone(nodeName);

	if (Bone)
	{
		Bone->update(m_CurrentTime);
		nodeTransform = Bone->get_local_transform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	//bone info map issue that's why the T-pose
	auto boneInfoMap = m_CurrentAnimation->get_bone_id_map();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		calculate_bone_transform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::get_final_bone_matrices()
{
	return m_FinalBoneMatrices;
}