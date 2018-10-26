/**
* @file NoteStateTransition.hpp
* @brief �m�[�c�̏�ԑJ�ڂ��s��
* @author feveleK5563
* @date 2018/10/26
*/

#pragma once
#include <array>
#include "../ECS/ECS.hpp"
#include "Animator.hpp"

namespace ECS
{
	struct NoteState : public ComponentData
	{
		enum struct State : short
		{
			NON,		//����Ȃ�
			BAD,		//������
			GOOD,		//������(�})
			GREAT,		//������(��)
			PARFECT,	//������(�D)

			MISSED,		//������Ȃ�����
			GRAZED,		//��������
			HITTED,		//��������
		} val;

		NoteState(): val(State::NON){}
	};

	/**
	* @brief �m�[�c�̏�ԑJ�ڊǗ��}��
	* - NoteState�AAnimatorByFrame���K�v
	*/
	class NoteStateTransition : public ComponentSystem
	{
	private:
		NoteState* noteState_;
		AnimatorByFrame* animator_;
		std::array<float, 9> hitTimeLine_;

	public:
		NoteStateTransition(std::array<float, 4> hitJudge, float arrivalBeatTime)
		{
			noteState_ = &entity->getComponent<NoteState>();
			animator_ = &entity->getComponent<AnimatorByFrame>();

			/*
			NON �� BAD �� GOOD �� GREAT �� PARFECT �� GREAT �� GOOD �� BAD �� NON �� MISSED �Ƃ������ɑJ�ڂ���
			BAD�̎��ɓ��͂������GRAZED�ֈڍs����
			GOOD,GREAT,PARFECT�̎��ɓ��͂������HITTED�ֈڍs����
			*/
			hitTimeLine_[0] = arrivalBeatTime - (hitJudge[3] / 2.f) - hitJudge[2] - hitJudge[1] - hitJudge[0];
			hitTimeLine_[1] = hitTimeLine_[0] + hitJudge[0];
			hitTimeLine_[2] = hitTimeLine_[1] + hitJudge[1];
			hitTimeLine_[3] = hitTimeLine_[2] + hitJudge[2];
			hitTimeLine_[4] = hitTimeLine_[3] + hitJudge[3];
			hitTimeLine_[5] = hitTimeLine_[4] + hitJudge[2];
			hitTimeLine_[6] = hitTimeLine_[5] + hitJudge[1];
			hitTimeLine_[7] = hitTimeLine_[6] + hitJudge[0];
			hitTimeLine_[8] = arrivalBeatTime * 2.f;

			noteState_->val = NoteState::State::NON;
		}
	};
}