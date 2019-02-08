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
	constexpr float PI = 3.1415926535f;

	/**
	* @brief クロッシュが回転しながらびゅーん
	*/
	class FlyAway final : public ComponentSystem
	{
	private:
		Position * position_ = nullptr;
		Rotation* rotate_ = nullptr;
		Counter_f cnt_;

		//! 移動するときの回転中心
		Vec2 center_;
		//! 回転速度
		float speed_;

		static constexpr float PI = 3.141592653589793f;
	public:
		//!移動の回転中心と回転速度を初期化します
		FlyAway(const Vec2 center, const float speed)
			:
			center_(center),
			speed_(speed)
		{}

		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
			rotate_ = &entity->getComponent<Rotation>();
			cnt_.setCounter(0.f, speed_, 0.f, 360.f);
		}

		void update() override
		{
			cnt_.add();
			//画像自体の回転
			rotate_->val = cnt_.getCurrentCount();
			//画像の回転移動処理(遅かったから微妙に調整)
			float angle = PI / 180.0f + 0.015f;
			position_->val.x =
				center_.x
				+ (position_->val.x - center_.x) * cosf(angle)
				- (position_->val.y - center_.y) * sinf(angle);
			position_->val.y =
				center_.y
				+ (position_->val.x - center_.x) * sinf(angle)
				+ (position_->val.y - center_.y) * cosf(angle);
		}
	};

	/**
	* @brief 画像の拡大を行う
	*- 指定したイージングで拡大
	*/
	class Expand final : public ComponentSystem
	{
	private:
		Scale * scale_ = nullptr;
		Easing ease_;
		Vec2 startSize_;
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
		void initialize() override
		{
			scale_ = &entity->getComponent<Scale>();
			startSize_ = scale_->val;
		}
		void update() override
		{
			ease_.run(em_, durationTime_);
			scale_->val.x = ease_.getVolume(startSize_.x, endSize_.x);
			scale_->val.y = ease_.getVolume(startSize_.y, endSize_.y);
		}
		[[nodiscard]] bool isEaseEnd()
		{
			return ease_.isEaseEnd();
		}
	};
	/**
	* @brief 画像の縮小を行う
	*- 指定したイージングで縮小
	*/
	class Reduction final : public ComponentSystem
	{
	private:
		Scale * scale_ = nullptr;
		Easing ease_;
		Vec2 startSize_;
		Vec2 endSize_;
		Ease em_;
		float durationTime_;
	public:
		Reduction(const Vec2 endSize, const Ease em, float durationTime)
			:
			endSize_(endSize),
			em_(em),
			durationTime_(durationTime)
		{}
		void initialize() override
		{
			scale_ = &entity->getComponent<Scale>();
			startSize_ = scale_->val;
		}
		void update() override
		{
			ease_.run(em_, durationTime_);
			scale_->val.x = ease_.getVolume(startSize_.x, endSize_.x);
			scale_->val.y = ease_.getVolume(startSize_.y, endSize_.y);
		}
		[[nodiscard]] const bool isEaseEnd()
		{
			return ease_.isEaseEnd();
		}
	};

	/**
	* @brief 指定スピードで回転させるだけ
	*/
	class Rotate final : public ComponentSystem
	{
	private:
		Rotation * rotation_ = nullptr;
		float speed_;
	public:
		Rotate(const float speed)
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
		Position * position_ = nullptr;
		float shift_;
		Counter_f angleCnt_;
	public:
		FallDance(const float shift, const float angleInit, const float rotateSpeed)
			:
			shift_(shift)
		{
			angleCnt_.setCounter(angleInit, rotateSpeed, 0.f, 360.f);
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

	/**
	* @brief 指定した座標までイージングで移動
	*/
	class EasingMove final : public ComponentSystem
	{
	private:
		Position * position_ = nullptr;
		Easing ease_;
		Ease em_;
		Vec2 startPos_;
		Vec2 endPos_;
		float durationTime_;
	public:
		EasingMove(const Ease em, Vec2 endPos, float durationTime)
			:
			em_(em),
			endPos_(endPos),
			durationTime_(durationTime)
		{}
		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
			startPos_ = position_->val;
		}
		void update() override
		{
			ease_.run(em_, durationTime_);
			position_->val.x = ease_.getVolume(startPos_.x, endPos_.x);
			position_->val.y = ease_.getVolume(startPos_.y, endPos_.y);
		}
		[[nodiscard]] const bool isEaseEnd()
		{
			return ease_.isEaseEnd();
		}
	};

	/**
	* @brief y座標が画面下に出たらエンティティを殺すコンポーネント
	* @param Vec2 size 画像サイズ
	*/
	class DestroyOutOfScreen final : public ComponentSystem
	{
	private:
		Position * position_ = nullptr;
		Vec2 size_;
	public:
		DestroyOutOfScreen(const Vec2& size)
			:
			size_(size)
		{}
		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
		}
		void update() override
		{
			if (position_->val.y - size_.y / 2 >= System::SCREEN_HEIGHT)
			{
				entity->destroy();
			}
		}
	};

	/**
	* @brief スポットライトの∞の動き
	*/
	class SpotLightMove final : public ComponentSystem
	{
	private:
		Position * position_ = nullptr;
		Counter_f counter_;
		float speed_;
		Vec2 startPos_;
		int moveCnt_;
	public:
		SpotLightMove(const float speed, const Vec2& startPos)
			:
			speed_(speed),
			startPos_(startPos)
		{}
		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
			counter_.setCounter(0.f, speed_, 0.f, 360.f);
			moveCnt_ = 0;
		}
		void update() override
		{
			counter_.add();
			if (counter_.isMax() && moveCnt_ <= 1)
			{
				counter_.reset();
				++moveCnt_;
			}
			position_->val.x += cosf(counter_.getCurrentCount()) * 500.f;
			position_->val.y += sinf(counter_.getCurrentCount() * 2.f) * 300.f;
		}
	};

	/**
	* @brief レムニスケート曲線で動くコンポーネント
	*/
	class Lemniscate final : public ComponentSystem
	{
	private:
		Position * position_ = nullptr;
		Counter_f counter_;
		float endX_;
	public:
		Lemniscate(const float endX)
			:
			endX_(endX)
		{}
		void initialize() override
		{
			position_ = &entity->getComponent<Position>();
			counter_.setCounter(0.f, 0.001f / 180.f * PI, 0.f, 360.f / 180.f * PI);
		}
		void update() override
		{
			counter_.add();
			if (counter_.isMax())
			{
				counter_.reset();
			}
			float a2 = powf(endX_, 2);
			position_->val.x += powf(2 * a2*cosf(2 * counter_.getCurrentCount()), 1 / 2.f) * cosf(counter_.getCurrentCount());
			position_->val.y += powf(2 * a2*cosf(2 * counter_.getCurrentCount()), 1 / 2.f) * sinf(counter_.getCurrentCount());
		}
	};
}
