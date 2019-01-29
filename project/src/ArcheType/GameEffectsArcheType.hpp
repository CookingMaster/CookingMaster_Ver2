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
#include "../Components/BeatByTrigger.hpp"

namespace ECS
{
	struct GameEffectsArcheType
	{
		//!マーカー
		static Entity* CreateMarker(const char* graphicName, const std::string& musicName, int bpm, int beat, ECS::Direction::Dir dir, const Vec2& pos, EntityManager* entityManager)
		{
			auto& marker = entityManager->addEntity();
			marker.addComponent<Transform>().setPosition(pos.x, pos.y);
			marker.addComponent<Color>();
			marker.addComponent<AlphaBlend>();

			marker.addComponent<SpriteDraw>(graphicName);
			if (dir == ECS::Direction::Dir::L)
			{
				auto& draw = marker.getComponent<SpriteDraw>();
				draw.turnGraph();
			}
			marker.addComponent<MarkerBodyController>(bpm, beat, musicName);

			marker.addGroup(ENTITY_GROUP::MARKER);
			return &marker;
		}
		//!鍋
		static Entity* CreateSaucepan(const char* divGraphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteAnimationDraw>(divGraphicName).setPivot(Vec2{0.f,0.f});
			e.getComponent<Scale>().val;
			e.addComponent<Animator>(0,0,7,0,7);
			e.addGroup(ENTITY_GROUP::BACK_OBJECT);
			return &e;
		}
		static Entity* CreateFan(const char* graphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteDraw>(graphicName);
			e.getComponent<Scale>().val;
			class RotationFan : public ComponentSystem
			{
			private:
				Rotation* rota_ = nullptr;
			public:
				void initialize() override
				{
					rota_ = &entity->getComponent<Rotation>();
				}
				void update() override
				{
					rota_->val += 16;
				}
			};
			e.addComponent<RotationFan>();
			e.addGroup(ENTITY_GROUP::BACK_OBJECT);
			return &e;
		}
		//!斬撃エフェクト
		static Entity* CreateSlashEffect(
			const char* divGraphicName,
			const Vec2& pos,
			ECS::Direction::Dir dir,
			EntityManager* entityManager,
			AlphaBlend::BlendMode bm = AlphaBlend::BlendMode::ADD)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>().blendMode = bm;
			if (dir == ECS::Direction::Dir::R)
			{
				e.addComponent<SpriteAnimationDraw>(divGraphicName).turnGraph();
			}
			else
			{
				e.addComponent<SpriteAnimationDraw>(divGraphicName);
			}
			e.addComponent<Animator>(0, 0, 3, 0, 2).setIsEndStopAnim(true);
			e.addComponent<KillEntity>(8);
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
			e.addGroup(ENTITY_GROUP::MASTER);
			return &e;
		}
		//!おやっさんの顔(表情)
		static Entity* CreateBossFace(const char* divGraphicName, const Vec2& pos, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.addComponent<SpriteAnimationDraw>(divGraphicName);
			e.addGroup(ENTITY_GROUP::MASTER);
			return &e;
		}
		//!おやっさんのセリフ(吹き出し用)
		static Entity* CreateBossBalloon(const char* graphicName, const Vec2& pos,const int& lifeSpan, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.getComponent<Scale>().val = Vec2{ 0.f,0.f };
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<SpriteDraw>(graphicName);
			e.addComponent<KillEntity>(lifeSpan);
			e.addComponent<Canvas>();
			e.addComponent<ExpandComponentSystem>(0.f, 1.f, 10.f);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}

		//!おやっさんのセリフ(コンボ表記用)
		static Entity* CreateBossMessage(const char* graphicName, const Vec2& pos, const Rectangle& rectAngle, const int& combNum, const int& lifeSpan, EntityManager* entityManager)
		{
			auto& e = entityManager->addEntity();
			e.addComponent<Transform>().setPosition(pos.x, pos.y);
			e.getComponent<Scale>().val = Vec2{ 0.f,0.f };
			e.addComponent<Color>();
			e.addComponent<AlphaBlend>();
			e.addComponent<Rectangle>(rectAngle);
			e.addComponent<SpriteRectDraw>(graphicName);
			e.addComponent<DrawFont2>(59.f, 75.f, combNum);
			e.addComponent<KillEntity>(lifeSpan);
			e.addComponent<ExpandComponentSystem>(0.f, 1.f, 10.f);
			e.addGroup(ENTITY_GROUP::UI);
			return &e;
		}
	};
}	//namespace ECS