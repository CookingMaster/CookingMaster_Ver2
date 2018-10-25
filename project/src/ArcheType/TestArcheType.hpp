/**
* @file Test.hpp
* @brief �G���e�B�e�B�̌��^�����e�X�g
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/Animator.hpp"
#include "../Components/PlayerController.hpp"
#include "../Components/UIComponents.hpp"
namespace ECS
{
	struct ArcheType
	{
		//!�G���e�B�e�B�̐����e�X�g
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

		//!BPM�A�j���[�V�����e�X�g
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

		//!�t���[���A�j���[�V�����e�X�g
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

		//!�v���C���G���e�B�e�B�e�X�g
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


		//!�o�[
		static Entity* CreateEmptyBarUI(const char* graphicName,const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, rect.x, rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!�o�[
		static Entity* CreateFullBarUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 0, rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<BarComponentSystemX>(rect.x, 0, 200);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!���v
		static Entity* CreateClockUI(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addGroup(ENTITY_GROUP::LAYER1);
			entity->addComponent<SimpleDraw>(graphicName);
			return entity;
		}
		//!���v�̐j
		static Entity* CreateNeedleUI(const char* graphicName, const Vec2 pos, EntityManager& entityManager_, int speed)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addGroup(ENTITY_GROUP::LAYER1);
			entity->addComponent<TimerNeedleMove>(speed);
			entity->addComponent<SpriteDraw>(graphicName);
			return entity;
		}
	};
}