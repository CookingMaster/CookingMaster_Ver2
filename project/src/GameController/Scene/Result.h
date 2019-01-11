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
		//! �萔
		const int SCORE_GREAT = 80;
		const int SCORE_GOOD = 50;
		//! �G���e�B�e�B�}�l�[�W���[
		ECS::EntityManager* entityManager_ = nullptr;
		//! �t�F�[�h
		ECS::Entity* fade_ = nullptr;
		//! �w�i
		ECS::Entity* back_ = nullptr;
		//! �N���b�V��
		ECS::Entity* cloche_ = nullptr;
		//! ����
		ECS::Entity* dish_ = nullptr;
		//! ������
		std::vector<ECS::Entity*> confetties_;
		//! �]���t�H���g
		ECS::Entity* evaluation_ = nullptr;
		//! �X�R�A
		int score_;
		//! �o�߃t���[���J�E���^
		Counter counter_;
		//! �X�e�[�W���
		std::string bgmName_;
		ECS::StageHighScore stage_;
		//! �t�F�[�h�A�E�g�t���O
		bool isFadeOut_;



		//! �X�R�A���痿���̉摜��ݒ肷��
		Vec2 setDishImg();
		//! �X�R�A���擾����
		void setStage();
	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override;
		void update() override;
		void draw() override;
		~Result();
	};

}