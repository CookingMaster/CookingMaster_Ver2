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
		AlphaBlend* alpha_ = nullptr;
		Easing ease_;
		Counter waitCnt_;
		bool bright_;
		bool isDelete_;
		float deleteTime_;

	public:
		FlashImage() :
			waitCnt_(0, 1, 0, 0),
			bright_(false),
			isDelete_(false),
			deleteTime_(0.f) {}

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
			waitCnt_.add();
			if (!waitCnt_.isMax())
			{
				return;
			}

			if (isDelete_)
			{
				ease_.run(Easing::CubicOut, deleteTime_);
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

		void setIsDelete(bool isDelete, float deleteTime = 30.f)
		{
			isDelete_ = isDelete;
			deleteTime_ = deleteTime;
			ease_.reset();
		}

		void setWaitTime(int waitTime)
		{
			waitCnt_.reset();
			waitCnt_.setCounter(0, 1, 0, waitTime);
		}
	};

	/*!
	@brief イージングで座標を操作する、透過具合も操作できる
	* - Positionが必要、なければ勝手に作る
	*/
	class EasingPosMove final : public ComponentSystem
	{
	private:
		Position* pos_ = nullptr;		//座標
		AlphaBlend* alpha_ = nullptr;	//アルファブレンド
		Easing ease_;		//イージング
		Vec2 start_;		//スタート位置
		Vec2 goal_;			//ゴール位置
		float durationTime;	//経過時間

		bool isTrans_;	//透過するか否か

	public:
		EasingPosMove() :
			start_(0, 0),
			goal_(0, 0),
			durationTime(60.f),
			isTrans_(false){}

		void initialize() override
		{
			if (!entity->hasComponent<Position>())
			{
				entity->addComponent<Position>();
			}
			pos_ = &entity->getComponent<Position>();

			if (!entity->hasComponent<AlphaBlend>())
			{
				entity->addComponent<AlphaBlend>();
			}
			alpha_ = &entity->getComponent<AlphaBlend>();
		}

		void update() override
		{
			ease_.run(ease_.CircOut, durationTime);
			pos_->val.x = ease_.getVolume(start_.x, goal_.x);
			pos_->val.y = ease_.getVolume(start_.y, goal_.y);

			if (isTrans_)
			{
				alpha_->alpha = int(ease_.getVolume(0, 255));
			}
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

		//イージングで透過させるか否か
		void isSetAlphaBlend(bool isTrans)
		{
			isTrans_ = isTrans;
			alpha_->alpha = 0;
		}
	};

	/*!
	@brief なんかのボタン入力があったら、引数に指定した関数を実行する
	*/
	class AnyInputFunction final : public ComponentSystem
	{
	public:
		typedef void(*Func)(ECS::Entity*, EntityManager&, bool&);

	private:
		Func func_;
		EntityManager& entityManager_;
		bool isPushed;

	public:
		AnyInputFunction(Func func, EntityManager& entityManager) :
			func_(func),
			entityManager_(entityManager),
			isPushed(false){}

		void update() override
		{
			auto& input = Input::Get();
			if (input.getIsAnyInput())
			{
				func_(entity, entityManager_, isPushed);
			}
		}

		bool getIsPushed()
		{
			return isPushed;
		}
	};

	/*!
	@brief 画像の指定座標に向かって拡大する
	* - PositionとScaleとSpriteDrawが必要
	*/
	class ZoomIn final : public ComponentSystem
	{
	private:
		Position* pos_ = nullptr;
		Scale* scale_ = nullptr;
		SpriteDraw* sd = nullptr;
		Counter_f cnt_;
		Vec2 zoomPos_;

		bool stopFlag_;

	public:
		ZoomIn(const float zoomSpd, const Vec2& zoomPos) :
			cnt_(1.f, zoomSpd, 1.f, 255.f),
			zoomPos_(zoomPos),
			stopFlag_(false) {}

		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			sd = &entity->getComponent<SpriteDraw>();
			scale_ = &entity->getComponent<Scale>();

			sd->setPivot(zoomPos_ - pos_->val);
			pos_->val += zoomPos_ - pos_->val;	//画像の左上が基準であるかのように見せる
		}

		void update() override
		{
			if (!stopFlag_)
			{
				scale_->val.x = cnt_.getCurrentCount();
				scale_->val.y = cnt_.getCurrentCount();
				++cnt_;
			}
		}

		void setStop(bool isStop)
		{
			stopFlag_ = isStop;
		}
	};

	/*!
	@brief タイトル画面で使っている動くやつらの動き（ドアとか雲とか）
	* - PositionとSpriteDrawが必要
	*/
	class TitleObjectsMover final : public ComponentSystem
	{
	private:
		Position* pos_ = nullptr;
		SpriteDraw* sd_ = nullptr;
		Vec2 spd_;
		float sizex = 0.f;

	public:
		TitleObjectsMover(const Vec2& spd):
			spd_(spd){}

		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			if (entity->hasComponent<SpriteDraw>())
			{
				sd_ = &entity->getComponent<SpriteDraw>();
				sizex = float(sd_->getSize().x);
			}
		}

		void update() override
		{
			pos_->val += spd_;

			if (System::SCREEN_WIDTH + sizex < int(pos_->val.x))
			{
				pos_->val.x = -sizex;
			}
		}
	};
}
