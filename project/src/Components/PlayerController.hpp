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
			Non,	//!未定義
			Idle,	//!立ち
			Left,	//!左切り
			Right,	//!右切り
			Enter,	//!入場
			Clear,	//!曲終了
		} val;
		PlayerState() : val(State::Non) {}
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

		bool handup = false;

	public:
		PlayerController() = default;

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
			recData_->addAnimData(0, 1, 3, 2, 2);	//左切り1
			recData_->addAnimData(0, 2, 3, 3, 2);	//左切り2
			recData_->addAnimData(0, 3, 3, 4, 2);	//右切り1
			recData_->addAnimData(0, 4, 3, 5, 2);	//右切り2

			recData_->setAnimData(0);	//待機アニメーションに設定
			animator_->setIsEndStopAnim(true);
		}

		void update() override
		{
			if (beatTrigger_->getTrigger())
			{
				handup = !handup;
				animator_->setIsMinusAnim(handup);
			}
		}
	};
}