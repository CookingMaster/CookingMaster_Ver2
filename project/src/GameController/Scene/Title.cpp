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
		, progress(0)
	{
		ResourceManager::GetGraph().load("Resource/image/press_any_key.png", "pak");
		ResourceManager::GetGraph().load("Resource/image/kari_titlelogo.png", "logo");
	}
	void Title::initialize()
	{
		ECS::TitleUIArcheType::CreateLogoArchetype("logo",
			Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f - 200.f),
			*entityManager_);
	}
	void Title::update()
	{
		entityManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			ON_SCENE_CHANGE(SceneName::SELECT, nullptr, StackPopFlag::POP, true);
		}

		BehaviorForProgress();
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

	void Title::BehaviorForProgress()
	{
		switch (progress)
		{
		case 0:
			auto& logo = entityManager_->getEntitiesByGroup(ENTITY_GROUP::TITLE_UI);
			if (logo[0]->getComponent<ECS::EasingPosMove>().getIsEaseEnd())
			{
				ECS::TitleUIArcheType::CreateMessageArchetype("pak",
					Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f + 200.f),
					*entityManager_);
				++progress;
			}
			break;
		}
	}
}