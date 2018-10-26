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

namespace ECS {
	/*!
	@brief BPMに応じてアニメーションする描画機能です。画像の中心が基準です
	* - SpriteAnimationDrawが必要です。
	*/
	class AnimatorByBPM final : public ComponentSystem
	{
	private:
		SpriteAnimationDraw* animation_ = nullptr;
		float bpm_ = 0;
		DWORD beat_ = 0;
		DWORD start_ = 0;
		std::string soundname_ = "";
		int indexX_ = 0;
		int indexY_ = 0;
		int xmin_ = 0;		//!描画する画像のX方向のインデックスの最小
		int ymin_ = 0;		//!描画する画像のY方向のインデックスの最小
		int maxXnum_ = 0;	//!描画する画像のX方向の枚数
		int maxYnum_ = 0;	//!描画する画像のY方向の枚数
	public:
		//!曲名とBPMを指定して初期化します
		AnimatorByBPM(const char* soundname, const int bpm)
		{
			soundname_ = soundname;
			setBPM(static_cast<float>(bpm));
			calcBeat();
		}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();
			start_ = GetSoundCurrentTime(ResourceManager::GetSound().getHandle(soundname_));
		}
		void update() override
		{
			DWORD now = GetSoundCurrentTime(ResourceManager::GetSound().getHandle(soundname_));
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
			animation_->setIndex(indexX_ * (indexY_ + 1));
		}
		//!BPMを設定する
		void setBPM(const float bpm)
		{
			bpm_ = bpm;
		}
		/**
		* @brief 1拍の長さ(ms)を計算する
		* @note 1000(ms) * 60(sec/min) / bpm(beat/min)
		*/
		void calcBeat()
		{
			beat_ = static_cast<DWORD>(1000 * (60.f / bpm_));
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

	/*!
	@brief 画像の横一列をアニメーションさせる描画機能です。
	* - SpriteAnimationDrawが必要です。
	*/
	class AnimatorByFrame final : public ComponentSystem
	{
	private:
		SpriteAnimationDraw * animation_ = nullptr;
		Counter counter_;
		int frame_ = 0;		//!アニメーションするフレーム数
		int indexX_ = 0;
		int indexY_ = 0;
		int xmin_ = 0;		//!描画する画像のX方向のインデックスの最小
		int ymin_ = 0;		//!描画する画像のY方向のインデックスの最小
		int maxXnum_ = 0;	//!描画する画像のX方向の枚数
		int maxYnum_ = 0;	//!描画する画像のY方向の枚数
		bool isEndStopAnim_;	//!アニメーションが一巡したら停止するか否か
	public:
		//!アニメーションの遷移フレーム数を設定する
		AnimatorByFrame(const int frame)
			:
			frame_(frame),
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
					if (isEndStopAnim_)
					{
						--indexX_;
					}
					else
					{
						indexX_ = xmin_;
					}
				}
			}
			animation_->setIndex(indexX_ + (indexY_ + maxXnum_));
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

		/**
		* @brief 現在のインデックスを指定値に設定する
		* @param indexY 設定するインデックス値
		* -
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
		* -
		*/
		void setIsEndStopAnim(bool isEndStopAnim)
		{
			isEndStopAnim_ = isEndStopAnim;
		}
	};
}