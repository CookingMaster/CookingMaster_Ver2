/**
* @file StageSelect.hpp
* @brief ステージセレクトシーンでの動作を記述します
* @author
* @date
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"


namespace Scene
{
	class StageSelect : public AbstractScene
	{
	private:
		ECS::EntityManager* entitytManager_;
	public:
		StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override {};
		void update() override;
		void draw() override;
	};

}