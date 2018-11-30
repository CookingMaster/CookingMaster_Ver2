/**
* @file TitleUIComponents.hpp
* @brief タイトルシーンで使用するUIのために作ったコンポーネントの方々
* @author feveleK5563
* @date 2018/11/22
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "Renderer.hpp"
#include "../Utility/Easing.hpp"
#include "../Input/Input.hpp"

namespace ECS
{
	/*!
	@brief 画像を点滅させる（強調させる）、フラグをtrueにすると消滅する
	* - AlphaBlendが必要、なければ勝手に作る
	*/
	class FlashImage final : public ComponentSystem
	{
	private:
		AlphaBlend* alpha_;
		Easing ease_;
		bool bright_;
		bool isDelete_;

	public:
		FlashImage() :
			bright_(false),
			isDelete_(false) {}

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
			if (isDelete_)
			{
				ease_.run(Easing::CubicOut, 30.f);
				alpha_->alpha = int(ease_.getVolume(255.f, 0.f));
				if (ease_.isEaseEnd())
				{
					entity->destroy();
				}
			}
			else
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
		}

		void setIsDelete(bool isDelete)
		{
			isDelete_ = isDelete;
			ease_.reset();
		}
	};

	/*!
	@brief イージングで座標を操作する
	* - Positionが必要、なければ勝手に作る
	*/
	class EasingPosMove final : public ComponentSystem
	{
	private:
		Position* pos_;		//座標
		Easing ease_;		//イージング
		Vec2 start_;		//スタート位置
		Vec2 goal_;			//ゴール位置
		float durationTime;	//経過時間

	public:
		EasingPosMove() :
			start_(0, 0),
			goal_(0, 0),
			durationTime(60.f) {}

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

	/*!
	@brief なんかのボタン入力があったら、引数に指定した関数を実行する
	*/
	class AnyInputFunction final : public ComponentSystem
	{
	public:
		typedef void(*Func)(ECS::Entity*, EntityManager&);

	private:
		Func func_;
		EntityManager& entityManager_;

	public:
		AnyInputFunction(Func func, EntityManager& entityManager) :
			func_(func),
			entityManager_(entityManager) {}

		void update() override
		{
			auto& input = Input::Get();
			if (input.getIsAnyInput())
			{
				func_(entity, entityManager_);
			}
		}
	};

	/*!
	@brief 画像の指定座標に向かって拡大する
	* - PositionとScaleとSpriteDrawが必要
	*/
	class ZoomIn final : public ComponentSystem
	{
	private:
		Position* pos;
		Scale* scale;
		SpriteDraw* sd;
		Counter_f cnt_;
		Vec2 zoomPos_;

	public:
		ZoomIn(const float zoomSpd, Vec2 zoomPos) :
			cnt_(1.f, zoomSpd, 1.f, 255.f),
			zoomPos_(zoomPos) {}

		void initialize() override
		{
			pos = &entity->getComponent<Position>();
			pos->val += zoomPos_;

			scale = &entity->getComponent<Scale>();

			sd = &entity->getComponent<SpriteDraw>();
			sd->setPivot(zoomPos_);
		}

		void update() override
		{
			scale->val.x = cnt_.getCurrentCount();
			scale->val.y = cnt_.getCurrentCount();
			++cnt_;
		}
	};
}