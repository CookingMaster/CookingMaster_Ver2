/**
* @file Title.hpp
* @brief タイトルシーンでの動作を記述します
* @author
* @date
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"


namespace Scene
{
	class Title final : public AbstractScene
	{
	private:
		ECS::EntityManager* entityManager_;
		int progress;
		float gain_;
	public:
		Title(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		~Title();
		void initialize() override;
		void update() override;
		void draw() override;

	private:
		void BehaviorForProgress();
	};

}
