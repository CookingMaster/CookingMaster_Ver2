#include "Pause.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"
#include "../../ArcheType/UIArcheType.hpp"


namespace Scene
{
	Pause::Pause(IOnSceneChangeCallback* scenePauseChange, [[maybe_unused]] const Parameter& parame, ECS::EntityManager* entityManager)
		: AbstractScene(scenePauseChange)
		, entitytManager_(entityManager)
	{
		ResourceManager::GetGraph().load("Resource/image/pause_black.png", "pause_bg");
		ResourceManager::GetGraph().load("Resource/image/pause.png", "pause");

	}
	void Pause::update()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			Parameter a;
			a.add("non",true);
			__super::getCallBack().onSceneChange(SceneName::GAME, &a, StackPopFlag::NON);
			//BGM�ĊJ���邱��
			return;
		}


	}
	void Pause::draw()
	{
		DrawFormatString(0, 0, 0xffffffff, "PAUSE���");
	}

}