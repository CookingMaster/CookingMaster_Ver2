/**
* @file ResultComponent.hpp
* @brief リザルト特有のコンポーネントを作成する
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
	/**
	* @brief クロッシュが回転しながらびゅーん
	*/
	class FlyAway final : public ComponentSystem
	{
	private:
		Position* position_;
		Rotation* rotate_;
		Counter_f cnt_;

		//! 移動するときの回転中心
		Vec2 center_;
		//! 回転速度
		float speed_;

		static constexpr float PI = 3.141592653589793f;
	public:
		//!移動の回転中心と速度を初期化します
		FlyAway(const Vec2 center, const float speed)
			:
			center_(center),
			speed_(speed)
		{}

		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
			rotate_ = &entity->getComponent<Rotation>();
			cnt_.SetCounter(0.f, speed_, 0.f, 360.f);
		}

		void update() override
		{
			cnt_.add();
			//画像自体の回転
			rotate_->val = cnt_.getCurrentCount();
			//画像の回転移動処理
			double angle = PI / 180.0;
			position_->val.x = static_cast<float>(
				center_.x 
				+ (position_->val.x - center_.x) * cos(angle) 
				- (position_->val.y - center_.y) * sin(angle)
				);
			position_->val.y = static_cast<float>(
				center_.y 
				+ (position_->val.x - center_.x) * sin(angle) 
				+ (position_->val.y - center_.y) * cos(angle)
				);
		}
	};

	/**
	* @brief 画像の拡大を行う
	*- 指定したイージングで拡大
	*/
	class Expand final : public ComponentSystem 
	{
	private:
		Scale* scale_;
		Easing ease_;
		Vec2 endSize_;
		Ease em_;
		float durationTime_;
	public:
		Expand(const Vec2 endSize, const Ease em, float durationTime) 
			:
			endSize_(endSize),
			em_(em),
			durationTime_(durationTime)
		{}
		void initialize() override {
			scale_ = &entity->getComponent<Scale>();
		}
		void update() override {
			ease_.run(em_, durationTime_);
			scale_->val.x = ease_.getVolume(1, endSize_.x);
			scale_->val.y = ease_.getVolume(1, endSize_.y);
		}
	};

	/**
	* @brief 指定スピードで回転させるだけ
	*/
	class Rotate final : public ComponentSystem 
	{
	private:
		Rotation* rotation_;
		float speed_;
	public:
		Rotate(float speed)
			:
			speed_(speed)
		{}
		void initialize() override 
		{
			rotation_ = &entity->getComponent<Rotation>();
		}
		void update() override 
		{
			rotation_->val += speed_;
		}
	};
	
	/**
	* @brief x方向に往復しながら下に落ちる
	* @param shift 左右にずれる量
	* @param angleInit 移動に使う角度の初期値
	* @param rotateSpeed 左右に振れる速度
	* @note これは画像の回転には関係ない、回転は別Component
	*/
	class FallDance final : public ComponentSystem
	{
	private:
		Position* position_;
		float shift_;
		Counter_f angleCnt_;
	public:
		FallDance(float shift, float angleInit, float rotateSpeed)
			:
			shift_(shift)
		{
			angleCnt_.SetCounter(angleInit, rotateSpeed, 0.f, 360.f);
		}
		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
		}
		void update() override 
		{
			position_->val.x += static_cast<float>(sin(++angleCnt_) * 10.f) + shift_;
			position_->val.y += 10;
		}
	};
}