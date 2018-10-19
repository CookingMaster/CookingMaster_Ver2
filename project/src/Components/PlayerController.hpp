/**
* @file PlayerController.hpp
* @brief �v���C���̏��̎q�𐧌䂷��R���|�[�l���g
* @author yokota0717
* @date 2018.10.16
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Class/ResourceManager.hpp"
#include "../System/System.hpp"
#include "Renderer.hpp"
#include "Animator.hpp"
#include "../Utility/Counter.hpp"
#include "../Input/Input.hpp"

namespace ECS {
	/**
	* @brief �v���C���̏�Ԃ�ێ�����
	*/
	struct PlayerState final : public ComponentData
	{
		enum class State : short
		{
			Non,	//!����`
			Idle,	//!����
			Left,	//!���؂�
			Right,	//!�E�؂�
			Up,		//!��؂�
			Down,	//!���؂�
			Miss,	//!�~�X(�����)
			Enter,	//!����
			Clear,	//!�ȏI��
		};
		State val;
		PlayerState() : val(State::Non) {}
		explicit PlayerState(State state) 
			:
			val(state)
		{}
	};


	/**
	* @brief �v���C����p�A�j���[�^�[
	* - SpriteAnimationDraw�APlayerState���K�v
	*/
	class AnimatorPlayer final : public ComponentSystem
	{
	private:
		PlayerState* state_;
		SpriteAnimationDraw* animation_ = nullptr;
		Counter counter_;
		int frame_ = 0;					//!�A�j���[�V��������t���[����
		int bpm_ = 0;					//!���݂�BPM
		DWORD beat_ = 0;				//!�ꔏ�̒���(ms)
		DWORD start_ = 0;
		std::string soundname_ = "";	//!���݂�BGM�̓o�^��
		int indexX_BPM_ = 0;			//!BPM�ɍ��킹�Ă��鎞��X�����̃C���f�b�N�X
		int indexX_Frame_ = 0;			//!�t���[���ɍ��킹�Ă��鎞��X�����̃C���f�b�N�X
		int indexY_ = 0;				//!Y�����̃C���f�b�N�X
		int maxXnum_BPM_ = 0;			//!BPM�ɍ��킹�Ă��鎞�̕`�悷��摜��X�����̖���
		int maxXnum_Frame_ = 0;			//!�t���[���ɍ��킹�Ă��鎞�̕`�悷��摜��X�����̖���
	public:
		AnimatorPlayer(const char* soundname, const int bpm, const int frame)
			:
			soundname_(soundname),
			bpm_(bpm),
			frame_(frame)
		{}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();
			state_ = &entity->getComponent<PlayerState>();
			counter_.SetCounter(0, 1, 0, frame_);
			start_ = GetSoundCurrentTime(ResourceManager::GetSound().getHandle(soundname_));
			calcBeat();
		}
		void update() override
		{
			updateByBPM();
			if (state_->val != PlayerState::State::Idle)
			{
				updateByFrame();
			}
			else
			{
				initFrameAnimator();
			}
		}
		//!BPM�ɍ��킹���X�V����������
		void updateByBPM()
		{
			DWORD now = GetSoundCurrentTime(ResourceManager::GetSound().getHandle(soundname_));
			if (now - start_ >= beat_)
			{
				++indexX_BPM_;
				if (indexX_BPM_ >= maxXnum_BPM_)
				{
					indexX_BPM_ = 0;
				}
				start_ = now;
			}
			animation_->setIndex(indexX_BPM_);
		}
		//!�t���[���ɍ��킹���X�V����������
		void updateByFrame()
		{
			//!X�����̃C���f�b�N�X�X�V
			if (++counter_ >= frame_)
			{
				++indexX_Frame_;
				if (indexX_Frame_ >= maxXnum_Frame_)
				{
					indexX_Frame_ = 0;
				}
			}
			//!Y�����̃C���f�b�N�X�ݒ�
			switch (state_->val)
			{
			case PlayerState::State::Left:
				indexY_ = 1; break;
			case PlayerState::State::Right:
				indexY_ = 2; break;
			case PlayerState::State::Down:
				//indexY_ = 3; break;
			default:
				indexY_ = 0; break;
			}
			//!�`��摜�w��
			animation_->setIndex(indexX_Frame_ + (indexY_ * maxXnum_BPM_));
		}
		//!�t���[���ɍ��킹���A�j���[�V�����֘A�̏�����
		void initFrameAnimator()
		{
			indexX_Frame_ = 0;
			counter_.reset();
			indexY_ = 0;
		}
		//!BPM��ݒ肷��
		void setBPM(const int bpm)
		{
			bpm_ = bpm;
		}
		/**
		* @brief 1���̒���(ms)���v�Z����
		* @note 1000(ms) * 60(sec/min) / bpm(beat/min)
		*/
		void calcBeat()
		{
			beat_ = static_cast<DWORD>(1000 * (60.f / bpm_));
		}
		/**
		* @brief �摜�����̐ݒ�
		* @param xmaxBPM BPM�ɍ��킹���A�j���[�V����������Ƃ��̉摜����
		* @param xmaxFrame �t���[���ɍ��킹���A�j���[�V����������Ƃ��̉摜����
		*/
		void setSpriteNum(const int xmaxBPM, const int xmaxFrame)
		{
			maxXnum_BPM_ = xmaxBPM;
			maxXnum_Frame_ = xmaxFrame;
		}
	};


	/**
	* @brief �v���C���̏��̎q�𐧌䂷��R���|�[�l���g
	* - AnimatorPlayer���K�v
	*/
	class PlayerController final : public ComponentSystem
	{
	private:
		AnimatorPlayer* animator_ = nullptr;
		PlayerState* state_;
		Counter counter_;
		int cutAnimMaxTime_ = 180;	//!�؂郂�[�V�����̑S�̂̒���(�t���[����)�A����𒴂����Idle�ɖ߂�
	public:
		PlayerController()
		{

		}
		void initialize() override
		{
			state_ = &entity->getComponent<PlayerState>();
			counter_.SetCounter(0, 1, 0, cutAnimMaxTime_);
		}
		void update() override
		{
			std::cout << (int)state_->val << std::endl;
			tryAction();
			actionByState();
			
		}
		//!�`�悷��摜�̃C���f�b�N�X��ݒ肷��
		void setGraphIndex()
		{

		}
		/**
		* @brief ���݂̏�Ԃɍ��킹�ē��͂��󂯕t����
		* @note �v����ɃQ�[�v����Think()
		*/
		void tryAction()
		{
			switch (state_->val)
			{
			case PlayerState::State::Idle:
				if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1)
				{
					changeState(PlayerState::State::Left);
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1)
				{
					changeState(PlayerState::State::Right);
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_UP) == 1)
				{
					//changeState(PlayerState::State::Up);
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
				{
					//changeState(PlayerState::State::Down);
				}
				break;
			case PlayerState::State::Left:
				[[fallthrough]];
			case PlayerState::State::Right:
				[[fallthrough]];
			case PlayerState::State::Up:
				[[fallthrough]];
			case PlayerState::State::Down:
				if (++counter_ >= cutAnimMaxTime_)
				{
					changeState(PlayerState::State::Idle);
				}
			case PlayerState::State::Enter:
				break;
			case PlayerState::State::Clear:
				break;
			case PlayerState::State::Non:
			default:
				break;
			}

		}
		void actionByState()
		{
			switch (state_->val)
			{
			case PlayerState::State::Idle:
				break;
			case PlayerState::State::Left:
				break;
			case PlayerState::State::Right:
				break;
			case PlayerState::State::Up:
				break;
			case PlayerState::State::Down:
				break;
			case PlayerState::State::Enter:
				break;
			case PlayerState::State::Clear:
				break;
			case PlayerState::State::Non:
			default:
				break;

			}
		}
		/**
		* @brief��Ԃ�ύX����
		* @param state �ύX������
		*/
		void changeState(PlayerState::State state)
		{
			state_->val = state;
		}
	};

}