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
		ECS::Entity* slide_ = nullptr;
		ECS::Entity* button_ = nullptr;
		ECS::Entity* moji_ = nullptr;
		ECS::Entity* frame_ = nullptr;
		ECS::Entity* bg_ = nullptr;
		ECS::Entity* stage_ = nullptr;
		std::string name_;
		std::string bgmPath_;
		bool isAuto_;
		void selectButton();
		void moveCursor();
		size_t stageNum_ = 0u;

	public:
		Pause(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override {};
		void update() override;
		void draw() override;
		~Pause();
	};

}
