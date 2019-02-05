#pragma once
#include "../GameController/GameController.h"
#include "../Components/TitleUIComponents.hpp"
#include "../Components/UIComponents.hpp"
#include "../System/System.hpp"

namespace ECS
{
	struct TitleUIArcheType final
	{
		static Entity* CreateMessageArchetype(const std::string& imgName, const Vec2& goalpos, EntityManager& entityManager)
		{
			auto* entity = &entityManager.addEntity();
			entity->addComponent<FlashImage>();

			entity->addComponent<Transform>().setPosition(-1000.f, -1000.f);
			entity->addComponent<SpriteDraw>(imgName.c_str());
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

					auto& bg = entityManager.getEntitiesByGroup(ENTITY_GROUP::TITLE_BACK);
					for (size_t i = 0; i < bg.size(); ++i)
					{
						bg[i]->getComponent<ZoomIn>().setStop(false);
						if (bg[i]->hasComponent<DoorMover>())
						{
							bg[i]->updateComponent<DoorMover>();
						}
					}
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

			entity->addComponent<Transform>().setPosition(-1000.f, -1000.f);
			entity->addComponent<FlashImage>();
			entity->stopComponent<FlashImage>();
			entity->addComponent<SpriteDraw>(imgName.c_str());
			entity->addComponent<EasingPosMove>().setDest(
				Vec2(System::SCREEN_WIDIH / 2.f, -50.f),
				goalpos,
				60.f);

			entity->addGroup(ENTITY_GROUP::TITLE_LOGO);

			return entity;
		}

		static Entity* CreateTitleBGArchetype(const std::string& imgName, const Vec2& pos, EntityManager& entityManager_, bool isDoor = false)
		{
			auto* entity = &entityManager_.addEntity();

			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<SpriteDraw>(imgName.c_str());
			entity->addComponent<ZoomIn>(0.008f, Vec2(556.f + 66.5f, 400.f + 116.f)).setStop(true);

			if (isDoor)
			{
				entity->addComponent<DoorMover>(Vec2(3.f, 0.f));
				entity->stopComponent<DoorMover>();
			}

			entity->addGroup(ENTITY_GROUP::TITLE_BACK);

			return entity;
		}

		static Entity* CreateFade(const std::string& imgName, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(0.f ,0.f);
			entity->addComponent<FadeComponent>().reset(255.f, 0.f, 60.f);
			entity->addComponent<SpriteDraw>(imgName.c_str()).setPivot(Vec2{ 0.f,0.f });

			entity->addGroup(ENTITY_GROUP::TOP_FADE);

			return entity;
		}
	};
}
