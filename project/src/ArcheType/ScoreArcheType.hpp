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
	struct ScoreArcheType
	{
		//!リザルト画面で得点を表示するためのエンティティです。生成時にハイスコアを保存します
		static Entity* CreateScoreEntity(const char* grapName, const Vec2& pos, const StageHighScore& stageName, const int score, EntityManager& entityManager_)
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
			entity->addComponent<DrawFont>(25,0).setNumber(score);
			entity->addGroup(ENTITY_GROUP::FADE);
			return entity;
		}
	};
}