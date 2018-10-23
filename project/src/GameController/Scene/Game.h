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

namespace Scene
{
	class Game : public AbstractScene
	{
	private:
		Parameter playerDetail;
		ECS::EntityManager* entityManager_;
		MusicScoreLoader msl;
		NotesCreator nc;
		ECS::Entity* bar;
	public:
		Game(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame, ECS::EntityManager* entityManager);
		~Game();
		virtual void update() override;
		virtual void draw() override;
	
	};
}