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
#include "../ArcheType/ArcheType.hpp"

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
	sceneStack_.push(std::make_unique<Scene::Title>(this, nullptr, &entityManager_));	//タイトルシーンを作成し、プッシュ
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
	if (gameEndFadeOut_ == nullptr)
	{
		//シーン更新
		sceneStack_.top()->update();
	}
	else
	{
		fadeSoundGain_ -= 0.01f;
		MasterSound::Get().setAllBGMGain(fadeSoundGain_);
		gameEndFadeOut_->update();
	}
	
}

void GameController::draw()
{
	//グループ順に描画
	SetDrawMode(DX_DRAWMODE_ANISOTROPIC);
	sceneStack_.top()->draw();
	if (gameEndFadeOut_ != nullptr)
	{
		gameEndFadeOut_->draw2D();
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

const bool GameController::pushEscapeAndFadeOut()
{
	if (Input::Get().getKeyFrame(KEY_INPUT_ESCAPE) == 1)
	{
		ResourceManager::GetGraph().load("Resource/image/fade_black.png", "endfade");
		ResourceManager::GetSound().load("Resource/sound/SE/thankyou.ogg", "end", SoundType::SE);
		fadeSoundGain_= MasterSound::Get().getBGMGain();
		Sound se("end");
		se.play(false,true);
		gameEndFadeOut_ = ECS::ArcheType::CreateEntity
		(
			"endfade",
			Vec2{ 0.f,0.f },
			entityManager_,
			ENTITY_GROUP::TOP_FADE
		);
		gameEndFadeOut_->addComponent<ECS::FadeComponent>(0.f, 255.f, 100.f);
	}
	if (gameEndFadeOut_ == nullptr)
	{
		return false;
	}
	if (gameEndFadeOut_->getComponent<ECS::FadeComponent>().isFadeEnd())
	{
		return true;
	}
	return false;
}
