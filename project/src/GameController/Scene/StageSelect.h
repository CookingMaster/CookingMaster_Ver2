/**
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
	class StageSelect final : public AbstractScene
	{
	private:
		ECS::Entity* option_ = nullptr;
		ECS::Entity* cursor_ = nullptr;
		ECS::Entity* bgmSlider_ = nullptr;
		ECS::Entity* seSlider_ = nullptr;
		ECS::Entity* bgmFullSlider_ = nullptr;
		ECS::Entity* seFullSlider_ = nullptr;
		ECS::Entity* bgmBar_ = nullptr;
		ECS::Entity* seBar_ = nullptr;
		ECS::Entity* star_[3]{};
		ECS::Entity* dish_[3]{};
		ECS::Entity* score_{};
		ECS::Entity* effects_[4]{};
		ECS::Entity* fade_ = nullptr;
		ECS::EntityManager* entityManager_ = nullptr;
		std::vector<ECS::Entity*> cursorTargets{};
		Counter cnt_;
		float bgmVal_ = 0;
		float seVal_ = 0;
		int backVal_ = 0;
		std::string bgmPath_;
		bool isPlay_ = false;
		bool isAuto_ = false;
		size_t stageNum_ = 0u;
		void entitySetUp();
		void optionSheetMove();
		void setSoundVolume();
		void changeLayer();
		void showDishParameter();
	public:
		StageSelect(IOnSceneChangeCallback* sceneChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		~StageSelect();
		void initialize() override;
		void update() override;
		void draw() override;
	};

}