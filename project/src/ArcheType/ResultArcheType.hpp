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
		static Entity* CreateDishEntity(const char* graphicName, const Vec2 rectXY, const Vec2 rectWH, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->getComponent<Transform>().setScale(0.7f,0.7f);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>((int)rectXY.x, (int)rectXY.y, (int)rectWH.x, (int)rectWH.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot({ 256.f,256.f });
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
			entity->addComponent<Transform>().setPosition(rand.getRand(0.f, 1280.f), rand.getRand(-1000.f, -100.f));
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(srcXY.x, srcXY.y, srcWH.x, srcWH.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot({ 50,50 });
			entity->addComponent<Rotate>(1.f);
			entity->addComponent<FallDance>(rand.getRand(-5.f, 5.f), rand.getRand(0.f, 180.f), rand.getRand(0.01f, 0.1f));
			entity->addGroup(ENTITY_GROUP::EFFECT);
			return entity;
		}
	};
}