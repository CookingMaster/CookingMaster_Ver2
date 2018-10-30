#include "Title.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"


namespace Scene
{
	Title::Title(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entitytManager_(entityManager)
	{
		
	}
	void Title::update()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			auto playerDetail = std::make_unique<Parameter>();
			playerDetail->add<std::string>("名前", "たかし");
			getCallBack().onSceneChange(SceneName::GAME, playerDetail.get(), StackPopFlag::POP, false);
		}
	}
	void Title::draw()
	{
		DrawFormatString(0, 0, 0xffffffff, "タイトル画面");
	}

}