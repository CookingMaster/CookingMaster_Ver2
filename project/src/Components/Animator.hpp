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

	struct AnimationData final
	{
		int minXnum_ = 0;	//!描画する画像のX方向のインデックスの最小値
		int minYnum_ = 0;	//!描画する画像のY方向のインデックスの最小値
		int maxXnum_ = 0;	//!描画する画像のX方向のインデックスの最大値
		int maxYnum_ = 0;	//!描画する画像のY方向のインデックスの最大値
		int speed_ = 1;		//!アニメーション速度
	};

	/*!
	@brief 画像をアニメーションさせる描画機能です。
	* - SpriteAnimationDrawが必要です。
	*/
	class Animator final : public ComponentSystem
	{
	private:
		SpriteAnimationDraw * animation_ = nullptr;
		AnimationData animData_;
		Counter counter_;
		int indexX_ = 0;		//!描画する画像のX方向のインデックス
		int indexY_ = 0;		//!描画する画像のY方向のインデックス
		int maxImgXNum_ = 0;	//!使用する画像のX方向の最大インデックス数
		int maxImgYNum_ = 0;	//!使用する画像のY方向の最大インデックス数
		bool isEndStopAnim_ = false;	//!アニメーションが一巡したら停止するか否か
		bool isMinusAnim_ = false;		//!アニメーションを逆順で行うか否か
		bool isAnimEnd_ = false;		//!アニメーションが終了したか否か

	public:
		//!アニメーションの遷移フレーム数を設定する
		Animator(const int animspd, const int maxX, const int maxY)
			:
			maxImgXNum_(maxX),
			maxImgYNum_(maxY)
		{
			animData_.speed_ = animspd;
		}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();

			counter_.SetCounter(0, 1, 0, animData_.speed_);
		}
		void update() override
		{
			if (++counter_ >= animData_.speed_)
			{
				//画像のX方向を次に進め、その値がXの最大値を超えていた場合Yを進める
				bool isNextImg = false;
				if (!isMinusAnim_)	{ ++indexX_; isNextImg = indexX_ >= animData_.maxXnum_; }
				else				{ --indexX_; isNextImg = indexX_ <= animData_.minXnum_; }

				if (isNextImg)
				{
					//画像のY方向を次に進め、その値がYの最大値を超えていた場合最初に戻す(または停止する)
					if (!isMinusAnim_)	{ ++indexY_; isNextImg = indexY_ >= animData_.maxYnum_; }
					else				{ --indexY_; isNextImg = indexY_ <= animData_.minYnum_; }

					if (isNextImg)
					{
						if ((!isMinusAnim_	&& isEndStopAnim_) ||
							(isMinusAnim_	&& !isEndStopAnim_))
						{
							//停止(アニメ遷移が逆順なら初期化)
							indexX_ = animData_.maxXnum_ - 1;
							indexY_ = animData_.maxYnum_ - 1;
							if (!isMinusAnim_)
							{
								isAnimEnd_ = true;
							}
						}
						else
						{
							//初期化(アニメ遷移が逆順なら停止)
							indexX_ = animData_.minXnum_;
							indexY_ = animData_.minYnum_;
							if (isMinusAnim_)
							{
								isAnimEnd_ = true;
							}
						}
					}
					else
					{
						if (!isMinusAnim_)
						{
							indexX_ = animData_.minXnum_;
						}
						else
						{
							indexX_ = animData_.maxXnum_;
						}
					}
				}
			}
			animation_->setIndex(indexX_ + (indexY_ * maxImgXNum_));
		}
		/**
		* @brief アニメーションのインデックス情報を設定する
		* @param minX Xのインデックスの最小
		* @param minY Yのインデックスの最小
		* @param maxX Xのインデックスの最大
		* @param maxY Yのインデックスの最大
		* @note 使用例：Yの0番目、Xの1~5番目でアニメーションする
		* -
		*/
		void setSpriteNum(const int xmin, const int ymin, const int xmax, const int ymax)
		{
			if (!isMinusAnim_)
			{
				indexX_ = xmin;
				indexY_ = ymin;
			}
			else
			{
				indexX_ = xmax;
				indexY_ = ymax;
			}
			isAnimEnd_ = false;

			animData_.minXnum_ = xmin;
			animData_.minYnum_ = ymin;
			animData_.maxXnum_ = xmax;
			animData_.maxYnum_ = ymax;
			counterReset();
		}

		/**
		* @brief 現在のインデックスを指定値に設定する
		* @param indexX,indexY 設定するインデックス値
		*/
		void setIndex(const int indexX, const int indexY)
		{
			indexX_ = indexX;
			indexY_ = indexY;
			isAnimEnd_ = false;
			counterReset();
		}

		/**
		* @brief アニメーションが一巡したら停止するか否かを設定する
		* @param isEndStopAnim アニメーションを停止するか否か
		*/
		void setIsEndStopAnim(const bool isEndStopAnim)
		{
			isEndStopAnim_ = isEndStopAnim;
		}

		/**
		* @brief アニメーションを逆順で行うか否かを設定する
		* @param isMinusAnim アニメーションを逆順で行うか否か
		*/
		void setIsMinusAnim(const bool isMinusAnim)
		{
			isMinusAnim_ = isMinusAnim;
		}

		/**
		* @brief アニメーション速度を変更する
		* @param animspd アニメーション速度
		*/
		void changeFrame(const int animspd)
		{
			animData_.speed_ = animspd;
			counter_.SetCounter(0, 1, 0, animspd);
		}

		/**
		* @brief カウンターをリセットする
		*/
		void counterReset()
		{
			counter_.reset();
		}

		/**
		* @brief アニメーションの終了フラグを取得する
		*/
		bool isAnimEnd()
		{
			if (isEndStopAnim_)
			{
				return false;
			}
			return isAnimEnd_;
		}
	};


	/*!
	@brief 使用するアニメーションを保存し、数値を指定するだけでそのアニメーションに設定されるようにする
	* - Animatorが必要です。
	*/
	class RecordAnimation final : public ComponentSystem
	{
	private:
		Animator* animator_ = nullptr;
		std::vector<std::unique_ptr<AnimationData>> recordData_;
	public:
		RecordAnimation() = default;

		void initialize() override
		{
			animator_ = &entity->getComponent<Animator>();
		}

		/**
		* @brief アニメーションを追加する
		* @param xmin Xのインデックスの最小
		* @param ymin Yのインデックスの最小
		* @param xmax Xのインデックスの最大
		* @param ymax Yのインデックスの最大
		* @return 登録番号
		* @note 追加された順に0,1,2…という風に番号が登録される
		*/
		int addAnimData(const int xmin, const int ymin, const int xmax, const int ymax, const int animspd)
		{
			recordData_.emplace_back();
			recordData_.back() = std::make_unique<AnimationData>();
			recordData_.back()->minXnum_ = xmin;
			recordData_.back()->minYnum_ = ymin;
			recordData_.back()->maxXnum_ = xmax;
			recordData_.back()->maxYnum_ = ymax;
			recordData_.back()->speed_ = animspd;
			return int(recordData_.size()) - 1;
		}

		/**
		* @brief アニメーションを設定する
		* @param setNum 登録番号
		*/
		void setAnimData(const int setNum)
		{
			animator_->setSpriteNum(
				recordData_[setNum]->minXnum_,
				recordData_[setNum]->minYnum_,
				recordData_[setNum]->maxXnum_,
				recordData_[setNum]->maxYnum_
			);
			animator_->changeFrame(recordData_[setNum]->speed_);
		}
	};
}