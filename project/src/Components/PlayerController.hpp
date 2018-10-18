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
		SpriteAnimationDraw* animation_ = nullptr;
		PlayerState* state_;
		float bpm_ = 0;
		DWORD beat_ = 0;
		DWORD start_ = 0;
		std::string soundname_ = "";
		Counter counter_;
		int frame_ = 0;		//!アニメーションするフレーム数
		int indexX_ = 0;
		int indexY_ = 0;
		int xmin_ = 0;		//!描画する画像のX方向のインデックスの最小
		int ymin_ = 0;		//!描画する画像のY方向のインデックスの最小
		int maxXnum_ = 0;	//!描画する画像のX方向の枚数
		int maxYnum_ = 0;	//!描画する画像のY方向の枚数

	public:
		AnimatorPlayer(const char* soundname, const float bpm, const int frame) 
			:
			soundname_(soundname),
			bpm_(bpm),
			frame_(frame)
		{
			calcBeat();
		}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();
			state_ = &entity->getComponent<PlayerState>();
		}
		void update() override
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
		//!BPMを設定する
		void setBPM(const float bpm)
		{
			bpm_ = bpm;
		}
		/**
		* @brief 1拍の長さ(ms)を計算する
		* @note 1000(ms) * 60(sec/min) / bpm(beat/min)
		*/
		void calcBeat()
		{
			beat_ = static_cast<DWORD>(1000 * (60.f / bpm_));
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