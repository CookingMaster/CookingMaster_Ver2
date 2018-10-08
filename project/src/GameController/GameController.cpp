#include "GameController.h"
#include "../Class/ResourceManager.hpp"
#include "Scene/SceneManager.hpp"
#include "../Input/Input.hpp"
#include "../Events/EventManager.hpp"
#include "../Events/TestEvent.hpp"
void GameController::resourceLoad()
{
	
}

GameController::GameController()
{
	//最初に必要なリソースやEntityの生成、ロードを行う
	resourceLoad();
	//初期シーンの設定
	Scene::SceneManager::Get().changeScene(Scene::SceneManager::State::TITLE, entityManager_);
	//イベント追加
	Event::EventManager::Get().addEvent(Scene::SceneManager::State::GAME, Event::TestEvents::BlendSelect);
}


void GameController::update()
{
	entityManager_.refresh();
	Input::Get().updateKey();
	//シーン更新
	Scene::SceneManager::Get().update();
	//イベント更新
	Event::EventManager::Get().update(entityManager_);
}

void GameController::draw()
{
	//シーン描画
	Scene::SceneManager::Get().draw();
}