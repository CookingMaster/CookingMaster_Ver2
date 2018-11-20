#pragma once
#include "../GameController/GameController.h"
#include "../Components/TitleUIComponents.hpp"

namespace ECS
{
	struct TitleUIArcheType
	{
		static Entity* CreateLogoArchetype(const std::string& imgName, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntityAddTag("unko");
			entity->addComponent<FlashImage>();

			entity->addComponent<Position>(pos);
			entity->addComponent<SimpleDraw>(imgName.c_str()).doCenter(true);
			entity->addGroup(ENTITY_GROUP::TITLE_PRESS);

			return entity;
		}
	};
}