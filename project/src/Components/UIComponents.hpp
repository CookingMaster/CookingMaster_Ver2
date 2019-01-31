/**
* @file UIComponents.hpp
* @brief UIのコンポーネント群です。
* @author moon
* @date 2018/10/11
* @history
- 2018.11.20 yokota0717
-# ButtonComponent、ButtonMojiComponentの仕様変更
-# Pauseで使うSelectFrameを追加
- 2018/12/14 tonarinohito
-# DrawFont2を追加(DrawFontの%なしバージョン)
-# DrawFontの初期化処理を修正
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Utility/Counter.hpp"
#include "Renderer.hpp"
#include "../Utility/Easing.hpp"
#include "BeatByTrigger.hpp"
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
		Rotation * rotate_ = nullptr;
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
			cnt_.setCounter(0, speed_, 0, 360);
		}

		void update() override
		{
			cnt_.add();
			rotate_->val = cnt_.getCurrentCount();
		}
	};

	/*!
	@brief フェードイン・アウトを行う
	* - AlphaBlendが必要、なければ勝手に作る
	*/
	class FadeComponent final : public ComponentSystem
	{
	private:
		AlphaBlend * alpha_ = nullptr;
		Easing ease;
		float start_, end_, spd_;

	public:
		//!フェードの遷移(0~255)と継続時間を設定
		FadeComponent() :
			start_(0.f),
			end_(0.f),
			spd_(0.f) {}

		void initialize() override
		{
			if (!entity->hasComponent<AlphaBlend>())
			{
				entity->addComponent<AlphaBlend>();
			}
			alpha_ = &entity->getComponent<AlphaBlend>();
		}

		void update() override
		{
			ease.run(ease.LinearIn, spd_);
			alpha_->alpha = (int)ease.getVolume(start_, end_);
		}

		bool isFadeEnd()
		{
			return ease.isEaseEnd();
		}

		void reset(float start, float end, float speed)
		{
			start_ = start;
			end_ = end;
			spd_ = speed;
			ease.reset();
		}
	};

	/*!
	@brief 画像を拡大し、また元の画像に戻ります
	* @param magni 倍率
	* @param speed 速さ（大きいほど速い）、倍率で割れる数値ですること（0.1とか）　
	*/
	class ExpandReduceComponentSystem final : public ComponentSystem
	{
	private:
		Scale * scale_ = nullptr;
		Counter_f cnt_;

		float magni_;
		float speed_;
		float frame_;
		bool flag_;
	public:
		ExpandReduceComponentSystem(float magni, float speed)
		{
			magni_ = magni;
			speed_ = speed;
			frame_ = 0.f;
		}

		void initialize() override
		{
			scale_ = &entity->getComponent<Scale>();
			cnt_.setCounter(1.f, speed_, 1.f, magni_);
		}

		void update() override
		{
			frame_ = cnt_.getCurrentCount();
			if (flag_ == true) {
				if (frame_ < magni_ / speed_ && !cnt_.isMax())
				{
					cnt_.add();
					frame_ = cnt_.getCurrentCount();
				}
				if (cnt_.isMax())
				{
					cnt_.sub();
					frame_ = cnt_.getCurrentCount();
					if (frame_ <= 1.f) {
						flag_ = false;
						cnt_.reset(1.f);
					}
				}
				scale_->val = frame_;
			}
		}

		void onExpand(bool flag)
		{
			flag_ = flag;
		}
	};

	/**
	*@brief 画像をイージングで拡大、縮小する
	* @param magni_start_　始まる数値（1.0fが元サイズ）
	* @param magni_end_ 目標の数値（1.0fが元サイズ）
	* @param speed 速さ（大きいほど速い）
	*/
	class ExpandComponentSystem final : public ComponentSystem
	{
	private:
		Scale * scale_ = nullptr;
		Easing easing_;

		float start_, end_, speed_;

	public:
		ExpandComponentSystem(float start, float end, float speed):
			start_(start),
			end_(end),
			speed_(speed){}

		void initialize() override
		{
			scale_ = &entity->getComponent<Scale>();
		}

		void update() override
		{
			easing_.run(easing_.CubicIn, speed_);
			scale_->val = easing_.getVolume(start_, end_);
		}

		void reset()
		{
			easing_.reset();
			scale_->val = easing_.getVolume(start_, end_);
		}

		[[nodiscard]] const bool isEaseEnd()
		{
			return easing_.isEaseEnd();
		}
	};

	/*
	*@brief　X軸のバーを指定パーセンテージ分描画する
	SpriteRectDraw、Rectがないとエラーが出ます
	*　rect_x_	元画像の全体の幅
	*　now_		現在の数値
	*　max_		最大数値
	*　size_w_	表示する画像の幅
	*/
	/*class BarComponentSystemX final : public ComponentSystem
	{
	private:
		Rectangle * rectangle_;
		Easing ease_;

		int maxScore_;
		int imgRect_x_;
		int score_;

	public:
		BarComponentSystemX(int rectX, int maxScore) :
			maxScore_(maxScore),
			imgRect_x_(rectX),
			score_(0) {}

		void initialize() override
		{
			rectangle_ = &entity->getComponent<Rectangle>();
		}

		void update() override
		{
			if (!ease_.isEaseEnd())
			{
				ease_.run(Easing::CircIn, 10);
			}

			float size_w_ = imgRect_x_ * ((float)score_ / (float)maxScore_);
			rectangle_->w = (int)ease_.getVolume((float)rectangle_->w, size_w_);
		}

		void addScore(const int addscore)
		{
			score_ += addscore;
			if (score_ > maxScore_)
			{
				score_ = maxScore_;
			}
			ease_.reset();
		}

		int getScore()
		{
			return int(((float)score_ / (float)maxScore_) * 100.f);
		}
	};*/

	/*
	*@brief　Y軸のバーを描画します
	SpriteRectDraw、Rectがないとエラーが出ます
	*　rect_y_	元画像の全体の高さ
	*　now_		現在の数値
	*　max_		最大数値
	*　size_h_	表示する画像の高さ
	*/
	class BarComponentSystemY final : public ComponentSystem
	{
	private:
		Rectangle * rectangle_;
		Position* position_;
		Easing ease_;

		float posY = 0.f;
		int maxScore_ = 0;
		int imgRect_y_ = 0;
		int score_ = 0;

	public:
		BarComponentSystemY(int rectY, int maxScore) :
			maxScore_(maxScore),
			imgRect_y_(rectY) {}

		void initialize() override
		{
			rectangle_ = &entity->getComponent<Rectangle>();
			position_ = &entity->getComponent<Position>();
			posY = position_->val.y;
		}

		void update() override
		{
			if (!ease_.isEaseEnd())
			{
				ease_.run(Easing::CircIn, 10);
			}

			float size_h_ = (float)rectangle_->h - (imgRect_y_ * ((float)score_ / (float)maxScore_));
			rectangle_->y = (int)ease_.getVolume((float)rectangle_->y, size_h_);
			position_->val.y = posY + rectangle_->y;
		}

		void addScore(const int addscore)
		{
			score_ += addscore;
			if (score_ > maxScore_)
			{
				score_ = maxScore_;
			}
			ease_.reset();
		}

		int getScore()
		{
			return int(((float)score_ / (float)maxScore_) * 100.f);
		}
	};

	/*
	*@brief フォント画像を描画します
	SpriteRectDraw、Rectがないとエラーが出ます
	*　rect_w	フォント1個の横幅
	*　num		表示する数字
	*/
	class DrawFont final : public ComponentSystem
	{
	private:
		Position * pos_ = nullptr;
		Rectangle* rectangle_ = nullptr;
		SpriteRectDraw* rectDraw_ = nullptr;

		int num_;
		Vec2 rect_;
		Vec2 initPos_;

		int font_[4];

		void setRectAndDraw(const bool isDraw = true)
		{
			int keta = 0;
			for (int i = 0; i < std::size(font_); ++i)
			{
				//十の位、百の位が0の時は各値を表示しない
				if ((i == 1 && num_ < 10) || (i == 0 && num_ < 100))
				{
					++keta;
					continue;
				}
				rectangle_->x = font_[i] * (int)rect_.x;

				pos_->val.x = initPos_.x + (i * rect_.x);
				rectDraw_->setPivot(Vec2(rect_.x / 2.f, rect_.y / 2.f));
				if (isDraw)
				{
					rectDraw_->draw2D();
				}
			}
		}

	public:
		DrawFont(float rectW, float rectH) :
			num_(0)
		{
			rect_.x = rectW;
			rect_.y = rectH;
		}

		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			rectangle_ = &entity->getComponent<Rectangle>();
			rectDraw_ = &entity->getComponent<SpriteRectDraw>();

			initPos_ = pos_->val;
			rectangle_->x = (int)rect_.x * num_;
			rectangle_->w = (int)rect_.x;
			font_[3] = 10;
			//setRectAndDraw(false);
		}

		void update() override
		{
		}
		void draw2D() override
		{
			setRectAndDraw();
		}
		void setNumber(int num)
		{
			num_ = num;
			font_[3] = 10;				//'%' 
			font_[2] = num % 10;		//一桁目(一の位)
			font_[1] = num % 100 / 10;	//二桁目(十の位)
			font_[0] = num / 100;		//三桁目(百の位)
		}
	};

	class ButtonCommponent final : public ComponentSystem
	{
	private:
		Position * pos_ = nullptr;
		Rectangle* rectangle_ = nullptr;
		SpriteRectDraw* rectDraw_ = nullptr;

		bool button_[3];
		float posX_;
		int rectW_;

		void setRect(SpriteRectDraw* draw)
		{
			for (int i = 0; i < std::size(button_); ++i)
			{
				//選択中のボタンの画像変更：仕様変更により消去
				//if (select_ == i) {
				//	rectangle_->x = rectW_;
				//}
				//else {
				//	rectangle_->x = 0;
				//}

				//隙間をもってボタン三つを描く
				//select_に当たっているボタンはrectが1個右に
				pos_->val.x = posX_ + i * (rectW_ + 30);
				if (draw != nullptr)
				{
					//draw->setPivot(Vec2{ rectW_ / 2, 56 / 2 });
					draw->draw2D();
				}
			}
		}
	public:
		ButtonCommponent(int rect_w)
		{
			rectW_ = rect_w;
		}

		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			rectangle_ = &entity->getComponent<Rectangle>();
			rectDraw_ = &entity->getComponent<SpriteRectDraw>();

			posX_ = pos_->val.x;
			rectangle_->w = rectW_;
			setRect(nullptr);
		}
		void update() override
		{
			setRect(nullptr);
		}
		void draw2D() override
		{
			setRect(rectDraw_);
		}
	};

	class ButtonMojiCommponent  final : public ComponentSystem
	{
	private:
		Position * pos_ = nullptr;
		Rectangle* rectangle_ = nullptr;
		SpriteRectDraw* rectDraw_ = nullptr;

		int moji_[3];
		float posX_;
		int rectW_;

		void setRect(SpriteRectDraw* draw)
		{
			for (int i = 0; i < std::size(moji_); ++i)
			{
				//! 元画像の描画する範囲をずらす
				rectangle_->x = rectW_ * i;
				pos_->val.x = posX_ + rectW_ * i;
				if (draw != nullptr)
				{
					//draw->setPivot(Vec2{ rectW_ / 2, 56 / 2 });
					draw->draw2D();
				}
			}
		}
	public:
		ButtonMojiCommponent(int rectW)
		{
			rectW_ = rectW;
		}
		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			rectangle_ = &entity->getComponent<Rectangle>();
			rectDraw_ = &entity->getComponent<SpriteRectDraw>();

			posX_ = pos_->val.x;
			rectangle_->w = rectW_;
		}
		void update() override
		{
			setRect(nullptr);
		}
		void draw2D() override
		{
			setRect(rectDraw_);
		}

	};

	class SelectFrame final : public ComponentSystem {
	private:
		Position * pos_ = nullptr;
		Rectangle* rectangle_ = nullptr;
		SpriteRectDraw* rectDraw_ = nullptr;

		float posX_;
		int select_;
	public:
		SelectFrame() {}
		void initialize() override {
			pos_ = &entity->getComponent<Position>();
			rectangle_ = &entity->getComponent<Rectangle>();
			rectDraw_ = &entity->getComponent<SpriteRectDraw>();
			posX_ = pos_->val.x;
			select_ = 0;
		}
		void update() override {}
		void draw2D() override {}

		/**
		* @brief 入力によって選択中のボタン識別変数を更新
		* @param num 左なら‐1、右なら1
		*/
		void setSelect(const int num)
		{
			if (num != 1 && num != -1) {
				DOUT << "param is incorrect" << std::endl;
				return;
			}
			//select_が0,1,2の中ひとつがなるように
			select_ = (select_ + num + 3) % 3;
			pos_->val.x = posX_ + ((138/*ボタンのサイズ*/ + 30/*ボタンの間隔*/)* select_);
		}

		int getSelect()
		{
			return select_;
		}

	};
	/*
	*@brief フォント画像を描画します
	SpriteRectDraw、Rectがないとエラーが出ます
	*　rect_w	フォント1個の横幅
	*　num		表示する数字
	*/
	class DrawFont2 final : public ComponentSystem
	{
	private:
		Position * pos_ = nullptr;
		Rectangle* rectangle_ = nullptr;
		SpriteRectDraw* rectDraw_ = nullptr;

		int num_;
		Vec2 rect_;
		Vec2 initPos_;

		int font_[3];

		void setRectAndDraw(const bool isDraw = true)
		{
			int keta = 0;
			rectDraw_->drawEnable();
			for (int i = 0; i < std::size(font_); ++i)
			{
				//十の位、百の位が0の時は各値を表示しない
				if ((i == 1 && num_ < 10) || (i == 0 && num_ < 100))
				{
					++keta;
					continue;
				}
				rectangle_->x = font_[i] * (int)rect_.x;
				pos_->val.x = initPos_.x + (i * rect_.x);
				if (isDraw)
				{
					rectDraw_->draw2D();
				}
			}
			rectDraw_->drawDisable();
		}

	public:
		DrawFont2(float rectW, float rectH, const int num) :
			num_(num)
		{
			rect_.x = rectW;
			rect_.y = rectH;
		}

		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			rectangle_ = &entity->getComponent<Rectangle>();
			rectDraw_ = &entity->getComponent<SpriteRectDraw>();

			initPos_ = pos_->val;
			rectangle_->x = (int)rect_.x * num_;
			rectangle_->w = (int)rect_.x;
			rectDraw_->setPivot(Vec2(rect_.x / 2.f, rect_.y / 2.f));
			setNumber();
		}

		void update() override
		{
		}
		void draw2D() override
		{
			setRectAndDraw();
		}
		void setNumber()
		{
			font_[2] = num_ % 10;		//一桁目(一の位)
			font_[1] = (num_ % 100) / 10;	//二桁目(十の位)
			font_[0] = num_ / 100;		//三桁目(百の位)
		}
		void setNumber(int num)
		{
			num_ = num;
			font_[2] = num % 10;		//一桁目(一の位)
			font_[1] = (num % 100) / 10;	//二桁目(十の位)
			font_[0] = num / 100;		//三桁目(百の位)
		}
	};

	/*
	*@brief マーカーをリズムに合わせてビクンビクンさせる（ExpandComponentSystemが必要）
	*/
	class MarkerBodyController final : public ComponentSystem
	{
	private:
		ExpandComponentSystem* expander;
		BeatByTrigger* beatbt;
		
		int bpm_;
		int beat_;
		std::string soundName_;

	public:
		MarkerBodyController(int bpm, int beat, const std::string& soundName) :
			bpm_(bpm),
			beat_(beat),
			soundName_(soundName) {}

		void initialize() override
		{
			CalcurationBeat beat(bpm_, beat_);
			entity->addComponent<ExpandComponentSystem>(1.2f, 1.f, beat.calcNote_Frame((float)beat_));
			expander = &entity->getComponent<ExpandComponentSystem>();

			entity->addComponent<BeatByTrigger>(bpm_, beat_, (float)beat_, soundName_);
			beatbt = &entity->getComponent<BeatByTrigger>();
		}

		void update() override
		{
			if (beatbt->getTrigger())
			{
				expander->reset();
			}
		}
	};

	//! 指定フレーム後に指定スピードでアルファ値を変更する
	class ChangeAlphaComponent final : public ComponentSystem
	{
	private:
		AlphaBlend * alphaBlend_ = nullptr;
		int elapse_ = 0;
		int fadeSpeed_ = 0;
		Counter counter_;

	public:
		ChangeAlphaComponent(const int elapse, const int fadeSpeed)
			:
			elapse_(elapse),
			fadeSpeed_(fadeSpeed)
		{}
		void initialize() override
		{
			alphaBlend_ = &entity->getComponent<AlphaBlend>();
			counter_.setCounter(0, 1, 0, elapse_);
		}
		void update() override
		{
			counter_.add();
			if (counter_.isMax())
			{
				alphaBlend_->alpha -= fadeSpeed_;
				if (alphaBlend_->alpha <= 0) {
					alphaBlend_->alpha = 0;
					entity->destroy();		//死ぬ
				}
			}
		}
	};
} //namespce ECS

