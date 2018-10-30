﻿/**
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
		Title(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override {};
		void update() override;
		void draw() override;
	};

}