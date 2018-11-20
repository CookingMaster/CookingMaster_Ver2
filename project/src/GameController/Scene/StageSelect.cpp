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
		, entitytManager_(entityManager)
	{
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/1280.png", "back");
		ResourceManager::GetGraph().load("Resource/image/book.png", "book");
		ResourceManager::GetGraph().load("Resource/image/stage.png", "stage");
		ResourceManager::GetSound().load("Resource/sound/Welcome.ogg", "selectBGM",SoundType::BGM);
	}

	void StageSelect::initialize()
	{
		e = ECS::ScoreArcheType::CreateSelectScoreEntity("font", Vec2{ 100,100 }, ECS::StageHighScore::STAGE1, *entitytManager_);
		ECS::ArcheType::CreateGameBG("back",  Vec2{ 0.f,0.f},*entitytManager_);
		auto book = ECS::ArcheType::CreateEntity("book",  Vec2{ System::SCREEN_WIDIH / 2, System::SCREEN_HEIGHT / 2 },
			*entitytManager_,  ENTITY_GROUP::BACK);

		std::unordered_map<std::string, ECS::Entity*> stageButtons;
		ECS::ArcheType::CreateEntity("stage", Vec2{ 100.f,200.f }, *entitytManager_, ENTITY_GROUP::UI);
		ECS::ArcheType::CreateEntity("stage", Vec2{ 100.f,240.f }, *entitytManager_, ENTITY_GROUP::UI);
		ECS::ArcheType::CreateEntity("stage", Vec2{ 100.f,280.f }, *entitytManager_, ENTITY_GROUP::UI);
		
		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		entitytManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			auto stage_name = std::make_unique<Parameter>();
			stage_name->add<std::string>("BGM_name", "Stage1");
			ResourceManager::GetSound().load("Resource/sound/Let'sCooking.wav", "Stage1", SoundType::BGM);
			ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
		{
			auto stage_name = std::make_unique<Parameter>();
			stage_name->add<std::string>("BGM_name", "Stage2");
			ResourceManager::GetSound().load("Resource/sound/act_bgm.wav", "Stage2", SoundType::BGM);
			ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			auto stage_name = std::make_unique<Parameter>();
			stage_name->add<std::string>("BGM_name", "Stage3");
			ResourceManager::GetSound().load("Resource/sound/Grass.wav", "Stage3", SoundType::BGM);
			ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
		}
		
	}
	void StageSelect::draw()
	{
		entitytManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "セレクト画面");
		int a = e->getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE1);
		DrawFormatString(0, 100, 0xffffffff, "%d", a);
		
	}

	StageSelect::~StageSelect()
	{
		entitytManager_->allDestory();
		ResourceManager::GetSound().remove("selectBGM");
	}
}