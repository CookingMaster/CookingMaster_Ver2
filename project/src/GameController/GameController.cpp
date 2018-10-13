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
	//最初に必要なリソースやEntityの生成、ロードを行う
	resourceLoad();
	//初期シーンの設定
	sceneStack.push(std::make_unique<Scene::Title >(this, param, &entityManager_));	//タイトルシーンを作成し、プッシュ
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
	//シーン更新
	sceneStack.top()->update();
}

void GameController::draw()
{
	//シーン描画
	sceneStack.top()->draw();
}