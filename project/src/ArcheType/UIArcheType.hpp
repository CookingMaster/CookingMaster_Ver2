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
		//!����ۂ̃o�[�i�X�R�A�\���p�j
		static Entity* CreateEmptyBarUI(const char* graphicName, const Vec2& rect, const Vec2& pos, EntityManager& entityManager_)
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
		//!�o�[�̒��g�i�X�R�A�\���p�j
		static Entity* CreateFullBarUI(const char* graphicName, const Vec2& rect, const Vec2& pos, int maxScore, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 0, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<BarComponentSystemX>((int)rect.x, maxScore);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
		//!���v
		static Entity* CreateClockUI(const char* graphicName, const Vec2& pos, EntityManager& entityManager_)
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
		static Entity* CreateNeedleUI(const char* graphicName, const Vec2& pos, EntityManager& entityManager_, float speed)
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
		//!�X�R�A�\���p�t�H���g
		static Entity* CreateFontUI(const char* graphicName, const Vec2& rect, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 0, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<DrawFont>(rect.x, rect.y);
			entity->addComponent<ExpandReduceComponentSystem>(1.5f, 0.1f);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}

		//---------- �|�[�Y���
		//�|�[�Y��ʂ̔w�i
		static Entity* CreatePauseBG(const char* graphicName, const Vec2& pos, EntityManager& entityManager_)
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
		static Entity* CreatePauseUI(const char* graphicName, const Vec2& rect, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, static_cast<int>(rect.x), static_cast<int>(rect.y));
			entity->addComponent<SpriteRectDraw>(graphicName).setPivot(Vec2{ 261.f,128.f });
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}
		//�|�[�Y��ʂ̃{�^��
		static Entity* CreateButtonUI(const char* graphicName, const Vec2& rect, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, static_cast<int>(rect.x), static_cast<int>(rect.y));
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<ButtonCommponent>(138);
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}
		//�|�[�Y��ʂ̃{�^���̏�ɂ̂��镶��
		static Entity* CreateButtonMojiUI(const char* graphicName, const Vec2& rect, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, static_cast<int>(rect.x), static_cast<int>(rect.y));
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<ButtonMojiCommponent>(static_cast<int>(rect.x));
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}
		//�|�[�Y��ʂ̑I�����t���[��
		static Entity* CreateSelectFrame(const char* graphicName, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 145, 64);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<SelectFrame>();
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}
	};
}