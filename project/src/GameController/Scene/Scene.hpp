/**
* @file Scene.hpp
* @brief Sceneの基底クラスです
* @author tonarinohito
* @date 2018/10/06
*/
#pragma once
namespace Scene
{
	//!Sceneの基底クラスです
	class IScene
	{
	public:
		virtual ~IScene() = default;
		//!更新処理を行います
		virtual void update() = 0;
		//!描画処理を行います
		virtual void draw() = 0;
		//!リソースの開放を行います
		virtual void release() = 0;
	};
}

