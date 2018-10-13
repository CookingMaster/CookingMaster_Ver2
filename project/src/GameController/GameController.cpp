#include "GameController.h"
#include "../Class/ResourceManager.hpp"
#include "Scene/SceneManager.hpp"
#include "../Input/Input.hpp"
#include "Scene/Title.h"
#include "Scene/Game.h"
void GameController::resourceLoad()
{
	
}

GameController::GameController()
{
	//�ŏ��ɕK�v�ȃ��\�[�X��Entity�̐����A���[�h���s��
	resourceLoad();
	//�����V�[���̐ݒ�
	sceneStack.push(std::make_unique<Scene::Title >(this, param, &entityManager_));	//�^�C�g���V�[�����쐬���A�v�b�V��
}

void GameController::onSceneChange(const Scene::SceneName& scene, const Parameter* parame, bool isStackClear)
{
	if (isStackClear || scene == Scene::SceneName::BACK_TO_SCENE)
	{
		sceneStack.pop();
		DOUT << "poped the scene stack" << std::endl;
	}
	switch (scene)
	{
	case Scene::SceneName::TITLE:
		sceneStack.push(std::make_unique<Scene::Title>(this, *parame, &entityManager_));
		break;
	case Scene::SceneName::GAME:
		sceneStack.push(std::make_unique<Scene::Game>(this, *parame, &entityManager_));
		break;
	default:
		break;
	}
}

void GameController::stackClear()
{
	while (!sceneStack.empty())
	{
		sceneStack.pop();
	}
}

void GameController::update()
{
	entityManager_.refresh();
	Input::Get().updateKey();
	//�V�[���X�V
	sceneStack.top()->update();
}

void GameController::draw()
{
	//�V�[���`��
	sceneStack.top()->draw();
}