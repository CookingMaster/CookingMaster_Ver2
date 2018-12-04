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
	@brief カーソルの動きです
	@details Transform,SpriteDrawが必要です
	*/
	class CursorMove final :public ComponentSystem
	{
	private:
		Transform* trans;
		SpriteDraw* sprite;
	public:
		CursorMove()
		{

		}
		void initialize() override
		{
			entity->addComponent<Transform>();
			entity->addComponent<SpriteDraw>();
		}
		void update() override
		{

		}

	};
}