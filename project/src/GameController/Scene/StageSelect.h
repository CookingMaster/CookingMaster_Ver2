/**
* @file StageSelect.hpp
* @brief ステージセレクトシーンでの動作を記述します
* @author tonari
* @date 2018/11/15
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
		static constexpr int UI_HEIGHT = 120;
		ECS::EntityManager* entityManager_ = nullptr;
		ECS::Entity* point = nullptr;
		ECS::Entity* book = nullptr;
		std::unordered_map<std::string, ECS::Entity*> stageMap;
		int score = 0;
		void selectStageMove(ECS::Entity* pEntity);
	public:
		StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		~StageSelect();
		void initialize() override;
		void update() override;
		void draw() override;
	};

}