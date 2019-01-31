/**
* @file ReplayNotesComponents.hpp
* @brief ノーツに関するコンポーネントを再開させるコンポーネント
* @author feveleK5563
* @date 2018/10/18
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/NoteStateTransition.hpp"

namespace ECS
{
	/*
	@brief 停止しているPhysicsとNoteStateTransitionを、指定フレーム後に再開させる
	@details PhysicsとNoteStateTransitionを予め用意しておく必要がある
	*/
	class ReplayNotesComponents final : public ComponentSystem
	{
	private:
		int cnt_;
	public:
		ReplayNotesComponents(const int span) :cnt_(span) {}

		void initialize() override
		{
			entity->stopComponent<Physics>();
			entity->stopComponent<NoteStateTransition>();
		}
		void update() override
		{
			--cnt_;
			if (cnt_ <= 0)
			{
				entity->updateComponent<Physics>();
				entity->updateComponent<NoteStateTransition>();
				entity->removeComponent<ReplayNotesComponents>();
			}
		}
	};
}
