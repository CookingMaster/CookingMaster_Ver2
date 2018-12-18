/**
* @file  Animator.hpp
* @brief アニメーション関連のコンポーネントです
* @note  BPM、またはフレームに応じたアニメーションをする
* @author yokota0717
* @date 2018/10/16
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Class/ResourceManager.hpp"
#include "../System/System.hpp"
#include "Renderer.hpp"
#include "../Utility/Counter.hpp"
#include "../Utility/CalcurationBeat.hpp"

namespace ECS {
	/*!
	@brief 画像の横一列をアニメーションさせる描画機能です。
	* - SpriteAnimationDrawが必要です。
	*/
	class Animator final : public ComponentSystem
	{
	private:
		SpriteAnimationDraw * animation_ = nullptr;
		Counter counter_;
		int frame_ = 0;		//!アニメーションするフレーム数
		int indexX_ = 0;
		int indexY_ = 0;
		int xmin_ = 0;			//!描画する画像のX方向のインデックスの最小
		int ymin_ = 0;			//!描画する画像のY方向のインデックスの最小
		int maxXnum_ = 0;		//!描画する画像のX方向の枚数
		int maxYnum_ = 0;		//!描画する画像のY方向の枚数
		int maxImgXNum_ = 0;	//!使用する画像のX方向の最大インデックス数
		int maxImgYNum_ = 0;	//!使用する画像のY方向の最大インデックス数
		bool isEndStopAnim_;	//!アニメーションが一巡したら停止するか否か
	public:
		//!アニメーションの遷移フレーム数を設定する
		Animator(const int frame, const int maxX, const int maxY)
			:
			frame_(frame),
			maxImgXNum_(maxX),
			maxImgYNum_(maxY),
			isEndStopAnim_(false)
		{}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();
			counter_.SetCounter(0, 1, 0, frame_);
		}
		void update() override
		{
			if (++counter_ >= frame_)
			{
				++indexX_;
				if (indexX_ >= maxXnum_)
				{
					++indexY_;
					if (indexY_ >= maxYnum_)
					{
						if (isEndStopAnim_)
						{
							indexX_ = maxXnum_ - 1;
							indexY_ = maxYnum_ - 1;
						}
						else
						{
							indexX_ = xmin_;
							indexY_ = ymin_;
						}
					}
					else
					{
						indexX_ = xmin_;
					}
				}
			}
			animation_->setIndex(indexX_ + (indexY_ * maxImgXNum_));
		}
		/**
		* @brief アニメーションのインデックス情報を設定する
		* @param minX Xのインデックスの最小
		* @param minY Yのインデックスの最小
		* @param maxX Xの枚数
		* @param maxY Yの枚数
		* @note 使用例：Yの0番目、Xの1~5番目でアニメーションする
		* -
		*/
		void setSpriteNum(const int xmin, const int ymin, const int xnum, const int ynum)
		{
			xmin_ = xmin;
			ymin_ = ymin;
			indexX_ = xmin;
			indexY_ = ymin;
			maxXnum_ = xnum;
			maxYnum_ = ynum;
			counter_.reset();
		}

		/**
		* @brief 現在のインデックスを指定値に設定する
		* @param indexX,indexY 設定するインデックス値
		*/
		void setIndex(int indexX, int indexY)
		{
			indexX_ = indexX;
			indexY_ = indexY;
			counter_.reset();
		}

		/**
		* @brief アニメーションが一巡したら停止するか否かを設定する
		* @param isEndStopAnim アニメーションを停止するか否か
		*/
		void setIsEndStopAnim(bool isEndStopAnim)
		{
			isEndStopAnim_ = isEndStopAnim;
		}

		/**
		* @brief アニメーション速度を変更する
		* @param frame アニメーション速度
		*/
		void changeFrame(int frame)
		{
			frame_ = frame;
			counter_.SetCounter(0, 1, 0, frame);
		}
	};

	/*!
	@brief BPMに応じてアニメーションする描画機能です。画像の中心が基準です
	* - SpriteAnimationDrawが必要です。
	*/
	class AnimatorByBPM final : public ComponentSystem
	{
	private:
		SpriteAnimationDraw* animation_ = nullptr;
		int bpm_ = 0;
		TCounter<int> cntTime_;	//時間計測
		TCounter<int> cntBar_;	//小節の計測
		int indexX_ = 0;
		int indexY_ = 0;
		int xmin_ = 0;		//!描画する画像のX方向のインデックスの最小
		int ymin_ = 0;		//!描画する画像のY方向のインデックスの最小
		int maxXnum_ = 0;	//!描画する画像のX方向の枚数
		int maxYnum_ = 0;	//!描画する画像のY方向の枚数
	public:
		//!曲名とBPMを指定して初期化します
		AnimatorByBPM(const int bpm) :
			bpm_(bpm)
		{

		}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();
		}
		void update() override
		{
			CalcurationBeat beat((float)bpm_);

			//一小節毎にノーツを生成する
			if (cntTime_.getCurrentCount() % int(beat.calcOneBar_Frame()) == 0)
			{

			}
			++cntTime_;
/*
			if (now - start_ >= beat_)
			{
				++indexX_;
				if (indexX_ > maxXnum_)
				{
					indexX_ = xmin_;
					if (maxYnum_ > 1)
					{
						++indexY_;
						if (indexY_ > maxYnum_)
						{
							indexY_ = ymin_;
						}
					}
				}
				start_ = now;
			}
			animation_->setIndex(indexX_ * (indexY_ + 1));*/
		}
		/**
		* @brief アニメーションのインデックス情報を設定する
		* @param minX Xのインデックスの最小
		* @param minY Yのインデックスの最小
		* @param maxX Xの枚数
		* @param maxY Yの枚数
		* @note 使用例：Yの0番目、Xの1~5番目でアニメーションする
		* -
		*/
		void setSpriteNum(const int xmin, const int ymin, const int xnum, const int ynum)
		{
			xmin_ = xmin;
			ymin_ = ymin;
			indexX_ = xmin;
			indexY_ = ymin;
			maxXnum_ = xnum;
			maxYnum_ = ynum;
		}
	};

}