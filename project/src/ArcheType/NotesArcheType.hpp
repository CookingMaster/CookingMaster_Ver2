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
		static Entity* CreateNotes(const NotesData& notesData, const ECS::Direction::Dir& dir, float wait, float arrivalBeatTime, float targetPosX, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			float posX;
			if (dir == ECS::Direction::Dir::L)
			{
				posX = notesData.xsize / -2.f;
				entity->addComponent<Transform>().setPosition(posX, float(System::SCREEN_HEIGHT) / 2.f);
			}
			else
			{
				posX = System::SCREEN_WIDIH + (notesData.xsize / 2.f);
				entity->addComponent<Transform>().setPosition(posX, float(System::SCREEN_HEIGHT) / 2.f);
			}
			float grav = 0.5f;

			//横方向の移動速度と、上にどの程度飛ぶかの計算を行う
			entity->addComponent<Velocity>((targetPosX - posX) / arrivalBeatTime, -grav * (arrivalBeatTime / 2.f));
			
			entity->addComponent<Gravity>(grav);
			entity->addComponent<Physics>();

			entity->addComponent<Color>();
			
			entity->addComponent<SpriteAnimationDraw>(notesData.imageName.c_str()).setPivot(
				Vec2(notesData.xsize / 2.f, notesData.ysize / 2.f));
			entity->addComponent<AnimatorByFrame>(notesData.animFlame).setSpriteNum(
				0, 0, notesData.xnum, notesData.ynum);

			entity->addComponent<NoteState>();
			entity->addComponent<NoteStateTransition>(notesData.hitJudge, arrivalBeatTime);

			entity->addComponent<ReplayNotesComponents>(int(wait));

			entity->addGroup(ENTITY_GROUP::NOTE);
			return entity;
		}
	};
}