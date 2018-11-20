/**
* @file Pause.hpp
* @brief ポーズシーンでの動作を記述します
* @author
* @date
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"


namespace Scene
{
	class Pause : public AbstractScene
	{
	private:
		ECS::EntityManager* entityManager_;
		ECS::Entity* slide;
		ECS::Entity* button;
		ECS::Entity* moji;
		ECS::Entity* frame;
		ECS::Entity* bg;
		std::string name;
	public:
		Pause(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override {};
		void update() override;
		void draw() override;
		~Pause();
	};

}