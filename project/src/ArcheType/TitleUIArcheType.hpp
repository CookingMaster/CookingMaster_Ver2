#pragma once
#include "../GameController/GameController.h"
#include "../Components/TitleUIComponents.hpp"
#include "../Components/UIComponents.hpp"
#include "../System/System.hpp"

namespace ECS
{
	struct TitleUIArcheType
	{
		static Entity* CreateMessageArchetype(const std::string& imgName, const Vec2& goalpos, EntityManager& entityManager)
		{
			auto* entity = &entityManager.addEntity();
			entity->addComponent<FlashImage>();

			entity->addComponent<Position>(-1000.f, -1000.f);
			entity->addComponent<SimpleDraw>(imgName.c_str()).isCenter(true);
			entity->addComponent<EasingPosMove>().setDest(
				Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT + 100.f),
				goalpos,
				90.f);

			//イージングが終了したらロゴと一緒に消える
			auto func = [](ECS::Entity* entity, EntityManager& entityManager, bool& isPushed)
			{
				if (!isPushed && entity->getComponent<EasingPosMove>().getIsEaseEnd())
				{
					entity->getComponent<FlashImage>().setIsDelete(true);

					auto& logo = entityManager.getEntitiesByGroup(ENTITY_GROUP::TITLE_LOGO);
					logo[0]->updateComponent<FlashImage>();
					logo[0]->getComponent<FlashImage>().setIsDelete(true);

					auto& bg = entityManager.getEntitiesByGroup(ENTITY_GROUP::TITLE_BG);
					bg[0]->updateComponent<ZoomIn>();
					isPushed = true;
				}
				return;
			};

			entity->addComponent<AnyInputFunction>(func, entityManager);
			entity->addGroup(ENTITY_GROUP::TITLE_MESSAGE);

			return entity;
		}

		static Entity* CreateLogoArchetype(const std::string& imgName, const Vec2& goalpos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			entity->addComponent<Position>(-1000.f, -1000.f);
			entity->addComponent<FlashImage>();
			entity->stopComponent<FlashImage>();
			entity->addComponent<SimpleDraw>(imgName.c_str()).isCenter(true);
			entity->addComponent<EasingPosMove>().setDest(
				Vec2(System::SCREEN_WIDIH / 2.f, -50.f),
				goalpos,
				60.f);

			entity->addGroup(ENTITY_GROUP::TITLE_LOGO);

			return entity;
		}

		static Entity* CreateTitleBGArchetype(const std::string& imgName, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			entity->addComponent<Transform>();
			entity->addComponent<SpriteDraw>(imgName.c_str());
			entity->addComponent<ZoomIn>(0.008f, Vec2(575.f, 660.f));
			entity->stopComponent<ZoomIn>();

			entity->addGroup(ENTITY_GROUP::TITLE_BG);

			return entity;
		}

		static Entity* CreateFade(const std::string& imgName, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Position>(0.f ,0.f);
			entity->addComponent<FadeComponent>().reset(255.f, 0.f, 60.f);
			entity->addComponent<SimpleDraw>(imgName.c_str());

			entity->addGroup(ENTITY_GROUP::TOP_FADE);

			return entity;
		}
	};
}