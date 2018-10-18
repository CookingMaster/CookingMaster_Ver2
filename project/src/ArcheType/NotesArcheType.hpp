/**
* @file NotesArcheType.hpp
* @brief ƒm[ƒc‚ÌŒ´Œ^‚ð¶¬‚·‚é
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Class/NotesAndScoreData.hpp"

namespace ECS
{
	struct NotesArcheType
	{
		//ƒm[ƒc‚ð¶¬
		static Entity* CreateNotes(const NotesData& notesData, const OneNoteData::Direction& dir, float wait, float arrivalBeatTime, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			if (dir == OneNoteData::Direction::LEFT)
			{
				entity->addComponent<Transform>().setPosition(-32.f, float(System::SCREEN_HEIGHT) / 2.f);
				entity->addComponent<Velocity>(5, 0);
			}
			else
			{
				entity->addComponent<Transform>().setPosition(float(System::SCREEN_WIDIH), float(System::SCREEN_HEIGHT) / 2.f);
				entity->addComponent<Velocity>(-5, 0);
			}
			entity->addComponent<Gravity>(0.f);
			entity->addComponent<Physics>();
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();

			entity->addComponent<SimpleDraw>(notesData.imageName.c_str());

			entity->addComponent<KillEntity>(arrivalBeatTime);

			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	};
}