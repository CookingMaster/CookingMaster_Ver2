/**
* @file NotesArcheType.hpp
* @brief �m�[�c�̌��^�𐶐�����
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
		//�m�[�c�𐶐�
		static Entity* CreateNotes(const NotesData& notesData, const ECS::Direction::Dir& dir, float wait, float arrivalBeatTime, int imgSize, float targetPosX, EntityManager& entityManager_)
		{
			auto* entity = &entityManager_.addEntity();

			float posX;
			if (dir == ECS::Direction::Dir::L)
			{
				posX = -float(imgSize / 2);
				entity->addComponent<Transform>().setPosition(posX, float(System::SCREEN_HEIGHT) / 2.f);
			}
			else
			{
				posX = float(System::SCREEN_WIDIH + imgSize / 2);
				entity->addComponent<Transform>().setPosition(posX, float(System::SCREEN_HEIGHT) / 2.f);
			}
			float grav = 0.5f;

			//横方向の移動速度と、上にどの程度飛ぶかの計算を行う
			entity->addComponent<Velocity>((targetPosX - posX) / arrivalBeatTime, -grav * (arrivalBeatTime / 2.f));
			
			entity->addComponent<Gravity>(grav);
			entity->addComponent<Physics>();

			entity->addComponent<Color>();

			entity->addComponent<SpriteDraw>(notesData.imageName.c_str());

			entity->addComponent<KillEntity>(int(arrivalBeatTime));
			entity->addComponent<ReplayPhysics>(int(wait));

			entity->addGroup(ENTITY_GROUP::LAYER1);
			return entity;
		}
	};
}