/**
* @file ArcheType.hpp
* @brief エンティティの原型を作る
* @author tonarinohito
* @date 2018/10/05
* @par History
- 2018/12/13 tonarinohito
-# 矩形で指定できるEntityとアニメーションで指定できるEntity追加
- 2018/12/20 tonarinohito
-# TestArcheTypeをArcheTypeにリネーム
*/
#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/Animator.hpp"
#include "../Components/PlayerController.hpp"
#include "../Components/UIComponents.hpp"
#include "../Input/Input.hpp"
namespace ECS
{
	struct ArcheType
	{
		//!Transformだけ持っているエンティティの生成
		static Entity* CreatePlainEntity(const Vec2& pos, EntityManager& entityManager)
		{
			auto* entity = &entityManager.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x,pos.y);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!画像を表示できるエンティティの生成
		static Entity* CreateEntity(const char* graphicName, const Vec2& pos, EntityManager& entityManager ,const Group group)
		{
			auto* entity = &entityManager.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteDraw>(graphicName).setPivot(Vec2{ 0.f,0.f });
			entity->addGroup(group);
			return entity;
		}
		//!画像を表示できるエンティティの生成
		static Entity* CreateRectEntity(const char* graphicName, const Vec2& pos, const Rectangle& rectangle, EntityManager& entityManager, const Group group)
		{
			auto* entity = &entityManager.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(rectangle);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot(Vec2{ 0.f,0.f });
			entity->addGroup(group);
			return entity;
		}
		//!分割画像を表示できるエンティティの生成
		static Entity* CreateAnimationEntity(const char* graphicName, const Vec2& pos, EntityManager& entityManager, const Group group)
		{
			auto* entity = &entityManager.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName).setPivot(Vec2{0.f,0.f});
			entity->addGroup(group);
			return entity;
		}
	};
}