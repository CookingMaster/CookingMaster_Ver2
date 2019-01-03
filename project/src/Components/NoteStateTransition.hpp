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
	* - NoteState�AAnimatorByFrame���K�v
	*/
	class NoteStateTransition : public ComponentSystem
	{
	private:
		std::array<AnimSheetData, 3> asd_;	//�A�j���[�V�����J�ڂ̂��
		std::array<float, 4> hitJudge_;
		float arrivalBeatTime_;

		NoteState* noteState_ = nullptr;
		Animator* animator_ = nullptr;
		std::array<float, 8> hitTimeLine_;
		Counter transCounter_;
		Counter_f flameCounter_;

		//������true�ɂ���ƃI�[�g���[�h�ɂȂ邼�I
		bool autoPerfectMode = IS_AUTO_PLAY;

	public:
		NoteStateTransition(const NotesData& nd, float arrivalBeatTime) :
			asd_(nd.animSData),
			hitJudge_(nd.hitJudge),
			arrivalBeatTime_(arrivalBeatTime),
			transCounter_(0, 8) {}

		void initialize() override
		{
			noteState_ = &entity->getComponent<NoteState>();
			animator_ = &entity->getComponent<Animator>();


			//�~���b���t���[���ɕϊ�
			for (auto& it : hitJudge_)
			{
				it = CalcurationBeat().millisecondToFrame(it);
			} 

			/*NON �� BAD �� GOOD �� GREAT �� PARFECT �� GREAT �� GOOD �� BAD �� MISSED �Ƃ������ɑJ�ڂ���
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
			hitTimeLine_[7] = hitTimeLine_[6] + 10.f;

			noteState_->state = NoteState::State::NON;
		}

		void update() override
		{
			if (transCounter_.isMax()) return;

			if (flameCounter_.getCurrentCount() >= hitTimeLine_[transCounter_.getCurrentCount()])
			{
				transition();
			}
			++flameCounter_;
		}

		/**
		* @brief ���͂��󂯂���̃m�[�c�̏�ԑJ�ڂ��s��
		*/
		void ActionToChangeNoteState()
		{
			switch (noteState_->state)
			{
			case NoteState::State::BAD:
				noteState_->state = NoteState::State::GRAZED;
				break;

			case NoteState::State::GOOD:
			case NoteState::State::GREAT:
			case NoteState::State::PARFECT:
				changeNoteAnim(1, false);
				noteState_->state = NoteState::State::HITTED;
				break;
			}
		}

		/**
		* @brief �m�[�c�̃q�b�g�������L�����ۂ����擾����
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

		//���݂̃m�[�c�̏�Ԃ��擾����
		[[nodiscard]]NoteState::State getNoteState() const
		{
			return noteState_->state;
		}

		//�m�[�c�����E�ǂ��炩�痈�Ă��邩���擾����
		[[nodiscard]]ECS::Direction::Dir getNoteDir() const
		{
			return noteState_->dir;
		}

	private:

		//��Ԃ�J�ڂ�����
		void transition()
		{
			if (noteState_->state == NoteState::State::MISSED ||
				noteState_->state == NoteState::State::GRAZED ||
				noteState_->state == NoteState::State::HITTED)
				return;

			switch (transCounter_.getCurrentCount())
			{
			case 0:	noteState_->state = NoteState::State::BAD;		
					break;
			case 1:	noteState_->state = NoteState::State::GOOD;
					break;
			case 2:	noteState_->state = NoteState::State::GREAT;
					break;
			case 3:	noteState_->state = NoteState::State::PARFECT;
				if (autoPerfectMode)
				{
					changeNoteAnim(1, false);
					noteState_->state = NoteState::State::HITTED;
					Sound se("onion");
					se.play(false,true);
				}
					break;
			case 4:	noteState_->state = NoteState::State::GREAT;	break;
			case 5:	noteState_->state = NoteState::State::GOOD;		break;
			case 6:	noteState_->state = NoteState::State::BAD;		break;
			case 7:	noteState_->state = NoteState::State::MISSED;
					changeNoteAnim(2, true);
					break;
			default: return;
			}
			++transCounter_;
		}

		//�m�[�c�̃A�j���[�V������ύX���A���t���[����ɏ�����悤�ɐݒ肷��
		void changeNoteAnim(int animMode, bool isStopMove)
		{
			animator_->setSpriteNum(
				asd_[animMode].xmin,
				asd_[animMode].ymin,
				asd_[animMode].xmax,
				asd_[animMode].ymax, 
				true);
			//animator_->changeFrame(10);
			animator_->setIsEndStopAnim(isStopMove);
			entity->stopComponent<Physics>();
			entity->updateComponent<KillEntity>();
		}
	};
}