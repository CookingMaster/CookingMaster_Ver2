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
#include "../src/Components/BasicComponents.hpp"

namespace Scene
{
	class StageSelect : public AbstractScene
	{
	private:
		static constexpr int UI_HEIGHT = 120;
		ECS::EntityManager* entityManager_ = nullptr;
		ECS::Entity* point_ = nullptr;
		std::vector<ECS::Entity*> UIMap_{};
		//選択用アイコンの移動処理に必要なデータ
		class Point
		{
		public:
			ECS::Position* pos = nullptr;
			size_t selectNum = 0u;
			void selectStageMove(ECS::Entity* pEntity, std::vector<ECS::Entity*> uiMap);
		}pointEntityMove;
		void selectStage();
		int score_ = 0;
		
	public:
		StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		~StageSelect();
		void initialize() override;
		void update() override;
		void draw() override;
	};

}