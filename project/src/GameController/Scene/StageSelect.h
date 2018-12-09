﻿/**
* @file StageSelect.hpp
* @brief ステージセレクトシーンでの動作を記述します
* @author tonari
* @date 2018/11/15
*/
#pragma once
#include "../src/Utility/Counter.hpp"
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"
#include "../src/Components/BasicComponents.hpp"
#include "../src/Components/BasicComponents.hpp"
namespace Scene
{
	class StageSelect : public AbstractScene
	{
	private:
		const Vec2 OPTION_POSITION{ 400.f,55.f };
		ECS::Entity* option_ = nullptr;
		ECS::Entity* cursor_ = nullptr;
		ECS::EntityManager* entityManager_ = nullptr;
		std::vector<ECS::Entity*> cursorTargets{};
		Counter cnt_;
		int backVal_ = 0;
		int score_ = 0;
		void entitySetUp();
	public:
		StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		~StageSelect();
		void initialize() override;
		void update() override;
		void draw() override;
	};

}