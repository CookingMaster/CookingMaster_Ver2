/**
 * @file GameMain.hpp
 * @brief Dxlibの更新とアプリケーションの更新を行います
 * @author tonarinohito
 * @date 2018/10/05
 */
#pragma once
#include "../System/System.hpp"
#include "../GameController/GameController.h"
#include "../Utility/FPS.hpp"
#include <memory>

//!アプリケーションを生成します
class GameMain final
{
private:
	std::unique_ptr<System> system;
	std::unique_ptr<GameController> game;
	void update()
	{
		game->update();
	}
	void draw()
	{
		game->draw();
	}

public:
	GameMain()
	{
		system = std::make_unique<System>();
		game = std::make_unique<GameController>();
	}
	//!アプリケーションの更新を行います
	void run()
	{
		while (system->isOk())
		{
			Fps::Get().update();
			update();
			draw();
			Fps::Get().wait();
		}
	}
};