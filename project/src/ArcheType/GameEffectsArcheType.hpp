/**
* @file GameEffectsArcheType.hpp
* @brief ゲームシーンで使う演出類の原型をまとめます
* @author tonarinohito
* @date 2018/12/21
*/
#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/Animator.hpp"
#include "../Components/PlayerController.hpp"
#include "../Components/UIComponents.hpp"
#include "../Components/TitleUIComponents.hpp"
#include "../Components/BossAnimator.hpp"

namespace ECS
{
	struct GameEffectsArcheType
	{
		//!マーカー
		static Entity* CreateMarker(const char* graphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& marker = entityManager->addEntity();
			marker.addComponent<Transform>().setPosition(pos.x, pos.y);
			marker.addComponent<Color>();
			marker.addComponent<AlphaBlend>();
			marker.addComponent<SpriteDraw>(graphicName);
			marker.addGroup(ENTITY_GROUP::UI);
			return &marker;
		}
		//!鍋
		static Entity* CreateSaucepan(const char* divGraphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteAnimationDraw>(divGraphicName);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
		//!斬撃エフェクト
		static Entity* CreateSlashEffect(const char* divGraphicName, const Vec2& pos, const int& lifeSpan,EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteAnimationDraw>(divGraphicName);
			//e.addComponent<Animator>();
			e.addComponent<KillEntity>(lifeSpan);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
		//!収縮円
		static Entity* CreateContractionCircle(const char* graphicName, const Vec2& pos, const int& lifeSpan, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteDraw>(graphicName);
			//e.addComponent<Contraction>();
			e.addComponent<KillEntity>(lifeSpan);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
		//!蛇口から出る水
		static Entity* CreateWater(const char* graphicName, const Vec2& pos, const ECS::Rectangle& rect,EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<Rectangle>(rect);
			e.addComponent<SpriteRectDraw>(graphicName);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
		//!開始の合図
		static Entity* CreateStartLogo(const char* graphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteDraw>(graphicName);
			e.addComponent<EasingPosMove>();
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
		//!料理進捗ゲージの型
		static Entity* CreateEmptyProgressUI(const char* graphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteDraw>(graphicName);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
		//!料理進捗ゲージ
		static Entity* CreateProgressUI(const char* graphicName, const Vec2& pos, const ECS::Rectangle& rect, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<Rectangle>(rect);
			e.addComponent<SpriteRectDraw>(graphicName);
			//e.addComponent<ProgressController>();
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}

		//!おやっさんの胴体(のっぺらぼう)
		static Entity* CreateBossBody(const char* divGraphicName, const Vec2& pos, int bpm, int beat, const std::string& soundName, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteAnimationDraw>(divGraphicName);
			e.addComponent<BeatByTrigger>(bpm, beat, float(beat), soundName);
			e.addComponent<BossAnimator>();
			e.addComponent<Canvas>();
			e.addGroup(ENTITY_GROUP::BACK_OBJECT);
			return &e;
		}
		//!おやっさんの顔(表情)
		static Entity* CreateBossFace(const char* divGraphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<SpriteAnimationDraw>(divGraphicName);
			e.addGroup(ENTITY_GROUP::BACK_OBJECT);
			return &e;
		}
		//!おやっさんのセリフ(吹き出し用)
		static Entity* CreateBossBalloon(const char* graphicName, const Vec2& pos,const int& lifeSpan, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteDraw>(graphicName);
			e.addComponent<KillEntity>(lifeSpan);
			e.addComponent<Canvas>();
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}

		//!おやっさんのセリフ(コンボ表記用)
		static Entity* CreateBossMessage(const char* graphicName, const Vec2& pos, const Rectangle& rectAngle, const int& combNum, const int& lifeSpan, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<Rectangle>(rectAngle);
			e.addComponent<SpriteRectDraw>(graphicName);
			e.addComponent<DrawFont2>(50.f, 100.f).setNumber(combNum);
			e.addComponent<KillEntity>(lifeSpan);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
	};
}	//namespace ECS