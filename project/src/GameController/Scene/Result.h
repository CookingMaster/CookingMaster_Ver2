/**
* @file Result.h
* @brief ���U���g�V�[��
* @author yokota0717
* @date 2018.11.22
* @param history
#- 2018.11.27 yokota0717
- �N���b�V���Ɨ����ǉ�
- ������ǉ�
#- 2018.11.29 yokota0717
- �X�R�A�ǂݍ��ݎb���
- �����_�ł̓n�C�X�R�A��ǂݍ��݁A��������Ƃɗ����̉摜��ς��Ă���
*/

#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"
#include "../../Utility/Counter.hpp"
#include "../../ArcheType/ScoreArcheType.hpp"


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
		//! ������
		std::vector<ECS::Entity*> confetties_;
		//! �X�R�A
		int score_;
		//! �o�߃t���[���J�E���^
		Counter counter_;
		//! �X�e�[�W���
		std::string bgmName_;

		//! �X�R�A���痿���̉摜��ݒ肷��
		float setDish();
		//! �X�R�A���擾����
		void setScore();
	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override;
		void update() override;
		void draw() override;
		~Result();
	};

}