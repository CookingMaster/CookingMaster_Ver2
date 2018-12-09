#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
#include "../src/Class/Sound.hpp"
#include "../src/ArcheType/TestArcheType.hpp"

namespace Scene
{
	
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
	{
		//背景
		ResourceManager::GetGraph().load("Resource/image/menuback.png", "back"); 
		//本
		ResourceManager::GetGraph().load("Resource/image/menu2.png", "menu");
		//文字
		ResourceManager::GetGraph().load("Resource/image/menuname.png", "menuname");
		//オプション
		ResourceManager::GetGraph().load("Resource/image/option2.png", "option");

	}

	void StageSelect::initialize()
	{
		
		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		entityManager_->update();
		
		if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
		{
			auto name = std::make_unique<Parameter>();
			ResourceManager::GetSound().load("Resource/sound/Let'sCooking.wav","stage1",SoundType::BGM);
			name->add<std::string>("BGM_name", "stage1");
			ON_SCENE_CHANGE(SceneName::GAME, name.get(), StackPopFlag::POP, true);
		}
	}
	void StageSelect::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		
	}

	StageSelect::~StageSelect()
	{
		entityManager_->allDestory();
		ResourceManager::GetSound().remove("selectBGM");
	}

}// namespace StageSelect