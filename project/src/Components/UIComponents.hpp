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
		Counter cnt_;

		int speed_;
	public:
		//!速度を初期化します。高いほど早い 最大360
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
	@brief 画像を拡大し、また縮小します
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
}