/**
* @file ResultComponent.hpp
* @brief ���U���g���L�̃R���|�[�l���g���쐬����
* @author yokota0717
* @date 2018.11.22
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Utility/Counter.hpp"
#include "Renderer.hpp"
#include "../Utility/Easing.hpp"
#include <DxLib.h>
#include <functional>

namespace ECS
{
	class FlyAway final : public ComponentSystem
	{
	private:
		Position* position_;
		Rotation* rotate_;
		Counter_f cnt_;
		Counter counter_;

		//! �ړ�����Ƃ��̉�]���S
		Vec2 center_;
		float speed_;

		static constexpr float PI = 3.141592653589793f;
	public:
		//!�ړ��̉�]���S�Ƒ��x�����������܂�
		FlyAway(const Vec2 center, const float speed)
		{
			center_ = center;
			speed_ = speed;
		}

		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
			rotate_ = &entity->getComponent<Rotation>();
			cnt_.SetCounter(0, speed_, 0, 360);
			counter_.SetCounter(0, 10, 0, 360);
		}

		void update() override
		{
			cnt_.add();
			counter_.add();
			rotate_->val = cnt_.getCurrentCount();
			float angle = counter_.getCurrentCount() * PI / 180.f;

			position_->val.x += cos(static_cast<double>(angle));
			position_->val.y -= sin(static_cast<double>(angle));
		}
	};
}