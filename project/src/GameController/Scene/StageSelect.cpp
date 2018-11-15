#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
namespace Scene
{
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entitytManager_(entityManager)
	{
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		
	}

	void StageSelect::initialize()
	{
		e = ECS::ScoreArcheType::CreateSelectScoreEntity("font", Vec2{ 100,100 }, ECS::StageHighScore::STAGE1, *entitytManager_);
	}
	void StageSelect::update()
	{
		entitytManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			auto playerDetail = std::make_unique<Parameter>();
			playerDetail->add<std::string>("名前", "たかし");
			getCallBack().onSceneChange(SceneName::GAME, playerDetail.get(), StackPopFlag::POP, true);
		}
		
	}
	void StageSelect::draw()
	{
		entitytManager_->draw2D();
		DrawFormatString(0, 0, 0xffffffff, "セレクト画面");
		int a = e->getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE1);
		DrawFormatString(0, 100, 0xffffffff, "%d", a);
		
	}

	StageSelect::~StageSelect()
	{
		entitytManager_->allDestory();
	}
}