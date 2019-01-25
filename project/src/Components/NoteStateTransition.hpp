/**
* @file NoteStateTransition.hpp
* @brief �m�[�c�̏�ԑJ�ڂ�s��
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
#include "../Class/Sound.hpp"
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
			MISS,		//�~�X

			MISSED,		//������Ȃ�����
			GRAZED,		//��������
			HITTED,		//��������
		} state;

		//����
		ECS::Direction::Dir dir;

		NoteState():
			state(State::NON),
			dir(ECS::Direction::Dir::L){}
	};

	/**
	* @brief �m�[�c�̏�ԑJ�ڊǗ��}��
	* - NoteState�AAnimatorByFrame�AVelocity�ATransform�AGravity���K�v
	*/
	class NoteStateTransition : public ComponentSystem
	{
	private:
		std::array<AnimSheetData, 3> asd_;	//�A�j���[�V�����J�ڂ̂��
		std::array<float, 4> hitJudge_;
		float arrivalBeatTime_;

		std::string seName_ = "";
		NoteState* noteState_ = nullptr;
		Animator* animator_ = nullptr;
		Velocity* velocity_ = nullptr;
		Position* position_ = nullptr;
		Rotation* rotation_ = nullptr;
		Gravity* gravity_ = nullptr;
		AlphaBlend* alphaBlend_ = nullptr;

		std::array<float, 6> hitTimeLine_;
		Counter transCounter_;		//��ԑJ�ڂ̃J�E���g
		Counter deathCounter_;		//���ʂ܂ł̎��Ԍv��
		Counter_f flameCounter_;

		//������true�ɂ���ƃI�[�g���[�h�ɂȂ邼�I
		bool autoPerfectMode = 
			//*
			IS_AUTO_PLAY/*/
			false/**/;

	public:
		NoteStateTransition(const NotesData& nd, float arrivalBeatTime) :
			asd_(nd.animSData),
			hitJudge_(nd.hitJudge),
			seName_(nd.seName),
			arrivalBeatTime_(arrivalBeatTime),
			transCounter_(0, 8) {}

		void initialize() override
		{
			noteState_ = &entity->getComponent<NoteState>();
			animator_ = &entity->getComponent<Animator>();
			velocity_ = &entity->getComponent<Velocity>();
			position_ = &entity->getComponent<Position>();
			rotation_ = &entity->getComponent<Rotation>();
			gravity_ = &entity->getComponent<Gravity>();
			alphaBlend_ = &entity->getComponent<AlphaBlend>();

			/*NON �� BAD �� GOOD �� GREAT �� PARFECT �� GOOD �� MISSED �Ə�Ԃ��J�ڂ���
			BAD�̎��ɓ��͂������GRAZED�֑J�ڂ���
			GOOD,GREAT,PARFECT�̎��ɓ��͂������HITTED�֑J�ڂ���
			�ȉ��̗���͊e����J�n���Ԃ̌v�Z*/
			hitTimeLine_[0] = arrivalBeatTime_ - ((hitJudge_[3] / 2.f) + hitJudge_[2] + hitJudge_[1] + hitJudge_[0]);	//Non
			hitTimeLine_[1] = hitTimeLine_[0] + hitJudge_[0];	//BAD
			hitTimeLine_[2] = hitTimeLine_[1] + hitJudge_[1];	//GOOD
			hitTimeLine_[3] = hitTimeLine_[2] + hitJudge_[2];	//GREAT
			hitTimeLine_[4] = hitTimeLine_[3] + hitJudge_[3];	//PARFECT
			hitTimeLine_[5] = hitTimeLine_[4] + 10.f;			//MISSED

			noteState_->state = NoteState::State::NON;
		}

		void update() override
		{
			if (transCounter_.isMax()) return;

			if (autoPerfectMode && noteState_->state == NoteState::State::PARFECT)
			{
				changeNoteAnim(1, true, 5);
				noteState_->state = NoteState::State::HITTED;
				Sound se(seName_);
				se.play(false, true);
				return;
			};

			if (flameCounter_.getCurrentCount() >= hitTimeLine_[transCounter_.getCurrentCount()])
			{
				transitionAndMove();
			}
			++flameCounter_;
		}

		/**
		* @brief ���͂�󂯂���̃m�[�c�̏�ԑJ�ڂ�s��
		*/
		void ActionToChangeNoteState()
		{
			switch (noteState_->state)
			{
			case NoteState::State::BAD:	//�������Ĕ��ł���
				noteState_->state = NoteState::State::GRAZED;
				velocity_->val.x /= 1.1f;
				velocity_->val.y = -30.f;
				gravity_->val = 1.5f;
				break;

			case NoteState::State::GOOD:	//�����Ɛ؂��
			case NoteState::State::GREAT:
			case NoteState::State::PARFECT:
				changeNoteAnim(1, true, 2);
				noteState_->state = NoteState::State::HITTED;
				break;
			}
		}

		/**
		* @brief �m�[�c�̃q�b�g�������L�����ۂ���擾����
		* @return bool �q�b�g�������L����
		*/
		[[nodiscard]] bool isActiveNote() const
		{
			if (noteState_->state == NoteState::State::BAD		||
				noteState_->state == NoteState::State::GOOD		||
				noteState_->state == NoteState::State::GREAT	||
				noteState_->state == NoteState::State::PARFECT)
			{
				return true;
			}
			return false;
		}

		//���݂̃m�[�c�̏�Ԃ�擾����
		[[nodiscard]]NoteState::State getNoteState() const
		{
			return noteState_->state;
		}

		//�m�[�c�����E�ǂ��炩�痈�Ă��邩��擾����
		[[nodiscard]]ECS::Direction::Dir getNoteDir() const
		{
			return noteState_->dir;
		}

		//�m�[�c�Ɏg�p�����SE�̖��O��擾����
		[[nodiscard]]const std::string& getSEName() const
		{
			return seName_;
		}

	private:

		//��Ԃ�J�ڂƊe�m�[�c�̋�����s��
		void transitionAndMove()
		{
			switch (noteState_->state)
			{
			//�������Ԃ��������]���Ȃ�����ł���
			case NoteState::State::GRAZED:
				if (position_->val.y > (System::SCREEN_HEIGHT - 100.f))
				{
					ChangeStateMISSED();
				}
				else
				{
					rotation_->val += 40.f;
				}
				return;

			//��Ԃ�MISS��������MISSED�ɕύX����
			case NoteState::State::MISS:
				if (position_->val.y > (System::SCREEN_HEIGHT - 100.f))
				{
					ChangeStateMISSED();
				}
				return;

			//���ł̊ԍۂɔ����Ȃ��ď�����
			case NoteState::State::MISSED:
				if (animator_->isAnimEnd() && alphaBlend_->alpha > 0)
				{
					alphaBlend_->alpha -= 5;
				}
				return;
			case NoteState::State::HITTED:
				if (animator_->isAnimEnd() && alphaBlend_->alpha > 0)
				{
					alphaBlend_->alpha -= 20;
				}
				return;
			}

			switch (transCounter_.getCurrentCount())
			{
			case 0:	noteState_->state = NoteState::State::BAD;
					break;

			case 1:	noteState_->state = NoteState::State::GOOD;
					break;

			case 2:	noteState_->state = NoteState::State::GREAT;
					break;

			case 3:	noteState_->state = NoteState::State::PARFECT;
				
					break;
			case 4:	noteState_->state = NoteState::State::GOOD;
					break;

			case 5:	noteState_->state = NoteState::State::MISS;
					return;

			default: return;
			}
			++transCounter_;
		}

		//�m�[�c�̃A�j���[�V������ύX���A���t���[����ɏ�����悤�ɐݒ肷��
		void changeNoteAnim(int animMode, bool isStopMove, int animspd)
		{
			animator_->setSpriteNum(
				asd_[animMode].xmin,
				asd_[animMode].ymin,
				asd_[animMode].xmax,
				asd_[animMode].ymax,
				true);
			animator_->changeFrame(animspd);
			animator_->setIsEndStopAnim(isStopMove);
			//entity->stopComponent<Physics>();
			entity->updateComponent<KillEntity>();
		}

		//��Ԃ�MISSED(�O�`�����ĂȂ�A�j���[�V����)�ɕύX����
		void ChangeStateMISSED()
		{
			transCounter_.setCounter(5, 1, 0, 1000);
			noteState_->state = NoteState::State::MISSED;
			changeNoteAnim(2, true, 5);
			rotation_->val = 0.f;
		}
	};
}