/**
* @file StopComponent.hpp
* @brief 指定したコンポーネントの更新処理を一定時間停止させるコンポーネント
* @author feveleK5563
* @date 2018/10/18
*/

#pragma once
#include "../ECS/ECS.hpp"
#include <vector>

namespace ECS
{
	class StopEntity final : public ComponentSystem
	{
	private:
		int cnt_;
		std::vector<ComponentSystem*> stopCom;
	public:
		StopEntity(const int span) :cnt_(span) {}

		void update() override
		{
			--cnt_;
			if (cnt_ <= 0)
			{
				entity->destroy();
				
			}
		}
		void kill()
		{
			cnt_ = 0;
		}
		void setKillLimit(const int limit)
		{
			cnt_ = limit;
		}
	};
}