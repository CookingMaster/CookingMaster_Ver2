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
#include "../Utility/Counter.hpp"

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
		Counter transCounter_;
		Counter_f flameCounter_;

	public:
		NoteStateTransition(std::array<float, 4> hitJudge, float arrivalBeatTime)
		{
			noteState_ = &entity->getComponent<NoteState>();
			animator_ = &entity->getComponent<AnimatorByFrame>();

			/*
			NON �� BAD �� GOOD �� GREAT �� PARFECT �� GREAT �� GOOD �� BAD �� NON �� MISSED �Ƃ������ɑJ�ڂ���
			BAD�̎��ɓ��͂������GRAZED�ֈڍs����
			GOOD,GREAT,PARFECT�̎��ɓ��͂������HITTED�ֈڍs����
			�ȉ��̗���͊e����J�n���Ԃ̌v�Z
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

			//�~���b���t���[���ɕϊ�
			for (auto& it : hitTimeLine_)
			{
				it = millisecondToFlame(it);
			}

			noteState_->val = NoteState::State::NON;
		}

		void update() override
		{
			if (int(flameCounter_.getCurrentCount()) > hitTimeLine_[transCounter_.getCurrentCount()])
			{
				transition();
			}
			++flameCounter_;
		}

		NoteState::State getNoteState()
		{
			return noteState_->val;
		}

	private:
		//�~���b���t���[�����ɕϊ�
		float millisecondToFlame(float ms)
		{
			return ms * (60.f / 1000.f);
		}

		//��Ԃ�J�ڂ�����
		void transition()
		{
			switch (transCounter_.getCurrentCount())
			{
			case 0:	noteState_->val = NoteState::State::BAD;		break;
			case 1:	noteState_->val = NoteState::State::GOOD;		break;
			case 2:	noteState_->val = NoteState::State::GREAT;		break;
			case 3:	noteState_->val = NoteState::State::PARFECT;	break;
			case 4:	noteState_->val = NoteState::State::GREAT;		break;
			case 5:	noteState_->val = NoteState::State::GOOD;		break;
			case 6:	noteState_->val = NoteState::State::BAD;		break;
			case 7:	noteState_->val = NoteState::State::NON;		break;
			case 8:	noteState_->val = NoteState::State::MISSED;		break;
			default: return;
			}
			++transCounter_;
		}
	};
}