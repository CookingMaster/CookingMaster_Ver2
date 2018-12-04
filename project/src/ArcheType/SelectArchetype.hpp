/**
* @file SelectArcheType.hpp
* @brief リザルトで生成するスコアエンティティの原型を作成します
* @author tonarinohito
* @date 2018/12/04
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/CursorMove.hpp"
namespace ECS
{
	//!カーソルエンティティ
	static Entity* CreateCursor(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
	{
		auto* entity = &entityManager_.addEntity();
		entity->addComponent<Transform>().setPosition(pos.x, pos.y);
		entity->addComponent<Color>();
		entity->addComponent<AlphaBlend>();
		entity->addComponent<SpriteDraw>(graphicName);
		entity->getComponent<SpriteDraw>().setPivot(Vec2{ 32,32 });
		entity->getComponent<CursorMove>();
		entity->addGroup(ENTITY_GROUP::UI);
		return entity;
	}
}