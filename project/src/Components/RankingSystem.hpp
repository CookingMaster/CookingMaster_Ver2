/**
* @file RankingSystem.hpp
* @brief リザルト画面でスコアの演出を行うためのコンポーネント群です
* @author tonarinohito
* @date 2018/10/26
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Collision/Collision.hpp"
#include <DxLib.h>

namespace ECS
{
	struct ScoreData : public ComponentData
	{
		int val = 0;
		ScoreData() = default;
		ScoreData(const int score) : val(score) {}
	};

	class RankingSystem : public ComponentSystem
	{
	private:
		ScoreData* score_;
		Position* pos_;
	public:
		void initialize() override
		{
			score_ = &entity->getComponent<ScoreData>();
			pos_ = &entity->getComponent<Position>();
		}

		void update() override
		{

		}

		void draw2D() override
		{
			
		}

	};
}