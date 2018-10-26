/**
* @file ScoreArcheType.hpp
* @brief ���U���g�Ő�������X�R�A�G���e�B�e�B�̌��^���쐬���܂�
* @author tonarinohito
* @date 2018/10/26
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/UIComponents.hpp"
#include "../Components/RankingSystem.hpp"

namespace ECS
{
	struct ScoreArcheType
	{
		static Entity* CreateScore(const char* grapName, const Vec2& pos, const int score, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();
			entity->addComponent<Rectangle>(0,0,0,45);
			entity->addComponent<ScoreData>(score);
			entity->addComponent<RankingSystem>();
			entity->addComponent<SpriteRectDraw>(grapName);
			entity->addComponent<DrawFont>(25,0).setNumber(score);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	};
}