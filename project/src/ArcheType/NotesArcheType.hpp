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
		static Entity* CreateNotes(const NotesData& notesData, const OneNoteData::Direction& dir, float wait, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			if (dir == OneNoteData::Direction::LEFT)
			{
				entity->addComponent<Transform>().setPosition(-32.f, 100.f);
				entity->addComponent<Velocity>(0, 5);
			}
			else
			{
				entity->addComponent<Transform>().setPosition(float(System::SCREEN_WIDIH), 100.f);
				entity->addComponent<Velocity>(0, -5);
			}
			entity->addComponent<Color>();
			entity->addComponent<AlphaBlend>();

			entity->addComponent<SpriteAnimationDraw>(notesData.imageName.c_str()).setIndex(1);
			entity->getComponent<SpriteAnimationDraw>().setPivot(Vec2{32, 32});

			entity->addComponent<KillEntity>(notesData.arrivalBeatTime);

			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	};
}