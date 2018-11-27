/**
* @file TitleUIComponents.hpp
* @brief �^�C�g���V�[���Ŏg�p����UI�̂��߂ɍ�����R���|�[�l���g�̕��X
* @author feveleK5563
* @date 2018/11/22
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "Renderer.hpp"
#include "../Utility/Easing.hpp"

namespace ECS
{
	/*!
	@brief �摜��_�ł�����i����������j
	* - AlphaBlend���K�v�A�Ȃ���Ώ���ɍ��
	*/
	class FlashImage final : public ComponentSystem
	{
	private:
		AlphaBlend* alpha_;
		Easing ease_;
		bool bright_;

	public:
		FlashImage(): bright_(false){}

		void initialize() override
		{
			if (!entity->hasComponent<AlphaBlend>())
			{
				entity->addComponent<AlphaBlend>();
			}
			alpha_ = &entity->getComponent<AlphaBlend>();
			alpha_->alpha = 255;
		}

		void update() override
		{
			if (bright_)
			{
				ease_.run(Easing::CubicOut, 60.f);
				alpha_->alpha = int(ease_.getVolume(150.f, 255.f));
			}
			else
			{
				ease_.run(Easing::CubicIn, 60.f);
				alpha_->alpha = int(ease_.getVolume(255.f, 150.f));
			}

			if (ease_.isEaseEnd())
			{
				bright_ = !bright_;
				ease_.reset();
			}
		}
	};

	/*!
	@brief �C�[�W���O�ō��W�𑀍삷��
	* - Position���K�v�A�Ȃ���Ώ���ɍ��
	*/
	class EasingPosMove final : public ComponentSystem
	{
	private:
		Position* pos_;		//���W
		Easing ease_;		//�C�[�W���O
		Vec2 start_;	//�X�^�[�g�ʒu
		Vec2 goal_;		//�S�[���ʒu
		float durationTime;	//�o�ߎ���1

	public:
		EasingPosMove(): 
			start_(0, 0),
			goal_(0, 0),
			durationTime(60.f){}

		void initialize() override
		{
			if (!entity->hasComponent<Position>())
			{
				entity->addComponent<Position>();
			}
			pos_ = &entity->getComponent<Position>();
		}

		void update() override
		{
			ease_.run(ease_.CircOut, durationTime);
			pos_->val.x = ease_.getVolume(start_.x, goal_.x);
			pos_->val.y = ease_.getVolume(start_.y, goal_.y);
		}

		void setDest(const Vec2& start, const Vec2& goal, float time)
		{
			start_ = start;
			goal_ = goal;
			durationTime = time;
			ease_.reset();
		}

		bool getIsEaseEnd()
		{
			return ease_.isEaseEnd();
		}

	};
}