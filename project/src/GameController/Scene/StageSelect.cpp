#include "StageSelect.h"
#include "../src/Input/Input.hpp"
namespace Scene
{
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entitytManager_(entityManager)
	{

	}
	void StageSelect::update()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			auto playerDetail = std::make_unique<Parameter>();
			playerDetail->add<std::string>("名前", "たかし");
			getCallBack().onSceneChange(SceneName::GAME, playerDetail.get(), StackPopFlag::POP, true);
		}
	}
	void StageSelect::draw()
	{
		DrawFormatString(0, 0, 0xffffffff, "セレクト画面");
	}

}