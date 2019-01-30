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
#include "../src/Class/BossController.hpp"
namespace Scene
{
	class Game : public AbstractScene
	{
	private:
		ECS::Entity* fade_ = nullptr;
		bool isPlay_ = false;
		ECS::EntityManager* entityManager_ = nullptr;
		int scoreNum_;
		std::string bgmName_;
		std::string bgmPath_;
		size_t stageNum_;
		std::unique_ptr<BossController> boss_;
		MusicScoreLoader msl_;
		NotesCreator nc_;
		int comb_ = 0;
		int maxComb_ = 0;
		ECS::Entity* scoreFont_ = nullptr;

		//こいつをtrueにするとオートモードになるぞ！
		bool autoPerfectMode_ = true;

	public:
		Game(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		~Game();
		void initialize() override;
		void update() override;
		void draw() override;

	private:
		int getNoteScore();
		void changePauseScene();
		void changeResultScene();
		void comboReset();
		void saveMaxComb();
		void createRankFont(int rank);
	};
}