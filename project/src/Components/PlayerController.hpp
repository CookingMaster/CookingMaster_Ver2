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
		Animator* animator_;
		RecordAnimation* recData_;
		BeatByTrigger* beatTrigger_;
		PlayerState* state_;

		Counter animCnt_;
		bool handup = false;

	public:
		PlayerController() :
			animCnt_(0, 10) {};

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
			recData_->addAnimData(0, 1, 3, 2, 3);	//左切り1
			recData_->addAnimData(0, 2, 3, 3, 3);	//左切り2
			recData_->addAnimData(0, 3, 3, 4, 3);	//右切り1
			recData_->addAnimData(0, 4, 3, 5, 3);	//右切り2

			recData_->setAnimData((int)PlayerState::State::Idle);	//待機アニメーションに設定
			animator_->setIsEndStopAnim(true);
		}

		void update() override
		{
			transitionState();
		}

	private:
		//入力や時間経過に対応してアニメーションと状態を遷移させる
		void transitionState()
		{
			//入力無しで待機状態
			if (animCnt_.isMax() && beatTrigger_->getTrigger())
			{
				handup = !handup;
				animator_->setIsMinusAnim(handup);
				state_->val = PlayerState::State::Idle;
				recData_->setAnimData((int)PlayerState::State::Idle);
				return;
			}

			//同時押しは許さない
			if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1 &&
				Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1)
			{
				return;
			}

			//左カーソルを押すと左切り状態
			if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1)
			{
				if (state_->val == PlayerState::State::Left1)
				{
					changeState(PlayerState::State::Left2);
				}
				else
				{
					changeState(PlayerState::State::Left1);
				}
				return;
			}
			//右カーソルを押すと右切り状態
			if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1)
			{
				if (state_->val == PlayerState::State::Right1)
				{
					changeState(PlayerState::State::Right2);
				}
				else
				{
					changeState(PlayerState::State::Right1);
				}
				return;
			}

			animCnt_.add();
		}

		//アニメーションをリセットし、指定番号の状態に変更する
		void changeState(PlayerState::State changeState)
		{
			state_->val = changeState;
			animator_->setIsMinusAnim(false);
			recData_->setAnimData((int)changeState);
			animCnt_.reset();
		}
	};
}