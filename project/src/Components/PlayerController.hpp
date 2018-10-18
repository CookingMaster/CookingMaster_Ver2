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
		explicit PlayerState() : val(State::Non) {}
	};
	/**
	* @brief �v���C����p�A�j���[�^�[
	* - SpriteAnimationDraw�APlayerState���K�v
	*/
	class AnimatorPlayer final : public ComponentSystem
	{
	private:
		PlayerState* state_;
		AnimatorByBPM* animatorBPM_ = nullptr;
		AnimatorByFrame* animatorFrame_ = nullptr;

	public:
		AnimatorPlayer() {}
		void initialize() override
		{
			animatorBPM_ = &entity->getComponent<AnimatorByBPM>();
			animatorFrame_ = &entity->getComponent<AnimatorByFrame>();
			state_ = &entity->getComponent<PlayerState>();
		}
		void update() override
		{
			switch (state_->val)
			{
			case PlayerState::State::Idle:
				entity->stopComponent<AnimatorByFrame>();
				entity->updateComponent<AnimatorByBPM>();
				break;
			case PlayerState::State::Left:
				[[fallthrough]];
			case PlayerState::State::Right:
				[[fallthrough]];
			case PlayerState::State::Up:
				[[fallthrough]];
			case PlayerState::State::Down:
				[[fallthrough]];
			case PlayerState::State::Enter:
				[[fallthrough]];
			case PlayerState::State::Clear:
				[[fallthrough]];
			case PlayerState::State::Non:
				[[fallthrough]];
			default:
				entity->stopComponent<AnimatorByBPM>();
				entity->updateComponent<AnimatorByFrame>();
				break;
			}
		}

	};


	/**
	* @brief �v���C���̏��̎q�𐧌䂷��R���|�[�l���g
	* - PlayerState���K�v
	*/
	class PlayerController final : public ComponentSystem
	{
	private:
		AnimatorPlayer* animator_ = nullptr;
		PlayerState * state_;
		int indexX_ = 0;
		int indexY_ = 0;
		Counter animCounter_;
	public:
		PlayerController()
		{

		}
		void initialize() override
		{
			if (!entity->hasComponent<AnimatorPlayer>())
			{
				entity->addComponent<PlayerState>();
			}
		}
		void update() override
		{
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
					changeState(PlayerState::State::Up);
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
				{
					changeState(PlayerState::State::Down);
				}
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

		void changeState(PlayerState::State state)
		{
			state_->val = state;
		}

		/**
		* @brief �A�j���[�V�����p�̃J�E���^��ݒ肷��
		*/
		void initAnimCounter(const int add, const int max)
		{
			animCounter_.SetCounter(0, add, 0, max);
		}
	};

}