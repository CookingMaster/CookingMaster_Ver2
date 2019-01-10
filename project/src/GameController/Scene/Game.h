/**
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
		ECS::EntityManager* entityManager_ = nullptr;
		int num_;
		std::string name_;
		std::unique_ptr<BossController> boss_;
		MusicScoreLoader msl_;
		NotesCreator nc_;
		int comb_ = 0;
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
		void ComboReset();
	};
}