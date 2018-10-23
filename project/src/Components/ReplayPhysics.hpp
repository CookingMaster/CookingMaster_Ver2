/**
* @file ReplayPhysics.hpp
* @brief ��~���Ă���Physics��KillEntity���ĊJ������R���|�[�l���g
* @author feveleK5563
* @date 2018/10/18
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BasicComponents.hpp"

namespace ECS
{
	/*
	@brief ��~���Ă���Physics��KillEntity���A�w��t���[����ɍĊJ������
	@details Physics��KillEntity��\�ߗp�ӂ��Ă����K�v������
	*/
	class ReplayPhysics final : public ComponentSystem
	{
	private:
		int cnt_;
	public:
		ReplayPhysics(const int span) :cnt_(span) {}

		void initialize() override
		{
			entity->stopComponent<Physics>();
			entity->stopComponent<KillEntity>();
		}
		void update() override
		{
			--cnt_;
			if (cnt_ <= 0)
			{
				entity->updateComponent<Physics>();
				entity->updateComponent<KillEntity>();
				entity->stopComponent<ReplayPhysics>();
			}
		}
	};
}