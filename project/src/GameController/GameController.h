/**
 * @file GameController.hpp
 * @brief �A�v���P�[�V�����̏������s���܂�
 * @author tonarinohito
 * @date 2018/10/06
 */
#pragma once
#include "../ECS/ECS.hpp"
#define ENTITY_GROUP (ECS::Group)GameController::GameGroup
class GameController final
{
private:
	ECS::EntityManager entityManager_;
	void resourceLoad();
public:

	/**
	*  @brief �G���e�B�e�B������̃O���[�v�ł�
	* - �`�惌�C���[�A�������A�G���e�B�e�B�̎擾�Ɏg���܂�
	* - OrderByDraw�g�p���A�ԍ����傫���قǎ�O�ɕ`�悳��܂�
	*/
	enum class GameGroup : ECS::Group
	{
		LAYER1,			//�e�X�g�p
		BACK,			//�Ŕw�ʔw�i
		BACK_OBJECT,	//�w�i�̓��M�Ȃǂ̒������
		BACK_STAFF,		//���œ����]�ƈ�
		MASTER,			//���������
		KITCHENWARE,	//������ɏ�ɍڂ����I�u�W�F�N�g
		COOKING_AREA,	//������⏰
		GIRL,			//���̎q
		RECEIVE_STAFF,	//�H�ނ��󂯎��X�^�b�t
		NOTE,			//����(�H��)
		EFFECT,			//���o�p�G�t�F�N�g
		UI,				//���v��X�R�A�Q�[�W����UI
		FADE,			//�t�F�[�h�p
		PAUSE_UI,		//�|�[�Y�pUI
		TOP_FADE,		//�őO�ʃt�F�[�h
		MAX,			//�ő吔
	};
	GameController();
	//!Entity�����Event�̍X�V�������s���܂�
	void update();
	//!Entity�̕`����s���܂�
	void draw();
};