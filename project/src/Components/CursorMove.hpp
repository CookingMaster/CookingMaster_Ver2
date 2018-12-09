/**
* @file CursorMove.hpp
* @brief セレクト画面で使うCursorの機能です
* @author tonarinohito
* @date 2018/12/04
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Renderer.hpp"
#include "../Utility/Vec.hpp"
#include <DxLib.h>

namespace ECS
{
	/*
	@brief カーソルの動きや入力処理です。
	@details Transform,SpriteDrawが必要です
	- カーソルはエンティティの左側に配置されます
	*/
	class CursorMove final :public ComponentSystem
	{
	private:
		bool isOptionSelect_ = false;
		Transform* trans = nullptr;
		SpriteDraw* sprite = nullptr;
		std::vector<Entity*> targetEntity_;
		Vec2 point_{0.f, 0.f};		//指し示す位置(主にメニューの名前が配置されている座標を格納)
		size_t index_ = 0;
		size_t preIndex = 0;
		std::string stageNames[3];	//とりあえず3つ
		void select()
		{
			if (!isOptionSelect_)
			{
				//上下
				if (Input::Get().getKeyFrame(KEY_INPUT_UP) == 1)
				{
					if (index_ > 0) { --index_; }
					else { index_ = 2; }
					preIndex = index_;
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
				{
					if (index_ < 2) { ++index_; }
					else { index_ = 0; }
					preIndex = index_;
				}

				//左右
				if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1)
				{
					if (index_ != 3) { index_ = 3; }
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1)
				{
					if (index_ == 3) { index_ = preIndex; }
				}
				//オプションに移行
				if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1 && index_ == 3)
				{
					isOptionSelect_ = true;
				}
			}
			else
			{
				if (index_ == 3) { index_ = 4; }
				//上下
				if (Input::Get().getKeyFrame(KEY_INPUT_UP) == 1)
				{
					if (index_ > 4) { --index_; }
					else { index_ = 6; }
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
				{
					if (index_ < 6) { ++index_; }
					else { index_ = 4; }
				}
				//ステージ選択に移行
				if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1 && index_ == 6)
				{
					isOptionSelect_ = false;
					index_ = preIndex;
				}
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
			stageNames[0] = "Let'sCooking.wav";
			stageNames[1] = "Let'sCooking.wav";
			stageNames[2] = "Let'sCooking.wav";
			point_ = setTargetEntity.at(0)->getComponent<ECS::Position>().val;
		}
		void initialize() override
		{
			trans = &entity->getComponent<Transform>();
			sprite = &entity->getComponent<SpriteDraw>();
		}
		void update() override
		{
			select();
			trans->setPosition(point_.x - 100, point_.y);
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

		//!決定ボタン(Z)を押すと選択したステージ名を返します
		[[nodiscard]] const std::string getSelectStage() const
		{
			if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
			{
				switch (index_)
				{
				case 0: return stageNames[index_];
				case 1: return stageNames[index_];
				case 2: return stageNames[index_];
				}
			}
			return "";
		}
	};
}