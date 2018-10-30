/**
* @file Pause.hpp
* @brief �|�[�Y�V�[���ł̓�����L�q���܂�
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
	public:
		Pause(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]]const Parameter& parame, ECS::EntityManager* entityManager);
		Pause() = default;
		virtual void update() override;
		virtual void draw() override;
	};

}