#pragma once
#include "../GameController/GameController.h"
#include "../Components/TitleUIComponents.hpp"
#include "../System/System.hpp"

namespace ECS
{
	struct TitleUIArcheType
	{
		static Entity* CreateMessageArchetype(const std::string& imgName, const Vec2& goalpos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<FlashImage>();

			entity->addComponent<Position>();
			entity->addComponent<SimpleDraw>(imgName.c_str()).doCenter(true);
			entity->addComponent<EasingPosMove>().setDest(
				Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT + 20.f),
				goalpos,
				90.f);
			entity->addGroup(ENTITY_GROUP::TITLE_UI);

			return entity;
		}

		static Entity* CreateLogoArchetype(const std::string& imgName, const Vec2& goalpos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			entity->addComponent<SimpleDraw>(imgName.c_str()).doCenter(true);
			entity->addComponent<EasingPosMove>().setDest(
				Vec2(System::SCREEN_WIDIH / 2.f, -50.f),
				goalpos,
				60.f);
			entity->addGroup(ENTITY_GROUP::TITLE_UI);

			return entity;
		}
	};
}