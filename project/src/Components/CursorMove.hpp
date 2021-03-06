﻿/**
* @file CursorMove.hpp
* @brief セレクト画面で使うCursorの機能です
* @author tonarinohito
* @date 2018/12/04
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Renderer.hpp"
#include "../Utility/Vec.hpp"
#include "../Class/Sound.hpp"
#include "musicName.hpp"
#include <DxLib.h>

namespace ECS
{
	/*
	@brief カーソルの動きや入力処理です。
	@details Transform,AlphaBlendが必要です
	- カーソルはエンティティの左側に配置されます
	*/
	class CursorMove final :public ComponentSystem
	{
	private:
		enum CursorPosition : size_t
		{
			STAGE1,
			STAGE2,
			STAGE3,
			OPTION,
			BGM_SLIDER,
			SE_SLIDER,
			BACK,
			MAX
		};
		bool isOptionSelect_ = false;
		bool isSliderSelect_ = false;
		Transform* trans_ = nullptr;
		AlphaBlend* blend_ = nullptr;
		std::vector<Entity*> targetEntity_{};
		Vec2 point_{0.f, 0.f};		//指し示す位置(主にメニューの名前が配置されている座標を格納)
		size_t index_ = 0;
		size_t preIndex_ = 0;
		std::string stageNames_[3]{};	//とりあえず3つ

		void selectOption()
		{
			//カーソルの動き
			if (!isSliderSelect_)
			{
				if (index_ == OPTION) { index_ = BGM_SLIDER; }
				//上下
				if (Input::Get().getKeyFrame(KEY_INPUT_UP) == 1)
				{
					Sound se("move");
					se.play(false,true);
					if (index_ > BGM_SLIDER) { --index_; }
					else { index_ = BACK; }
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
				{
					Sound se("move");
					se.play(false, true);
					if (index_ < BACK) { ++index_; }
					else { index_ = BGM_SLIDER; }
				}
				//ステージ選択に移行
				if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1 && index_ == BACK)
				{
					Sound se("turn");
					se.play(false, true);
					isOptionSelect_ = false;
					index_ = OPTION;
				}
				if (!isSliderSelect_ && (index_ == BGM_SLIDER || index_ == SE_SLIDER))
				{
					if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
					{
						Sound se("volume");
						se.play(false, true);
						isSliderSelect_ = true;
						blend_->alpha = 100;
					}
				}
			}
			//スライダーの選択
			else
			{
				if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
				{
					Sound se("volume");
					se.play(false, true);
					isSliderSelect_ = false;
					blend_->alpha = 255;
				}
			}
		}
		void selectStage()
		{
			//上下
			if (index_ != OPTION && Input::Get().getKeyFrame(KEY_INPUT_UP) == 1)
			{
				Sound se("move");
				se.play(false, true);
				if (index_ > STAGE1) { --index_; }
				else { index_ = STAGE3; }
				preIndex_ = index_;
			}
			if (index_ != OPTION && Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
			{
				Sound se("move");
				se.play(false, true);
				if (index_ < STAGE3) { ++index_; }
				else { index_ = STAGE1; }
				preIndex_ = index_;
			}

			//左右
			if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1 && index_ != OPTION)
			{
				Sound se("turn");
				se.play(false, true);
				if (index_ != OPTION) { index_ = OPTION; }
			}
			if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1 && index_ == OPTION)
			{
				Sound se("move");
				se.play(false, true);
				if (index_ == OPTION) { index_ = preIndex_; }
			}
			//オプションに移行
			if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1 && index_ == OPTION)
			{
				Sound se("turn");
				se.play(false,true);
				isOptionSelect_ = true;
			}
		}
		void select()
		{
			if (!isOptionSelect_)
			{
				selectStage();
			}
			else
			{
				selectOption();
			}
			
			point_ = targetEntity_.at(index_)->getComponent<ECS::Position>().val;
		}
	public:
		CursorMove(std::vector<Entity*> setTargetEntity)
			:targetEntity_(setTargetEntity)
		{
			if (setTargetEntity.empty())
			{
				DOUT_BY_FUNCTION << FILENAME_AND_LINE << "not found entityes" << std::endl;
				return;
			}
			point_ = setTargetEntity.at(0)->getComponent<ECS::Position>().val;
		}
		void initialize() override
		{
			trans_ = &entity->getComponent<Transform>();
			blend_ = &entity->getComponent<AlphaBlend>();
		}
		void update() override
		{
			//曲名のセット
			{
				stageNames_[0] = targetEntity_.at(0)->getComponent<ECS::MusicName>().val;
				stageNames_[1] = targetEntity_.at(1)->getComponent<ECS::MusicName>().val;
				stageNames_[2] = targetEntity_.at(2)->getComponent<ECS::MusicName>().val;
			}
			select();
			trans_->setPosition(point_.x - 100, point_.y);
		}
		//!指し示しているindex(要素番号)を取得します
		[[nodiscard]] const size_t getIndex() const 
		{
			return index_;
		}

		//!オプションモード中かboolで返します
		[[nodiscard]] const bool isOptionSelected() const
		{
			return isOptionSelect_;
		}

		//!スライダー選択モード中かboolで返します
		[[nodiscard]] const bool isSiliderSelect() const
		{
			return isSliderSelect_;
		}

		//!決定ボタン(Z)を押すと選択したステージ名を返します
		[[nodiscard]] const std::string getSelectStage() const
		{
			if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1 && index_ < 3)
			{
				return stageNames_[index_];
			}
			else if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1 && index_ < 3)
			{
				return stageNames_[index_] + "|AUTO";
			}
			return "";
		}
		//!決定ボタン(ZまたはX)を押すと選択したステージ番号を返します
		[[nodiscard]] const size_t getStageNumber() const
		{
			if ((Input::Get().getKeyFrame(KEY_INPUT_Z) == 1 || Input::Get().getKeyFrame(KEY_INPUT_X) == 1) && index_ < 3)
			{
				switch (index_)
				{
				case STAGE1: return 1; break;
				case STAGE2: return 2; break;
				case STAGE3: return 3; break;
				}
			}
			return MAX;
		}
	};
}
