/**
* @file PlayerController.hpp
* @brief プレイヤの女の子を制御するコンポーネント
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
	* @brief プレイヤの女の子を制御するコンポーネント
	* - AnimatorByBPMが必要です
	*/
	class PlayerController final : public ComponentSystem
	{
	public:
		//!状態を管理する
		enum class State
		{
			Non,	//!未定義
			Idle,	//!立ち
			Left,	//!左切り
			Right,	//!右切り
			Up,		//!上切り
			Down,	//!下切り
			Miss,	//!ミス(あれば)
			Enter,	//!入場
			Clear,	//!曲終了
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
		//!描画する画像のインデックスを設定する
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