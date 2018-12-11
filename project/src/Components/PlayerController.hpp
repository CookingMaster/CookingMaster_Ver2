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
		PlayerState() : val(State::Non) {}
		explicit PlayerState(State state) 
			:
			val(state)
		{}
	};


	/**
	* @brief プレイヤ専用アニメーター
	* - SpriteAnimationDraw、PlayerStateが必要
	*/
	class AnimatorPlayer final : public ComponentSystem
	{
	private:
		PlayerState* state_ = nullptr;
		SpriteAnimationDraw* animation_ = nullptr;
		Counter counter_;
		int frame_ = 0;					//!アニメーションするフレーム数
		int bpm_ = 0;					//!現在のBPM
		DWORD beat_ = 0;				//!一拍の長さ(ms)
		DWORD start_ = 0;
		std::string soundname_ = "";	//!現在のBGMの登録名
		int indexX_BPM_ = 0;			//!BPMに合わせている時のX方向のインデックス
		int indexX_Frame_ = 0;			//!フレームに合わせている時のX方向のインデックス
		int indexY_ = 0;				//!Y方向のインデックス
		int maxXnum_BPM_ = 0;			//!BPMに合わせている時の描画する画像のX方向の枚数
		int maxXnum_Frame_ = 0;			//!フレームに合わせている時の描画する画像のX方向の枚数
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
		//!BPMに合わせた更新処理をする
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
		//!フレームに合わせた更新処理をする
		void updateByFrame()
		{
			//!X方向のインデックス更新
			if (++counter_ >= frame_)
			{
				++indexX_Frame_;
				if (indexX_Frame_ >= maxXnum_Frame_)
				{
					indexX_Frame_ = 0;
				}
			}
			//!Y方向のインデックス設定
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
			//!描画画像指定
			animation_->setIndex(indexX_Frame_ + (indexY_ * maxXnum_BPM_));
		}
		//!フレームに合わせたアニメーション関連の初期化
		void initFrameAnimator()
		{
			indexX_Frame_ = 0;
			counter_.reset();
			indexY_ = 0;
		}
		//!BPMを設定する
		void setBPM(const int bpm)
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
		/**
		* @brief 画像枚数の設定
		* @param xmaxBPM BPMに合わせたアニメーションをするときの画像枚数
		* @param xmaxFrame フレームに合わせたアニメーションをするときの画像枚数
		*/
		void setSpriteNum(const int xmaxBPM, const int xmaxFrame)
		{
			maxXnum_BPM_ = xmaxBPM;
			maxXnum_Frame_ = xmaxFrame;
		}
	};


	/**
	* @brief プレイヤの女の子を制御するコンポーネント
	* - AnimatorPlayerが必要
	*/
	class PlayerController final : public ComponentSystem
	{
	private:
		AnimatorPlayer* animator_ = nullptr;
		PlayerState* state_ = nullptr;
		Counter counter_;
		int cutAnimMaxTime_ = 180;	//!切るモーションの全体の長さ(フレーム数)、これを超えるとIdleに戻る
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
		* @brief状態を変更する
		* @param state 変更する状態
		*/
		void changeState(PlayerState::State state)
		{
			state_->val = state;
		}
	};

}