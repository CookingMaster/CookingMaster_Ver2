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
}