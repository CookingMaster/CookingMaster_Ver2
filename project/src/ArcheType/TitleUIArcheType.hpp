#pragma once
#include "../GameController/GameController.h"
#include "../Components/TitleUIComponents.hpp"

namespace ECS
{
	struct TitleUIArcheType
	{
		static Entity* CreateLogoArchetype(EntityManager& entityManager_)
		{
			auto& entity = entityManager_.addEntity();
			entity.addComponent<FlashImage>();

			entity.addComponent<Position>(0.f, 0.f);
			entity.addComponent<RectDraw>();
		}
	};
}