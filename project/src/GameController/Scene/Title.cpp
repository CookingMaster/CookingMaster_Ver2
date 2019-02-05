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
		ResourceManager::GetGraph().load("Resource/image/logo.png", "logo");
		ResourceManager::GetGraph().load("Resource/image/title_bg.png", "title_bg");
		ResourceManager::GetGraph().load("Resource/image/title_cloud.png", "cloud");
		ResourceManager::GetGraph().load("Resource/image/title_door.png", "title_door");
		ResourceManager::GetGraph().load("Resource/image/title_building.png", "title_building");
		ResourceManager::GetGraph().load("Resource/image/fade_white.png", "fw");
	}
	void Title::initialize()
	{
		ECS::TitleUIArcheType::CreateFade("fw", *entityManager_);

		ECS::TitleUIArcheType::CreateLogoArchetype("logo",
			Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f - 200.f),
			*entityManager_);

		ECS::TitleUIArcheType::CreateTitleBGArchetype("title_bg", Vec2(0.f, 0.f), *entityManager_);		//背景
		for (int i = 1; i <= 2; ++i)
		{
			ECS::TitleUIArcheType::CreateCloud("cloud", 172, i, *entityManager_);//雲
		}
		ECS::TitleUIArcheType::CreateTitleBGArchetype("title_door", Vec2(556.f, 388.f), *entityManager_, true);	//ドア
		ECS::TitleUIArcheType::CreateTitleBGArchetype("title_building", Vec2(251.f, 92.f), *entityManager_);	//外装
	}
	void Title::update()
	{
		entityManager_->update();
		BehaviorForProgress();
	}
	void Title::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "タイトル画面");
	}

	Title::~Title()
	{
		entityManager_->removeAll();
		ResourceManager::GetSound().remove("pak");
	}

	void Title::BehaviorForProgress()
	{
		auto& logo = entityManager_->getEntitiesByGroup(ENTITY_GROUP::TITLE_LOGO);
		auto& message = entityManager_->getEntitiesByGroup(ENTITY_GROUP::TITLE_MESSAGE);
		auto& fade = entityManager_->getEntitiesByGroup(ENTITY_GROUP::TOP_FADE);
		switch (progress)
		{
		case 0:
			if (logo[0]->getComponent<ECS::EasingPosMove>().getIsEaseEnd())
			{
				ECS::TitleUIArcheType::CreateMessageArchetype("pak",
					Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f + 200.f),
					*entityManager_);
				++progress;
			}
			break;

		case 1:
			if (message[0]->getComponent<ECS::AnyInputFunction>().getIsPushed())
			{
				fade[0]->getComponent<ECS::FadeComponent>().reset(0.f, 255.f, 80.f);
				++progress;
			}
			break;

		case 2:
			if (fade[0]->getComponent<ECS::FadeComponent>().isFadeEnd())
			{
				ON_SCENE_CHANGE(SceneName::SELECT, nullptr, StackPopFlag::POP, true);
			}
			break;
		}
	}
}
