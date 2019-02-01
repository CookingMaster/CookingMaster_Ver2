﻿#include "GameController.h"
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
	//初期シーンの設定
	sceneStack_.push(std::make_unique<Scene::StageSelect>(this, nullptr, &entityManager_));	//タイトルシーンを作成し、プッシュ
	MasterSound::Get().update();
	sceneStack_.top()->initialize();
	
}

void GameController::onSceneChange(const Scene::SceneName& scene, Parameter* parame, const Scene::StackPopFlag stackClearFlag, const bool isInitialize)
{
	switch (stackClearFlag)
	{
	case Scene::StackPopFlag::NON:
		break;
	case Scene::StackPopFlag::POP:
		DOUT << "poped the scene stack" << std::endl;
		sceneStack_.pop();
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
		sceneStack_.push(std::make_unique<Scene::Title>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::SELECT:
		sceneStack_.push(std::make_unique<Scene::StageSelect>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::GAME:
		sceneStack_.push(std::make_unique<Scene::Game>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::PAUSE:
		sceneStack_.push(std::make_unique<Scene::Pause>(this, parame, &entityManager_));
		break;
	case Scene::SceneName::RESULT:
		sceneStack_.push(std::make_unique<Scene::Result>(this, parame, &entityManager_));
		break;
	default:
		break;
	}
	MasterSound::Get().update();
	if(isInitialize)	
	{
		sceneStack_.top()->initialize();
	}
}

void GameController::stackClear()
{
	while (!sceneStack_.empty())
	{
		sceneStack_.pop();
	}
}

void GameController::update()
{
	MasterSound::Get().update();
	entityManager_.refresh();
	Input::Get().updateKey();
	//シーン更新
	sceneStack_.top()->update();
}

void GameController::draw()
{
	//グループ順に描画
	SetDrawMode(DX_DRAWMODE_ANISOTROPIC);
	sceneStack_.top()->draw();
	SetDrawMode(DX_DRAWMODE_NEAREST);
}
