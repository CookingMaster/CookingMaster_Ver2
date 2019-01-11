/**
* @file BossAnimator.hpp
* @brief おやっさんをリズムに合わせてアニメーションさせるコンポーネント
* @author feveleK5563
* @date 2019.1.10
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "Animator.hpp"
#include "BeatByTrigger.hpp"

namespace ECS
{
	class BossAnimator final : public ComponentSystem
	{
	private:
		Animator* animator_ = nullptr;
		BeatByTrigger* beatTrigger_ = nullptr;
		bool shoulderUp = false;

	public:
		void initialize() override
		{
			if (!entity->hasComponent<Animator>())
			{
				entity->addComponent<Animator>();
			}
			animator_ = &entity->getComponent<Animator>();
			animator_->setSpriteNum(0, 0, 3, 1, true);
			animator_->setIsEndStopAnim(true);
			animator_->changeFrame(2);

			beatTrigger_ = &entity->getComponent<BeatByTrigger>();
		}

		void update() override
		{
			//肩上げの動きを行っている時
			if (shoulderUp)
			{
				//肩が完全に上がったら下げる
				if (animator_->isAnimEnd())
				{
					animator_->setIsMinusAnim(false);
					shoulderUp = false;
				}
			}
			//肩下げの動きを行っている時
			else
			{
				//タイミング良く肩を上げる
				if (beatTrigger_->getTrigger())
				{
					animator_->setIsMinusAnim(true);
					shoulderUp = true;
				}
			}
		}
	};
}