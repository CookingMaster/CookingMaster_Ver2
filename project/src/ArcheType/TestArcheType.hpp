/**
* @file Test.hpp
* @brief エンティティの原型を作るテスト
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
namespace ECS
{
	namespace ArcheType
	{
		//!エンティティの生成テスト
		Entity* CreateTestEntity(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
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
	}
}