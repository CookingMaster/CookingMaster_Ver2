/**
* @file Test.hpp
* @brief エンティティの原型を作るテスト
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/Animator.hpp"
namespace ECS
{
	struct ArcheType
	{
		//!エンティティの生成テスト
		static Entity* CreateTestEntity(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x,pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName).setIndex(1);
			entity->getComponent<SpriteAnimationDraw>().setPivot(Vec2{ 32,32 });
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}

		//!BPMアニメーションテスト
		static Entity* CreateAnimationEntity(const char* graphicName, const char* soundName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName).setIndex(1);
			entity->getComponent<SpriteAnimationDraw>().setPivot(Vec2{ 32,32 });
			entity->addComponent<AnimatorByBPM>(soundName, 130).setSpriteNum(1, 0, 5, 3);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}

		//!フレームアニメーションテスト
		static Entity* CreateFrameAnimationEntity(const char* graphicName, const Vec2 pos, const int frame, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName).setIndex(1);
			entity->getComponent<SpriteAnimationDraw>().setPivot(Vec2{ 32,32 });
			entity->addComponent<AnimatorByFrame>(frame).setSpriteNum(1, 0, 5, 1);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	};
}