#include "Title.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"


namespace Scene
{
	Title::Title(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] const Parameter& parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entitytManager_(entityManager)
	{
		
	}
	void Title::update()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			Parameter playerDetail;
			playerDetail.add<std::string>("名前", "たかし");
			getCallBack().onSceneChange(SceneName::GAME, &playerDetail, true);
		}
		else if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
		{
			Parameter playerDetail;
			playerDetail.add<std::string>("名前", "まゆみ");
			getCallBack().onSceneChange(SceneName::GAME, &playerDetail, true);
		}
		else if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			Parameter playerDetail;
			playerDetail.add<std::string>("名前", "みつひこ");
			getCallBack().onSceneChange(SceneName::GAME, &playerDetail, true);
		}
	}
	void Title::draw()
	{
		DrawFormatString(0, 0, 0xffffffff, "タイトル画面");
	}

}