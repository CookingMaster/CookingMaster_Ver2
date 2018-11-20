/**
* @file UIArcheType.hpp
* @brief UIのアーキタイプを作ります
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
		//!バー(スコア)
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
		//!バー（スコア）
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
		//!時計
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
		//!時計の針
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
		//!フォント
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

		//---------- ポーズ画面
		//ポーズ画面の背景
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
		//ポーズ画面のパンネル
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
		//ポーズ画面のボタン
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
		//ポーズ画面のボタンの上にのせる文字
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