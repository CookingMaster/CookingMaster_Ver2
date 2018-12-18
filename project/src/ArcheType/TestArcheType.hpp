/**
* @file Test.hpp
* @brief エンティティの原型を作るテスト
* @author tonarinohito
* @date 2018/10/05
* @par History
- 2018/12/13 tonarinohito
-# 矩形で指定できるEntityとアニメーションで指定できるEntity追加
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
		//!座標だけ持っているエンティティの生成
		static Entity* CreatePlainEntity(const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Position>(pos);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!画像を表示できるエンティティの生成
		static Entity* CreateEntity(const char* graphicName, const Vec2& pos, EntityManager& entityManager_ ,const Group group)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteDraw>(graphicName).setPivot(Vec2{ 0.f,0.f });
			entity->addGroup(group);
			return entity;
		}
		//!画像を表示できるエンティティの生成
		static Entity* CreateRectEntity(const char* graphicName, const Vec2& pos, const Rectangle& rectAngle, EntityManager& entityManager_, const Group group)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(rectAngle);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot(Vec2{ 0.f,0.f });
			entity->addGroup(group);
			return entity;
		}
		//!分割画像を表示できるエンティティの生成
		static Entity* CreateMultiEntity(const char* graphicName, const Vec2& pos, EntityManager& entityManager_, const Group group)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName).setPivot(Vec2{0.f,0.f});
			entity->addGroup(group);
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
		//static Entity* CreateFrameAnimationEntity(const char* graphicName, const Vec2 pos, const int frame, EntityManager& entityManager_)
		//{
		//	auto* entity = &entityManager_.addEntity();
		//	entity->addComponent<Transform>().setPosition(pos.x, pos.y);
		//	entity->addComponent<Color>();
		//	entity->addComponent<AlphaBlend>();
		//	entity->addComponent<SpriteAnimationDraw>(graphicName).setIndex(1);
		//	entity->getComponent<SpriteAnimationDraw>().setPivot(Vec2{ 32,32 });
		//	entity->addComponent<AnimatorByFrame>(frame).setSpriteNum(1, 0, 5, 1);
		//	entity->addGroup(ENTITY_GROUP::LAYER1);
		//	return entity;
		//}

		//!プレイヤエンティティテスト
		static Entity* CreatePlayerEntity(const char* graphicName, const char* soundName, const Vec2 pos, const int frame, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName);
			entity->getComponent<SpriteAnimationDraw>().setPivot(Vec2{ 32,32 });
			entity->addComponent<PlayerState>(PlayerState::State::Idle);
			entity->addComponent<AnimatorPlayer>(soundName, 130, frame).setSpriteNum(6, 4);
			entity->addComponent<PlayerController>();
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}

		//!ゲームBGエンティティテキスト
		static Entity* CreateGameBG(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SimpleDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	};
}