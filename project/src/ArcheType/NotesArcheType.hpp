/**
* @file NotesArcheType.hpp
* @brief ƒm[ƒc‚ÌŒ´Œ^‚ðì‚é
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Class/NotesAndScoreData.hpp"

namespace ECS
{
	namespace NotesArcheType
	{
		//ƒm[ƒc‚ð¶¬
		Entity* CreateTestEntity(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName).setIndex(1);
			entity->getComponent<SpriteAnimationDraw>().setPivot(Vec2{ 32,32 });
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	}
}