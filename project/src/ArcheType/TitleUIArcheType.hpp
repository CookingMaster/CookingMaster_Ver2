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
			
			//イージングが終了したら遷移フラグを立てる
			auto func = [](ECS::Entity* entity)
			{
				if (entity->getComponent<EasingPosMove>().getIsEaseEnd())
				{
					entity->getComponent<EasingPosMove>().setDest(
						Vec2(System::SCREEN_WIDIH / 2.f, -20.f),
						Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT + 20.f),
						90.f);
				}
				return;
			};

			entity->addComponent<AnyInputFunction>(func);
			entity->addGroup(ENTITY_GROUP::TITLE_UI);

			return entity;
		}

		static Entity* CreateLogoArchetype(const std::string& imgName, const Vec2& goalpos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			entity->addComponent<Position>();
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