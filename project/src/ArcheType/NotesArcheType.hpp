/**
* @file NotesArcheType.hpp
* @brief ノーツの原型を生成する
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/ReplayPhysics.hpp"
#include "../Class/NotesAndScoreData.hpp"

namespace ECS
{
	struct NotesArcheType
	{
		//ノーツを生成
		static Entity* CreateNotes(const NotesData& notesData, const OneNoteData::Direction& dir, float wait, float arrivalBeatTime, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			if (dir == OneNoteData::Direction::LEFT)
			{
				entity->addComponent<Transform>().setPosition(-32.f, float(System::SCREEN_HEIGHT) / 2.f);
				entity->addComponent<Velocity>(10, 0);
			}
			else
			{
				entity->addComponent<Transform>().setPosition(float(System::SCREEN_WIDIH), float(System::SCREEN_HEIGHT) / 2.f);
				entity->addComponent<Velocity>(-10, 0);
			}
			entity->addComponent<Gravity>(0.f);
			entity->addComponent<Physics>();

			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();

			entity->addComponent<SimpleDraw>(notesData.imageName.c_str());

			entity->addComponent<KillEntity>(int(arrivalBeatTime));
			entity->addComponent<ReplayPhysics>(int(wait));

			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	};
}