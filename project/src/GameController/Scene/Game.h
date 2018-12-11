﻿/**
* @file Game.hpp
* @brief ゲームシーンでの動作を記述します
* @author 
* @date 
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"
#include "../../Class/MusicScoreLoader.hpp"
#include "../../Class/NotesCreator.hpp"
#include "../src/ArcheType/UIArcheType.hpp"
namespace Scene
{
	class Game : public AbstractScene
	{
	private:
		ECS::EntityManager* entityManager_ = nullptr;
		MusicScoreLoader msl_;
		NotesCreator nc_;
		int num_;
		std::string name_;
	public:
		Game(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		~Game();
		void initialize() override;
		void update() override;
		void draw() override;
	
	private:
		int getNoteScore();
		void changePauseScene();
		void cangeResultScene();
	};
}