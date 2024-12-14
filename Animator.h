#pragma once
#ifndef ANIMATOR_H
#include"Animation.h"

class Animator
{
	public:
		Animator(Animation* animation);
		Animator();
		void update_animation(float dt);
		void play_animation(Animation* pAnimation);
		void calculate_bone_transform(const AssimpNodeData* node, const glm::mat4& parentTransform);
		std::vector<glm::mat4>* get_final_bone_matrices();
	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		Animation* m_CurrentAnimation { nullptr };
		float m_CurrentTime{ 0.0f };
};

#endif // !ANIMATOR_H
