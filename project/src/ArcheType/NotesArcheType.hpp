/**
* @file NotesArcheType.hpp
* @brief ノーツの原型を生成する
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "../GameController/GameController.h"
#include "../Components/Renderer.hpp"
#include "../Components/ReplayNotesComponents.hpp"
#include "../Components/Animator.hpp"
#include "../Class/NotesAndScoreData.hpp"

namespace ECS
{
	struct NotesArcheType
	{
		static Entity* CreateNotes(
			const NotesData& notesData,
			const ECS::Direction::Dir& dir,
			float wait,
			float arrivalBeatTime,
			const Vec2& targetPos,
			EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			float posX = 0.f;
			switch (dir)
			{
			case ECS::Direction::Dir::L:
				posX = -(notesData.xsize / 2.f);	//出現するX座標
				entity->addComponent<Transform>().setPosition(posX, targetPos.y);
				break;

			case ECS::Direction::Dir::R:
				posX = System::SCREEN_WIDIH + (notesData.xsize / 2.f);	//出現するX座標
				entity->addComponent<Transform>().setPosition(posX, targetPos.y);
				break;
			}

			float gravity = 0.5f;
			//横方向の移動速度と、上にどの程度飛ぶかの計算を行う
			entity->addComponent<Velocity>((targetPos.x - posX) / arrivalBeatTime, -gravity * (arrivalBeatTime / 2.f));
			
			entity->addComponent<Gravity>(gravity);
			entity->addComponent<Physics>();
			
			entity->addComponent<AlphaBlend>();
			entity->addComponent<SpriteAnimationDraw>(notesData.imageName.c_str()).setPivot(
				Vec2(notesData.xsize / 2.f, notesData.ysize / 2.f));
			entity->addComponent<Animator>(notesData.animFlame, notesData.xnum, notesData.ynum).setSpriteNum(
				notesData.animSData[0].xmin,
				notesData.animSData[0].ymin,
				notesData.animSData[0].xmax,
				notesData.animSData[0].ymax,
				true);

			entity->addComponent<NoteState>().dir = dir;
			entity->addComponent<NoteStateTransition>(notesData, arrivalBeatTime);

			entity->addComponent<ReplayNotesComponents>(int(wait));

			entity->addComponent<KillEntity>(60);
			entity->stopComponent<KillEntity>();

			entity->addGroup(ENTITY_GROUP::NOTE);
			return entity;
		}
	};
}