/**
* @file UIComponents.hpp
* @brief UI�̃R���|�[�l���g�Q�ł��B
* @author moon
* @date 2018/10/11
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Utility/Counter.hpp"
#include "Renderer.hpp"
#include <DxLib.h>
#include <functional>
namespace ECS
{
	/*!
	@brief ���v�̐j�̓����ł�
	*/
	class TimerNeedleMove final : public ComponentSystem
	{
	private:
		Rotation* rotate_;
		Counter cnt_;

		int speed_;
	public:
		//!���x�����������܂��B�����قǑ���
		TimerNeedleMove(const int speed)
		{
			speed_ = speed;
		}

		void initialize() override
		{
			rotate_ = &entity->getComponent<Rotation>();
			cnt_.SetCounter(0, speed_, 0, 360);
		}

		void update() override
		{
			cnt_.add();
			rotate_->val = cnt_.getCurrentCount();
		}
	};

	/*!
	@brief �t�F�[�h�C�����܂�
	*/
	class FadeInComponentSystem final : public ComponentSystem
	{
	private:
		AlphaBlend* alpha_;
		Counter cnt_;

		int speed_;
	public:
		//!�t�F�C�h�̑��x�����������܂�
		FadeInComponentSystem(const int speed)
		{
			speed_ = speed;
		}

		void initialize() override
		{
			alpha_ = &entity->getComponent<AlphaBlend>();
			cnt_.SetCounter(0, speed_, 0, 255);
		}

		void update() override
		{
			cnt_.add();
			alpha_->alpha = cnt_.getCurrentCount();
		}
	};

	/*!
	@brief �t�F�[�h�A�E�g���܂�
	*/
	class FadeOutComponentSystem final : public ComponentSystem
	{
	private:
		AlphaBlend* alpha_;
		Counter cnt_;

		int speed_;
	public:
		//!�t�F�C�h�̑��x�����������܂��B�����قǑ���
		FadeOutComponentSystem(const int speed)
		{
			speed_ = speed;
		}

		void initialize() override
		{
			alpha_ = &entity->getComponent<AlphaBlend>();
			cnt_.SetCounter(255, speed_, 0, 255);
		}
		void update() override
		{
			cnt_.sub();
			alpha_->alpha = cnt_.getCurrentCount();
		}
	};

}