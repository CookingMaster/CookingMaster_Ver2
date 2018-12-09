#include "GameController.h"
#include "../Class/ResourceManager.hpp"
#include "Scene/SceneManager.hpp"
#include "../Input/Input.hpp"
#include "Scene/Title.h"
#include "Scene/StageSelect.h"
#include "Scene/Game.h"
#include "Scene/Pause.h"
#include "Scene/Result.h"
#include "../Class/Sound.hpp"
void GameController::resourceLoad()
{
	
}

GameController::GameController()
{
	//最初に必要なリソースやEntityの生成、ロードを行う
	resourceLoad();
	//サウンド情報の読み込み
	std::ifstream ifs("Resource/system/gain.bin");
	float BGM;
	float SE;

	ifs >> BGM >> SE;

	MasterSound::Get().setAllBGMGain(BGM);
	MasterSound::Get().setAllSEGain(SE);
	MasterSound::Get().update();
	//初期シーンの設定
	sceneStack.push(std::make_unique<Scene::Title>(this, nullptr, &entityManager_));	//タイトルシーンを作成し、プッシュ
	sceneStack.top()->initialize();
	
}

void GameController::onSceneChange(const Scene::SceneName& scene, Parameter* parame, const Scene::StackPopFlag stackClearFlag, const bool isInitialize)
{
	switch (stackClearFlag)
	{
	case Scene::StackPopFlag::NON:
		break;
	case Scene::StackPopFlag::POP:
		DOUT << "poped the scene stack" << std::endl;
		sceneStack.pop();
		break;
	case Scene::StackPopFlag::ALL_CLEAR:
		DOUT << "poped the scene stack all" << std::endl;
		stackClear();
		break;
	default:
		break;
	}
	switch (scene)
	{
	case Scene::SceneName::TITLE:
		sceneStack.push(std::make_unique<Scene::Title>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::SELECT:
		sceneStack.push(std::make_unique<Scene::StageSelect>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::GAME:
		sceneStack.push(std::make_unique<Scene::Game>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::PAUSE:
		sceneStack.push(std::make_unique<Scene::Pause>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::RESULT:
		sceneStack.push(std::make_unique<Scene::Result>(this, parame, &entityManager_));
		break;
	default:
		break;
	}

	if(isInitialize)	
	{
		sceneStack.top()->initialize();
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
	MasterSound::Get().update();
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