/**
* @file AnimatorPlayer.hpp
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
#include "BeatByTrigger.hpp"
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
			Idle	= 0,	//!立ち
			Left1	= 1,	//!左切り1
			Left2	= 2,	//!左切り2
			Right1	= 3,	//!右切り1
			Right2	= 4,	//!右切り2
			Enter	= 5,	//!入場
			Clear	= 6,	//!曲終了
		} val;
		PlayerState() : val(State::Idle) {}
		explicit PlayerState(State state) 
			:
			val(state)
		{}
	};

	/**
	* @brief プレイヤ専用アニメーター
	* - Animator、RecordAnimation、BeatByTrigger、PlayerStateが必要
	*/
	class PlayerController final : public ComponentSystem
	{
	private:
		Animator* animator_ = nullptr;
		RecordAnimation* recData_ = nullptr;
		BeatByTrigger* beatTrigger_ = nullptr;
		PlayerState* state_ = nullptr;

		Counter animCnt_;	//切りモーションの終了までの時間を計測したりする
		bool handup_ = false;

		const bool isAutoMode_;

	public:
		PlayerController(const bool isAuto) :
			animCnt_(0, 15),
			isAutoMode_(isAuto) {};

		void initialize() override
		{
			animator_ = &entity->getComponent<Animator>();
			recData_ = &entity->getComponent<RecordAnimation>();
			beatTrigger_ = &entity->getComponent<BeatByTrigger>();
			if (!entity->hasComponent<PlayerState>())
			{
				entity->addComponent<PlayerState>();
			}
			state_ = &entity->getComponent<PlayerState>();

			recData_->addAnimData(0, 0, 3, 1, 2);	//待機
			recData_->addAnimData(0, 1, 3, 2, 4);	//左切り1
			recData_->addAnimData(0, 2, 3, 3, 4);	//左切り2
			recData_->addAnimData(0, 3, 3, 4, 4);	//右切り1
			recData_->addAnimData(0, 4, 3, 5, 4);	//右切り2

			recData_->setAnimData((int)PlayerState::State::Idle, true);	//待機アニメーションに設定
			animator_->setIsEndStopAnim(true);
		}

		void update() override
		{
			transitionState();
		}

		//切りアニメーション実行
		void playSlashAnim(ECS::Direction::Dir dir)
		{
			switch (dir)
			{
			//右切り
			case ECS::Direction::Dir::R:
				if (state_->val == PlayerState::State::Right1)
				{
					changeState(PlayerState::State::Right2);
				}
				else
				{
					changeState(PlayerState::State::Right1);
				}
				break;

			//左切り
			case ECS::Direction::Dir::L:
				if (state_->val == PlayerState::State::Left1)
				{
					changeState(PlayerState::State::Left2);
				}
				else
				{
					changeState(PlayerState::State::Left1);
				}
				break;
			}
		}

	private:
		//入力や時間経過に対応してアニメーションと状態を遷移させる
		void transitionState()
		{
			//待機動作が次のリズムのタイミングになったらパタパタ
			if (state_->val == PlayerState::State::Idle && beatTrigger_->getTrigger())
			{
				handup_ = !handup_;
				animator_->setIsMinusAnim(handup_);
				recData_->setAnimData((int)PlayerState::State::Idle, false);
			}
			//切りモーションが終了したら待機
			if (state_->val != PlayerState::State::Idle && animCnt_.isMax())
			{
				handup_ = !handup_;
				animator_->setIsMinusAnim(handup_);
				state_->val = PlayerState::State::Idle;
				recData_->setAnimData((int)PlayerState::State::Idle, false);
				animator_->setIndexMax();
			}

			//同時押しは許さない
			if (beatTrigger_->isEndSound() ||
				(Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1 &&
				 Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1))
			{
				return;
			}

			//オートモードだったら入力させない
			if (isAutoMode_)
			{
				animCnt_.add();
				return;
			}

			//左カーソルを押すと左切り状態
			if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1)
			{
				playSlashAnim(ECS::Direction::Dir::L);
			}
			//右カーソルを押すと右切り状態
			if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1)
			{
				playSlashAnim(ECS::Direction::Dir::R);
			}

			animCnt_.add();
		}

		//アニメーションをリセットし、指定番号の状態に変更する
		void changeState(PlayerState::State changeState)
		{
			state_->val = changeState;
			animator_->setIsMinusAnim(false);
			recData_->setAnimData((int)changeState, true);
			animCnt_.reset();
		}
	};
}