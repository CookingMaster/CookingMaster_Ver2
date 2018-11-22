/**
* @file Result.h
* @brief リザルトシーン
* @author yokota0717
* @date 2018.11.22
*/

#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"
#include "../../Utility/Counter.hpp"

namespace Scene
{
	class Result : public AbstractScene
	{
	private:
		ECS::EntityManager* entityManager_;
		//! 背景
		ECS::Entity* back_;
		//! クロッシュ
		ECS::Entity* cloche_;
		//! 料理
		ECS::Entity* dish_;
		//! スコア
		int score_;
		//! 経過フレームカウンタ
		Counter counter_;

	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override;
		void update() override;
		void draw() override;
		~Result();
	};

}