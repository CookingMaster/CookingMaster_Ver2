/**
* @file Result.h
* @brief ���U���g�V�[��
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
		//! �w�i
		ECS::Entity* back_;
		//! �N���b�V��
		ECS::Entity* cloche_;
		//! ����
		ECS::Entity* dish_;
		//! �X�R�A
		int score_;
		//! �o�߃t���[���J�E���^
		Counter counter_;

	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override;
		void update() override;
		void draw() override;
		~Result();
	};

}