#pragma once
#include "../GameController/GameController.h"
#include "../Components/TitleUIComponents.hpp"

namespace ECS
{
	struct TitleUIArcheType
	{
		static Entity* CreateMessageArchetype(const std::string& imgName, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<FlashImage>();

			entity->addComponent<Position>(pos);
			entity->addComponent<SimpleDraw>(imgName.c_str()).doCenter(true);
			entity->addGroup(ENTITY_GROUP::TITLE_PRESS);

			return entity;
		}

		static Entity* CreateLogoArchetype(const std::string& imgName, const Vec2& pos, EntityManager& entityManager_)
		{

		}
	};
}