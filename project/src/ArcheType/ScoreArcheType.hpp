/**
* @file ScoreArcheType.hpp
* @brief リザルトで生成するスコアエンティティの原型を作成します
* @author tonarinohito
* @date 2018/10/26
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/UIComponents.hpp"
#include "../Components/ScoreSystem.hpp"

namespace ECS
{
	struct ScoreArcheType final
	{

		//!リザルト画面で得点を表示するためのエンティティです。生成時にハイスコアを保存します
		static Entity* CreateResultScoreEntity(const char* grapName, const Vec2& pos, const StageHighScore& stageName, const int score, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0,0,0,45);
			entity->addComponent<ScoreData>(score);
			entity->addComponent<ScoreSystem>(stageName);
			entity->addComponent<ResultEffect>();
			entity->addComponent<SpriteRectDraw>(grapName).setPivot(Vec2{12.5f,22.5f});
			entity->addComponent<DrawFont>(25.f,0.f).setNumber(score);
			entity->addGroup(ENTITY_GROUP::FADE);
			return entity;
		}

		//スコアを外部ファイルに出力します。ハイスコアでない場合更新されません
		static Entity* CreateScoreEntity(const StageHighScore& stageName, const int score, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<ScoreData>(score);
			entity->addComponent<ScoreSystem>(stageName);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!セレクト画面で得点を表示するためのエンティティです。
		static Entity* CreateSelectScoreEntity(const char* grapName, const Vec2& pos, const StageHighScore& stageName, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0, 0, 500, 100);
			entity->addComponent<ScoreData>();
			int hi = entity->addComponent<ScoreSystem>(stageName).getHighScore(stageName);
			entity->addComponent<SpriteRectDraw>(grapName).setPivot(Vec2{ 12.5f,22.5f });
			entity->addComponent<DrawFont2>(50.f, 100.f, hi);
			entity->addGroup(ENTITY_GROUP::UI);
			return entity;
		}
	};
}