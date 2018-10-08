#include "Title.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"
namespace Scene
{
	Title::Title(ECS::EntityManager& manager) :
		entityManager_(manager)
	{}
	void Title::update()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_S) == 1)
		{
			SceneManager::Get().changeScene(SceneManager::State::GAME, entityManager_);
		}
		
	}
	void Title::draw()
	{
		DrawFormatString(0,0,0xffffffff,"Sキー押してゲームシーンへ");
	}
	void Title::release()
	{
	}
}