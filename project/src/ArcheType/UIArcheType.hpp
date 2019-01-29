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
#include "../Components/TitleUIComponents.hpp"

namespace ECS
{
	struct UIArcheType
	{
		//!空っぽのバー（スコア表示用）
		static Entity* CreateEmptyBarUI(const char* graphicName, const Vec2& rect, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, (int)rect.x, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addGroup(ENTITY_GROUP::KITCHENWARE);
			return entity;
		}
		//!バーの中身（スコア表示用）
		static Entity* CreateFullBarUI(const char* graphicName, const Vec2& rect, const Vec2& pos, int maxScore, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, (int)rect.y, (int)rect.x, (int)rect.y);
			entity->addComponent<SpriteRectDraw>(graphicName);
			entity->addComponent<BarComponentSystemY>((int)rect.y, maxScore);
			entity->addGroup(ENTITY_GROUP::KITCHENWARE);
			return entity;
		}
		//!時計
		static Entity* CreateClockUI(const char* graphicName, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addGroup(ENTITY_GROUP::UI);
			entity->addComponent<SpriteDraw>(graphicName);
			return entity;
		}
		//!時計の針
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
		//!スコア表示用フォント
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
		//! 斬った時の評価フォントエンティティ
		static Entity* CreateRankFont(const char* graphicName, const Vec2& pos, EntityManager& entityManager_) 
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(graphicName);
			entity->addComponent<ChangeAlphaComponent>(5, 10);
			entity->addComponent<EasingPosMove>();
			entity->getComponent<EasingPosMove>().setDest(pos, Vec2{ pos.x,pos.y - 30.f }, 15);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
		//---------- ポーズ画面
		//ポーズ画面の背景
		static Entity* CreatePauseBG(const char* graphicName, const Vec2& pos, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteDraw>(graphicName).setPivot(Vec2{0.f,0.f});
			entity->addGroup(ENTITY_GROUP::PAUSE_UI);
			return entity;
		}
		//ポーズ画面のパンネル
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
		//ポーズ画面のボタン
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
		//ポーズ画面のボタンの上にのせる文字
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
		//ポーズ画面の選択肢フレーム
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