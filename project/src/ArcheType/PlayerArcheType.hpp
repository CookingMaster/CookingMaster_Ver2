/**
* @file PlayerArcheType.hpp
* @brief ノーツの原型を生成する
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Animator.hpp"
#include "../Components/PlayerController.hpp"

namespace ECS
{
	struct Player
	{
		static Entity* CreatePlayer(std::string& soundName, const char* graphicName, const Vec2& imgSize, const Vec2& pos, const int bpm, const int beat, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();
			entity->addComponent<Transform>().setPosition(pos.x, pos.y);
			Vec2 pivot(imgSize.x / 2.f, imgSize.y / 2.f);
			entity->addComponent<SpriteAnimationDraw>(graphicName).setPivot(pivot);

			entity->addComponent<Animator>(5, 3, 5);
			entity->addComponent<RecordAnimation>();
			entity->addComponent<BeatByTrigger>(bpm, beat, float(beat), soundName);
			entity->addComponent<PlayerState>();
			entity->addComponent<PlayerController>();

			entity->addGroup(ENTITY_GROUP::GIRL);

			return entity;
		}
	};
}