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
		//!���x�����������܂��B�����قǑ��� �ő�360
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
		//!�t�F�C�h�̑��x�����������܂��B�����قǑ����@�ő�255
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
		//!�t�F�C�h�̑��x�����������܂��B�����قǑ����@�ő�255
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

	/*!
	@brief �摜���g�債�A�܂��k�����܂�
	*/
	class ExpandReduceComponentSystem final : public ComponentSystem
	{
	private:
		Scale* scale_;
		Counter_f cnt_;

		float magni_;
		float speed_;
		int frame_;
	public:
		/**
		*@brief �摜���g�債�A�܂��k�����܂�
		* @param magni �{��
		* @param speed �����i�傫���قǑ����j
		*/
		ExpandReduceComponentSystem(float magni, float speed)
		{
			magni_ = magni;
			speed_ = speed;
			frame_ = 0;
		}
		
		void initialize() override
		{
			scale_ = &entity->getComponent<Scale>();
			cnt_.SetCounter(1.f, speed_, 1.f, magni_);
		}
		void update() override
		{
			++frame_;
			if (frame_ < magni_ / speed_)
			{
				cnt_.add();
			}
			if (cnt_.isMax())
			{
				cnt_.sub();
			}
			scale_->val = cnt_.getCurrentCount();
		}
	};

	class ExpandComponentSystem final : public ComponentSystem
	{
	private:
		Scale* scale_;
		Counter_f cnt_;

		float magni_min_;
		float magni_max_;
		float speed_;
	public:
		/**
		*@brief �摜���g�債�܂�
		* @param magni_min_�@�n�܂鐔�l�i1�����T�C�Y�j
		* @param magni_max_ �g�傷�鐔�l�i1�����T�C�Y�j
		* @param speed �����i�傫���قǑ����j
		*/
		ExpandComponentSystem(float magni_min, float magni_max, float speed)
		{
			magni_min_ = magni_min;
			magni_max_ = magni_max;
			speed_ = speed;
		}

		void initialize() override
		{
			scale_ = &entity->getComponent<Scale>();
			cnt_.SetCounter(magni_min_, speed_, magni_min_, magni_max_);
		}

		void update() override
		{
			cnt_.add();
			scale_->val = cnt_.getCurrentCount();
		}
	};
}