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
	struct ResultArcheType final
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
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>((int)rectXY.x, (int)rectXY.y, (int)rectWH.x, (int)rectWH.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot({256.f,256.f});
			entity->getComponent<Transform>().setScale(0.7f,0.7f);
			entity->addGroup(ENTITY_GROUP::KITCHENWARE);	//よさげなの使いまわし
			return entity;
		}
		static Entity* CreateBackEntity(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteDraw>(graphicName).setPivot(Vec2{0.f,0.f});
			entity->addGroup(ENTITY_GROUP::BACK);
			return entity;
		}
		static Entity* CreateConfettiEntity(const char* graphicName, const Vec2_i srcXY, const Vec2_i srcWH, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			Random rand;
			entity->addComponent<Transform>().setPosition(rand.getRand(0.f, 1280.f), rand.getRand(-1100.f, -200.f));
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(srcXY.x, srcXY.y, srcWH.x, srcWH.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot({ 50,50 });
			entity->addComponent<Rotate>(1.f);
			entity->addComponent<FallDance>(rand.getRand(-5.f, 5.f), rand.getRand(0.f, 180.f), rand.getRand(0.01f, 0.1f));
			entity->addComponent<DestroyOutOfScreen>(Vec2{ 100.f,100.f });
			entity->addGroup(ENTITY_GROUP::EFFECT);
			return entity;
		}
		static Entity* CreateEvaluationEntity(const char* graphicName, const int index, const Vec2 pos, const Vec2_i imageSize, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->getComponent<Scale>().val = Vec2{ 0.f,0.f };
			Vec2 pivot(imageSize.x / 2.f, imageSize.y / 2.f);
			entity->addComponent<SpriteAnimationDraw>(graphicName).setPivot(pivot);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName).setIndex(index);
			entity->addComponent<ECS::Expand>(Vec2{ 1.f,1.f }, Easing::ExpoOut, 5.f);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
		static Entity* CreateScoreEntity(const char* graphicName, const Vec2 pos, const int score, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->getComponent<Scale>().val = Vec2{ 0.f,0.f };
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Expand>(Vec2{ 1.f,1.f }, Easing::ExpoIn, 20.f);
			entity->addComponent<Rectangle>(0, 0, 59, 75);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot(Vec2{ 59.f / 2,75.f / 2 });
			entity->addComponent<DrawFont2>(59.f, 75.f, score);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
		static Entity* CreateSpotLightEntity(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->getComponent<Scale>().val = Vec2{ 0.f,0.f };
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>(AlphaBlend::BlendMode::ADD, 255);
			entity->addComponent<Expand>(Vec2{ 1.f,1.f }, Easing::ExpoIn, 5.f);
			entity->addComponent<SpriteDraw>(graphicName).setPivot(Vec2{ 256.f,256.f });
			entity->addComponent<SpotLightMove>(2.5f, pos);
			//entity->addComponent<Lemniscate>(30.f);
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}

	};
}
