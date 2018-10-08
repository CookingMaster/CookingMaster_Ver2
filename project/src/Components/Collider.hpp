/**
* @file Collider.hpp
* @brief コリジョンに必要なコンポーネント群です。
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Collision/Collision.hpp"
#include <DxLib.h>

namespace ECS
{
	class IBoxCollider
	{
	public:
		IBoxCollider() = default;
		virtual ~IBoxCollider() = default;
		/** @brief コリジョンの色を指定します*/
		virtual void setColor(const int r, const int g, const int b) = 0;
		/** @brief コリジョンの位置を変更します*/
		virtual void setOffset(const float x, const float y) = 0;
		/** @brief コリジョンを塗りつぶします*/
		virtual void fillEnable() = 0;
		/** @brief コリジョンの塗りつぶしを解除します*/
		virtual void fillDisable() = 0;
		/** @brief コリジョンの描画を有効にします*/
		virtual void drawEnable() = 0;
		/** @brief コリジョンの描画を無効にします*/
		virtual void drawDisable() = 0;
		/** @brief 幅を返します*/
		virtual float w() const = 0;
		/** @brief 高さを返します*/
		virtual float h() const = 0;
		/** @brief x座標を返します、この値はオフセットされた値です*/
		virtual float x() const = 0;
		/** @brief y座標を返します、この値はオフセットされた値です*/
		virtual float y() const = 0;
	};

	class ICircleCollider
	{
	public:
		ICircleCollider() = default;
		virtual ~ICircleCollider() = default;
		/** @brief コリジョンの色を指定します*/
		virtual void setColor(const int r, const int g, const int b) = 0;
		/** @brief コリジョンの位置を変更します*/
		virtual void setOffset(const float x, const float y) = 0;
		/** @brief コリジョンを塗りつぶします*/
		virtual void fillEnable() = 0;
		/** @brief コリジョンの塗りつぶしを解除します*/
		virtual void fillDisable() = 0;
		/** @brief コリジョンの描画を有効にします*/
		virtual void drawEnable() = 0;
		/** @brief コリジョンの描画を無効にします*/
		virtual void drawDisable() = 0;
		/** @brief 半径を返します*/
		virtual float radius() const = 0;
		/** @brief x座標を返します、この値はオフセットされた値です*/
		virtual float x() const = 0;
		/** @brief y座標を返します、この値はオフセットされた値です*/
		virtual float y() const = 0;
	};

	/*!
	@brief 矩形です.
	@details  Positionが必要です
	*/
	class BoxCollider final : public ComponentSystem, public IBoxCollider
	{
	private:
		Position* pos_ = nullptr;
		Vec2 offSetPos_;
		float w_, h_;
		unsigned int color_ = 4294967295;
		bool isFill_ = false;
		bool isDraw_ = true;
	public:
		explicit BoxCollider(const float ww, const float hh)
		{
			w_ = ww;
			h_ = hh;
		}
		~BoxCollider()
		{
			pos_ = nullptr;
		}
		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
		}
		void draw2D() override
		{
			if (isDraw_)
			{
				auto convert = pos_->val.offSetCopy(offSetPos_.x, offSetPos_.y);
				DrawBoxAA(
					convert.x,
					convert.y,
					convert.x + w(),
					convert.y + h(),
					color_, isFill_, 2);
			}
		}
		void setColor(const int r, const int g, const int b) override
		{
			color_ = GetColor(r, g, b);
		}
		void setOffset(const float x, const float y) override
		{
			offSetPos_.x = x;
			offSetPos_.y = y;
		}
		void fillEnable() override { isFill_ = true; }
		void fillDisable() override { isFill_ = false; }
		void drawEnable() override { isDraw_ = true; }
		void drawDisable() override { isDraw_ = false; }
		float w() const override { return w_; }
		float h() const override { return h_; }
		float x() const override { return pos_->val.x + offSetPos_.x; }
		float y() const override { return pos_->val.y + offSetPos_.y; }
	};
	

	/*!
	@brief 円です.
	@details  Positionが必要です
	*/
	class CircleCollider final : public ComponentSystem, public ICircleCollider
	{
	private:
		Position* pos_ = nullptr;
		Vec2 offSetPos_;
		float r_;
		unsigned int color_ = 4294967295;
		bool isFill_ = false;
		bool isDraw_ = true;
	public:
		explicit CircleCollider(const float r)
		{
			r_ = r;
		}
		~CircleCollider()
		{
			pos_ = nullptr;
		}
		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
		}
		void draw2D() override
		{
			if (isDraw_)
			{
				auto convert = pos_->val.offSetCopy(offSetPos_.x, offSetPos_.y);
				DrawCircleAA(
					convert.x,
					convert.y,
					r_,
					24,
					color_, isFill_, 2);
			}
		}
		void setColor(const int r, const int g, const int b) override
		{
			color_ = GetColor(r, g, b);
		}
		void setOffset(const float x, const float y) override
		{
			offSetPos_.x = x;
			offSetPos_.y = y;
		}
		void fillEnable() override { isFill_ = true; }
		void fillDisable() override { isFill_ = false; }
		void drawEnable() override { isDraw_ = true; }
		void drawDisable() override { isDraw_ = false; }
		float radius() const override { return r_; }
		float x() const override { return pos_->val.x + offSetPos_.x; }
		float y() const override { return pos_->val.y + offSetPos_.y; }
	};
}