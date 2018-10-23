/**
 * @file GameController.hpp
 * @brief �A�v���P�[�V�����̏������s���܂�
 * @author tonarinohito
 * @date 2018/10/06
 * @par History
 - 2018/10/14 tonarinohito
 -# ���̃N���X�ŃV�[���̃X�^�b�N���Ď�����悤�ɕύX
 */
#pragma once
#include "../ECS/ECS.hpp"
#include "Scene/SceneManager.hpp"
#include "Scene/Parameter.hpp"
#include <stack>
#define ENTITY_GROUP (ECS::Group)GameController::GameGroup

class GameController final : public Scene::IOnSceneChangeCallback
{
private:
	ECS::EntityManager entityManager_;
	std::stack<std::unique_ptr<Scene::AbstractScene>> sceneStack;	//�V�[���̃X�^�b�N
	Parameter param;
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
	~GameController() = default;
	/*!
	* @brief �V�[���ύX(�e�V�[������R�[���o�b�N�����)
	* @param parame ���̃V�[���ɓn�������l�B�s�v�Ȃ�nullptr���w�肵�܂�
	* @param scene �ύX����V�[����enum
	* @param stackClear ���݂̃V�[���̃X�^�b�N���N���A���邩
	*/
	void onSceneChange(const Scene::SceneName& scene, const Parameter* parame, const Scene::StackPopFlag stackClear) override;
	//!���ׂẴV�[���X�^�b�N���N���A���܂�
	void stackClear() override;
	//!Entity�̍X�V�������s���܂�
	void update();
	//!Entity�̕`����s���܂�
	void draw();
};