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
		ResourceManager::GetGraph().load("Resource/image/title_sora.png", "title_back");
		ResourceManager::GetGraph().load("Resource/image/title_cloud.png", "cloud");
		ResourceManager::GetGraph().load("Resource/image/title_tennai.png", "title_ten");
		ResourceManager::GetGraph().load("Resource/image/title_door.png", "title_door");
		ResourceManager::GetGraph().load("Resource/image/title_gaisou.png", "title_gaisou");
		ResourceManager::GetGraph().load("Resource/image/fade_white.png", "fw");
	}
	void Title::initialize()
	{
		ECS::TitleUIArcheType::CreateFade("fw", *entityManager_);

		ECS::TitleUIArcheType::CreateLogoArchetype("logo",
			Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f - 200.f),
			*entityManager_);

		ECS::TitleUIArcheType::CreateTitleBGArchetype("title_back", Vec2(0.f, 0.f), *entityManager_);		//背景
		for (int i = 0; i < 3; ++i)
		{
			//雲
		}
		ECS::TitleUIArcheType::CreateTitleBGArchetype("title_ten", Vec2(607.f, 534.f), *entityManager_);	//店内
		ECS::TitleUIArcheType::CreateTitleBGArchetype("title_door", Vec2(607.f, 534.f), *entityManager_);	//ドア
		ECS::TitleUIArcheType::CreateTitleBGArchetype("title_gaisou", Vec2(0.f, 0.f), *entityManager_);		//外装
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
