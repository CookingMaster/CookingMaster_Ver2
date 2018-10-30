#include "Pause.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"


namespace Scene
{
	Pause::Pause(IOnSceneChangeCallback* scenePauseChange, [[maybe_unused]] const Parameter& parame, ECS::EntityManager* entityManager)
		: AbstractScene(scenePauseChange)
		, entitytManager_(entityManager)
	{

	}
	void Pause::update()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			Parameter a;
			a.add("non",true);
			__super::getCallBack().onSceneChange(SceneName::GAME, &a, StackPopFlag::NON);
			//BGMŽ~‚ß‚é‚±‚Æ
			return;
		}
	}
	void Pause::draw()
	{
		DrawFormatString(0, 0, 0xffffffff, "PAUSE‰æ–Ê");
	}

}