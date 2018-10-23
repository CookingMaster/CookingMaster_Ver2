/**
* @file ReplayPhysics.hpp
* @brief 停止しているPhysicsとKillEntityを再開させるコンポーネント
* @author feveleK5563
* @date 2018/10/18
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BasicComponents.hpp"

namespace ECS
{
	/*
	@brief 停止しているPhysicsとKillEntityを、指定フレーム後に再開させる
	@details PhysicsとKillEntityを予め用意しておく必要がある
	*/
	class ReplayPhysics final : public ComponentSystem
	{
	private:
		int cnt_;
	public:
		ReplayPhysics(const int span) :cnt_(span) {}

		void initialize() override
		{
			entity->stopComponent<Physics>();
			entity->stopComponent<KillEntity>();
		}
		void update() override
		{
			--cnt_;
			if (cnt_ <= 0)
			{
				entity->updateComponent<Physics>();
				entity->updateComponent<KillEntity>();
				entity->stopComponent<ReplayPhysics>();
			}
		}
	};
}