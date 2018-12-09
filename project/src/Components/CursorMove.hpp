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
		Transform* trans = nullptr;
		SpriteDraw* sprite = nullptr;
		Vec2 point_{0.f, 0.f};		//指し示す位置(主にメニューの名前が配置されている座標を格納)

	public:
		CursorMove() = default;
		CursorMove(const Vec2& setTargetPos)
		{
			point_ = setTargetPos;
		}
		CursorMove(Entity* setTargetEntity)
		{
			point_ = setTargetEntity->getComponent<ECS::Position>().val;
		}
		void initialize() override
		{
			trans = &entity->getComponent<Transform>();
		}
		void update() override
		{
			trans->setPosition(point_.x - 100, point_.y);

		}
		//!指し示すエンティティをで指定します。
		//!対象のエンティティにPositionがあることが前提です
		void setPoints(Entity* target)
		{
			if (target ==  nullptr)
			{
				DOUT_BY_FUNCTION << FILENAME_AND_LINE << "not found entityes" << std::endl;
				return;
			}

			point_ = target->getComponent<ECS::Position>().val;
		}
	};
}