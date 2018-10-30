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
#include "../Components/PlayerController.hpp"
#include "../Components/UIComponents.hpp"
#include "../Input/Input.hpp"
namespace ECS
{
	void BarEvent(Entity* entity)
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_Q) == 1)
		{
			//バーUIのゲージを進める
			entity->getComponent<BarComponentSystemX>().addScore(10);
		}
		
	}
	int BarEvent2(Entity* entity)
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_W) == 1)
		{
			//バーUIのゲージを進める
			entity->getComponent<BarComponentSystemX>().addScore(40);
		}
		return 0;
	}
	void BarEvent3(Entity* entity)
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_E) == 1)
		{
			//バーUIのゲージを進める
			entity->getComponent<BarComponentSystemX>().addScore(80);
		}
	}
	struct BarEvent4
	{
		void operator()(Entity* e)
		{
			if (Input::Get().getKeyFrame(KEY_INPUT_E) == 1)
			{
				//バーUIのゲージを進める
				e->getComponent<BarComponentSystemX>().addScore(100);
			}
		}
	};

	struct ArcheType
	{
		//!エンティティの生成テスト
		static Entity* CreateAA(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 0, 38);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<EventFunctionSystem<void>>(BarEvent);
			entity->addComponent<EventFunctionSystem<int>>(BarEvent2);
			entity->addComponent<EventFunctionSystem<void, class AAA>>(BarEvent4());
			entity->addComponent<BarComponentSystemX>(424, 0, 200);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
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
	};
}