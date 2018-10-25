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
#include "../../Utility/Easing.hpp"
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
	@brief �摜���g�債�A�܂����̉摜�ɖ߂�܂�
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

	/*
	*@brief�@X���̃o�[��`�悵�܂�
	SpriteRectDraw�ARect���Ȃ��ƃG���[���o�܂�
	*�@rect_x_	���摜�̑S�̂̕�
	*�@now_		���݂̐��l
	*�@max_		�ő吔�l
	*�@size_w_	�\������摜�̕�
	*/
	class BarComponentSystemX final : public ComponentSystem
	{
	private:
		Rectangle* rectangle_;
		Easing eas_;

		int rect_x_;
		int score_;
		int atScore_;
		int max_;

	public:
		BarComponentSystemX(int rectX, int now, int max)
		{
			rect_x_ = rectX;
			score_ = now;
			max_ = max;
		}
		
		void initialize() override
		{
			rectangle_ = &entity->getComponent<Rectangle>();
			
		}
		void update() override
		{
			if (atScore_ < score_) {
				eas_.run(Easing::CircIn, 10);
			}
			if (eas_.isEaseEnd()) {
				atScore_ = score_;
				eas_.reset();
			}
			float size_w_ = score_ * rect_x_ / (float)max_;
			rectangle_->w = eas_.getVolume(rectangle_->w, size_w_- rectangle_->w);
			
		}
		void addScore(int score)
		{
			atScore_ = score_;
			score_ += score;
		}
	};

	/*
	*@brief�@Y���̃o�[��`�悵�܂�
	SpriteRectDraw�ARect���Ȃ��ƃG���[���o�܂�
	*�@rect_y_	���摜�̑S�̂̍���
	*�@now_		���݂̐��l
	*�@max_		�ő吔�l
	*�@size_h_	�\������摜�̍���
	*/
	/*class BarComponentSystemY final : public ComponentSystem
	{
	private:
		SpriteRectDraw* rect_;
		Rectangle* rectangle_;

		int rect_y_;
		int now_;
		int max_;
		int size_h_;

	public:

		BarComponentSystemY(int rectY, int now, int max)
		{
			rect_y_ = rectY;
			now_ = now;
			max_ = max;
		}

		void initialize() override
		{
			rect_ = &entity->getComponent<SpriteRectDraw>();
			rectangle_ = &entity->getComponent<Rectangle>();

			size_h_ = (int)(now_ * rectangle_->h / (float)max_);
		}
		void update() override
		{
			rect_->setRect(rectangle_->x, rectangle_->y, rectangle_->w, size_h_);
		}
	};*/

	/*
	*@brief �t�H���g�摜��`�悵�܂�
	SpriteRectDraw�ARect���Ȃ��ƃG���[���o�܂�
	*�@rect_w	�t�H���g1�̉���
	*�@num		�\�����鐔��
	*/
	class DrawFont final : public ComponentSystem
	{
	private:
		Rectangle* rectangle_;
		
		int rectW_;
		int num_;
		int srcX_;

	public:

		DrawFont(int rect_w, int num)
		{
			rectW_ = rect_w;
			num_ = num;
		}

		void initialize() override
		{
			rectangle_ = &entity->getComponent<Rectangle>();

			srcX_ = rectW_ * num_;
		}

		void update() override
		{
			rectangle_->x = srcX_;
			rectangle_->w = rectW_;
		}
	};


}