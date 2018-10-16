/**
* @file Title.hpp
* @brief タイトルシーンでの動作を記述します
* @author 
* @date 
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"


namespace Scene
{
	class Title : public AbstractScene
	{
	private:
		ECS::EntityManager* entitytManager_;
	public:
		Title(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]]const Parameter& parame, ECS::EntityManager* entityManager);
		virtual void update() override;
		virtual void draw() override;
	};

}