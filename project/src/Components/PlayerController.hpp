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
	* @brief プレイヤの状態を保持する
	*/
	struct PlayerState final : public ComponentData
	{
		enum class State : short
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
		State val;
		explicit PlayerState() : val(State::Non) {}
	};
	/**
	* @brief プレイヤ専用アニメーター
	* - SpriteAnimationDraw、PlayerStateが必要
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
	* @brief プレイヤの女の子を制御するコンポーネント
	* - PlayerStateが必要
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
		//!描画する画像のインデックスを設定する
		void setGraphIndex()
		{

		}
		/**
		* @brief 現在の状態に合わせて入力を受け付ける
		* @note 要するにゲープロのThink()
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
		* @brief アニメーション用のカウンタを設定する
		*/
		void initAnimCounter(const int add, const int max)
		{
			animCounter_.SetCounter(0, add, 0, max);
		}
	};

}