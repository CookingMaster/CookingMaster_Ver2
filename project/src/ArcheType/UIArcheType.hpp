/**
* @file UIArcheType.hpp
* @brief UI�̃A�[�L�^�C�v�����܂�
* @author moon
* @date 2018/10/25
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/UIComponents.hpp"

namespace ECS
{
	struct UIArcheType
	{
		//!�o�[(�X�R�A)
		static Entity* CreateEmptyBarUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, (int)rect.x, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
		//!�o�[�i�X�R�A�j
		static Entity* CreateFullBarUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 0, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<BarComponentSystemX>((int)rect.x, 0, 200);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
		//!���v
		static Entity* CreateClockUI(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addGroup(ENTITY_GROUP::UI);
			entity->addComponent<SimpleDraw>(graphicName);
			return entity;
		}
		//!���v�̐j
		static Entity* CreateNeedleUI(const char* graphicName, const Vec2 pos, EntityManager& entityManager_, float speed)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addGroup(ENTITY_GROUP::UI);
			entity->addComponent<TimerNeedleMove>(speed);
			entity->addComponent<SpriteDraw>(graphicName).setPivot(Vec2{ 7.f,48.f });
			return entity;
		}
		//!�t�H���g
		static Entity* CreateFontUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 0, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::UI);
			entity->addComponent<DrawFont>((int)rect.x, 0);
			entity->addComponent<ExpandReduceComponentSystem>(1.3f, 0.1f);
			return entity;
		}

		//---------- �|�[�Y���
		//�|�[�Y��ʂ̔w�i
		static Entity* CreatePauseBG(const char* graphicName, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SimpleDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}
		//�|�[�Y��ʂ̃p���l��
		static Entity* CreatePauseUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			//entity->addComponent<Transform>().setScale(rect.x, rect.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, rect.x, rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot(Vec2{ 261.f,128.f });
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			entity->addComponent<ExpandComponentSystem>(0.f, 1.f, 0.05f);
			return entity;
		}
		//�|�[�Y��ʂ̃{�^��
		static Entity* CreateButtonUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Transform>().setScale(rect.x, rect.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, rect.x, rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot(Vec2{rect.x, rect.y});
			entity->addComponent<ButtonCommponent>(0, 138);
			entity->addComponent<ExpandComponentSystem>(0.f, 1.f, 0.05f);
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}
		//�|�[�Y��ʂ̃{�^���̏�ɂ̂��镶��
		static Entity* CreateButtonMojiUI(const char* graphicName, const Vec2 rect, const Vec2 pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Transform>().setScale(rect.x, rect.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, rect.x, rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot(Vec2{rect.x, rect.y});
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			entity->addComponent<ButtonMojiCommponent>(rect.x);
			entity->addComponent<ExpandComponentSystem>(0.f, 1.f, 0.05f);
			return entity;
		}
	};
}