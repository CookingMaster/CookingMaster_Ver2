#include "Title.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"
#include "../../ArcheType/TitleUIArcheType.hpp"


namespace Scene
{
	Title::Title(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
	{
		ResourceManager::GetGraph().load("Resource/image/press_any_key.png", "pak");
	}
	void Title::initialize()
	{
		ECS::TitleUIArcheType::CreateLogoArchetype("pak", Vec2(200, 100), *entityManager_);
	}
	void Title::update()
	{
		entityManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			ON_SCENE_CHANGE(SceneName::SELECT, nullptr, StackPopFlag::POP, true);
		}
	}
	void Title::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "タイトル画面");
	}

	Title::~Title()
	{
		entityManager_->allDestory();
		ResourceManager::GetSound().remove("pak");
	}
}