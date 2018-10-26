/**
* @file UIArcheType.hpp
* @brief UIのアーキタイプを作ります
* @author moon
* @date 2018/10/25
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/UIComponents.hpp"

namespace ECS
{
	struct UIArcheType
	{
		//!バー
		static Entity* CreateEmptyBarUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, (int)rect.x, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!バー
		static Entity* CreateFullBarUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 0, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<BarComponentSystemX>((int)rect.x, 0, 200);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!時計
		static Entity* CreateClockUI(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addGroup(ENTITY_GROUP::LAYER1);
			entity->addComponent<SimpleDraw>(graphicName);
			return entity;
		}
		//!時計の針
		static Entity* CreateNeedleUI(const char* graphicName, const Vec2 pos, EntityManager& entityManager_, float speed)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addGroup(ENTITY_GROUP::LAYER1);
			entity->addComponent<TimerNeedleMove>(speed);
			entity->addComponent<SpriteDraw>(graphicName).setPivot(Vec2{ 7.f,48.f });
			return entity;
		}
	};
}