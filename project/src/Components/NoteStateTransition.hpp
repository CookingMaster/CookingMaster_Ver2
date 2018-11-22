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
#include "BasicComponents.hpp"
#include "../ECS/ECS.hpp"

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
		std::array<float, 4> hitJudge_;
		float arrivalBeatTime_;

		NoteState* noteState_;
		AnimatorByFrame* animator_;
		std::array<float, 9> hitTimeLine_;
		Counter transCounter_;
		Counter_f flameCounter_;

	public:
		NoteStateTransition(const std::array<float, 4>& hitJudge, float arrivalBeatTime) :
			hitJudge_(hitJudge),
			arrivalBeatTime_(arrivalBeatTime),
			transCounter_(0, 9) {}

		void initialize() override
		{
			noteState_ = &entity->getComponent<NoteState>();
			animator_ = &entity->getComponent<AnimatorByFrame>();


			//�~���b���t���[���ɕϊ�
			for (auto& it : hitJudge_)
			{
				it = millisecondToFlame(it);
			} 

			/*NON �� BAD �� GOOD �� GREAT �� PARFECT �� GREAT �� GOOD �� BAD �� NON �� MISSED �Ƃ������ɑJ�ڂ���
			BAD�̎��ɓ��͂������GRAZED�ֈڍs����
			GOOD,GREAT,PARFECT�̎��ɓ��͂������HITTED�ֈڍs����
			�ȉ��̗���͊e����J�n���Ԃ̌v�Z*/
			hitTimeLine_[0] = arrivalBeatTime_ - ((hitJudge_[3] / 2.f) + hitJudge_[2] + hitJudge_[1] + hitJudge_[0]);
			hitTimeLine_[1] = hitTimeLine_[0] + hitJudge_[0];
			hitTimeLine_[2] = hitTimeLine_[1] + hitJudge_[1];
			hitTimeLine_[3] = hitTimeLine_[2] + hitJudge_[2];
			hitTimeLine_[4] = hitTimeLine_[3] + hitJudge_[3];
			hitTimeLine_[5] = hitTimeLine_[4] + hitJudge_[2];
			hitTimeLine_[6] = hitTimeLine_[5] + hitJudge_[1];
			hitTimeLine_[7] = hitTimeLine_[6] + hitJudge_[0];
			hitTimeLine_[8] = hitTimeLine_[7] + 10.f;

			noteState_->val = NoteState::State::NON;
		}

		void update() override
		{
			if (transCounter_.isMax()) return;

			if (flameCounter_.getCurrentCount() > hitTimeLine_[transCounter_.getCurrentCount()])
			{
				transition();
			}
			++flameCounter_;
		}

		/**
		* @brief ���͂��󂯂���̃m�[�c�̏�ԑJ�ڂ��s��
		* @return bool ���肪�L�����������ۂ�(�L����������true���Ԃ�)
		*/
		bool ActionToChangeNoteState()
		{
			switch (noteState_->val)
			{
			case NoteState::State::NON:
			case NoteState::State::MISSED:
			case NoteState::State::GRAZED:
			case NoteState::State::HITTED:
				return false;

			case NoteState::State::BAD:
				noteState_->val = NoteState::State::GRAZED;
				break;

			case NoteState::State::GOOD:
			case NoteState::State::GREAT:
			case NoteState::State::PARFECT:
				noteState_->val = NoteState::State::HITTED;
				break;
			}
			return true;
		}

		//���݂̃m�[�c�̏�Ԃ��擾����
		[[nodiscard]]NoteState::State getNoteState() const
		{
			return noteState_->val;
		}

	private:
		//�~���b���t���[�����ɕϊ�
		float millisecondToFlame(float ms)
		{
			return (ms / 1000.f) * 60.f;
		}

		//��Ԃ�J�ڂ�����
		void transition()
		{
			if (noteState_->val == NoteState::State::MISSED ||
				noteState_->val == NoteState::State::GRAZED ||
				noteState_->val == NoteState::State::HITTED)
				return;

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
			case 8:	noteState_->val = NoteState::State::MISSED;
					animator_->setSpriteNum(0, 1, 2, 2);
					animator_->changeFrame(10);
					animator_->setIsEndStopAnim(true);
					entity->stopComponent<Physics>();
					entity->updateComponent<KillEntity>();
					break;
			default: return;
			}
			++transCounter_;
		}
	};
}