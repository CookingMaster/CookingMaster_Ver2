/**
* @file UIComponents.hpp
* @brief UIのコンポーネント群です。
* @author moon
* @date 2018/10/11
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
	/*!
	@brief 時計の針の動きです
	*/
	class TimerNeedleMove final : public ComponentSystem
	{
	private:
		Rotation* rotate_;
		Counter_f cnt_;

		float speed_;
	public:
		//!速度を初期化します。高いほど早い 最大360
		TimerNeedleMove(const float speed)
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
	@brief フェードインします
	*/
	class FadeInComponentSystem final : public ComponentSystem
	{
	private:
		AlphaBlend* alpha_;
		Counter cnt_;

		int speed_;
	public:
		//!フェイドの速度を初期化します。高いほど早い　最大255
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
	@brief フェードアウトします
	*/
	class FadeOutComponentSystem final : public ComponentSystem
	{
	private:
		AlphaBlend* alpha_;
		Counter cnt_;

		int speed_;
	public:
		//!フェイドの速度を初期化します。高いほど早い　最大255
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
	@brief 画像を拡大し、また元の画像に戻ります
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
		*@brief 画像を拡大し、また縮小します
		* @param magni 倍率
		* @param speed 速さ（大きいほど速い）
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
		*@brief 画像を拡大します
		* @param magni_min_　始まる数値（1が元サイズ）
		* @param magni_max_ 拡大する数値（1が元サイズ）
		* @param speed 速さ（大きいほど速い）
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
	*@brief　X軸のバーを描画します
	SpriteRectDraw、Rectがないとエラーが出ます
	*　rect_x_	元画像の全体の幅
	*　now_		現在の数値
	*　max_		最大数値
	*　size_w_	表示する画像の幅
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
			rectangle_->w = (int)eas_.getVolume((float)rectangle_->w, size_w_- (float)rectangle_->w);
			
		}
		void addScore(int score)
		{
			atScore_ = score_;
			score_ += score;
		}
		int getScore()
		{
			return score_;
		}
	};

	/*
	*@brief　Y軸のバーを描画します
	SpriteRectDraw、Rectがないとエラーが出ます
	*　rect_y_	元画像の全体の高さ
	*　now_		現在の数値
	*　max_		最大数値
	*　size_h_	表示する画像の高さ
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
	*@brief フォント画像を描画します
	SpriteRectDraw、Rectがないとエラーが出ます
	*　rect_w	フォント1個の横幅
	*　num		表示する数字
	*/
	class DrawFont final : public ComponentSystem
	{
	private:
		Position* pos_;
		Rectangle* rectangle_;
		SpriteRectDraw* rectDraw_;
		
		int rectW_;
		int num_;
		int posX_;
		
		int font_[4];
		
		void setRect()
		{
			for (int i = 0; i < 4; ++i) 
			{
				if ((i == 0 && num_ < 100) || i == 1 && num_ < 10) 
				{
					continue;
				}
				rectangle_->x = font_[i] * rectW_;
				pos_->val.x = posX_ + (i * rectW_);
				rectDraw_->draw2D();
			}
		}

	public:
		DrawFont(int rect_w, int num)
		{
			rectW_ = rect_w;
			num_ = num;
		}

		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			rectangle_ = &entity->getComponent<Rectangle>();
			rectDraw_ = &entity->getComponent<SpriteRectDraw>();

			posX_ = pos_->val.x;
			rectangle_->x = rectW_ * num_;
			rectangle_->w = rectW_;
		}

		void update() override
		{
			rectangle_->x = rectW_ * num_;
			setRect();
		}

		void setNumber(int num)
		{
			num_ = num;
			font_[3] = 10;
			font_[2] = num % 10;
			font_[1] = num % 100 / 10;
			font_[0] = num / 100;
		}
	};


}