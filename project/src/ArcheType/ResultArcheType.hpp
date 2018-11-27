/**
* @file ResultArcheType.hpp
* @brief リザルトのアーキタイプを作ります
* @author yokota0717
* @date 2018.11.22
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/ResultComponent.hpp"
#include "../Utility/Random.hpp"

namespace ECS
{
	struct ResultArcheType
	{
		static Entity* CreateClocheEntity(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::COOKING_AREA);	//よさげなの使いまわし
			return entity;
		}
		static Entity* CreateDishEntity(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, (int)rect.x, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot({ 200,125 });
			entity->addGroup(ENTITY_GROUP::KITCHENWARE);	//よさげなの使いまわし
			return entity;
		}
		static Entity* CreateBackEntity(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SimpleDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::BACK);
			return entity;
		}
		static Entity* CreateConfettiEntity(const char* graphicName, const Vec2_i srcXY, const Vec2_i srcWH, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			Random rand;
			entity->addComponent<Transform>().setPosition(rand.getRand(0.f, 1280.f), rand.getRand(-800.f, -100.f));
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(srcXY.x, srcXY.y, srcWH.x, srcWH.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot({ 50,50 });
			entity->addComponent<Rotate>(1.f);
			entity->addComponent<FallDance>(rand.getRand(-5.f, 5.f), rand.getRand(0.f,180.f));
			entity->addGroup(ENTITY_GROUP::EFFECT);
			return entity;
		}
	};
}