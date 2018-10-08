/**
* @file Title.hpp
* @brief タイトルシーンでの動作を記述します
* @author 
* @date 
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "Scene.hpp"

namespace Scene
{
	class Title final : public IScene
	{
	private:
		ECS::EntityManager& entityManager_;
	public:
		Title(ECS::EntityManager& manager);
		~Title() = default;
		void update() override;
		void draw() override;
		void release() override;
	};
}