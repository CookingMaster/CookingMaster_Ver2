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
	* @brief �v���C���̏��̎q�𐧌䂷��R���|�[�l���g
	* - AnimatorByBPM���K�v�ł�
	*/
	class PlayerController final : public ComponentSystem
	{
	public:
		//!��Ԃ��Ǘ�����
		enum class State
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
	private:
		State state_ = State::Non;
		AnimatorByBPM* animator_ = nullptr;
		int indexX_ = 0;
		int indexY_ = 0;
	public:
		PlayerController()
		{

		}
		void initialize() override
		{
			animator_ = &entity->getComponent<AnimatorByBPM>();
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
		void tryAction()
		{
			switch (state_)
			{
			case State::Idle:
				if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1)
				{
					changeState(State::Left);
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1)
				{
					changeState(State::Right);
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_UP) == 1)
				{
					changeState(State::Up);
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
				{
					changeState(State::Down);
				}
				break;
			case State::Left:
				break;
			case State::Right:
				break;
			case State::Up:
				break;
			case State::Down:
				break;
			case State::Enter:
				break;
			case State::Clear:
				break;
			case State::Non:
			default:
				break;

			}

		}
		void actionByState()
		{
			switch (state_)
			{
			case State::Idle:
				break;
			case State::Left:
				break;
			case State::Right:
				break;
			case State::Up:
				break;
			case State::Down:
				break;
			case State::Enter:
				break;
			case State::Clear:
				break;
			case State::Non:
			default:
				break;

			}

		}

		void changeState(State state)
		{
			state_ = state;
		}
	};

}