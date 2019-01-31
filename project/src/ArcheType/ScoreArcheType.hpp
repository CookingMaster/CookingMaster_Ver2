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
#include "../Components/ScoreSystem.hpp"

namespace ECS
{
	struct ScoreArcheType final
	{

		//!���U���g��ʂœ��_��\�����邽�߂̃G���e�B�e�B�ł��B�������Ƀn�C�X�R�A��ۑ����܂�
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

		//�X�R�A���O���t�@�C���ɏo�͂��܂��B�n�C�X�R�A�łȂ��ꍇ�X�V����܂���
		static Entity* CreateScoreEntity(const StageHighScore& stageName, const int score, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<ScoreData>(score);
			entity->addComponent<ScoreSystem>(stageName);
			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
		//!�Z���N�g��ʂœ��_��\�����邽�߂̃G���e�B�e�B�ł��B
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