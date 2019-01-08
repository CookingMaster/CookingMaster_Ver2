#include "Result.h"
#include "../../System/System.hpp"
#include "../GameController.h"
#include "../../ArcheType/ResultArcheType.hpp"
#include "../../Components/ResultComponent.hpp"
#include "../../Components/UIComponents.hpp"


Scene::Result::Result(IOnSceneChangeCallback * sceneTitleChange, [[maybe_unused]]Parameter * parame, ECS::EntityManager * entityManager)
	:
	AbstractScene(sceneTitleChange)
	, entityManager_(entityManager)
{
	if (parame != nullptr) {
		bgmName_ = (parame->get<std::string>("BGM_name"));
	}
}

void Scene::Result::initialize()
{
	//画像
	ResourceManager::GetGraph().load("Resource/image/cloche.png", "cloche");
	ResourceManager::GetGraph().load("Resource/image/1280.png", "game_bg");
	ResourceManager::GetGraph().load("Resource/image/dish.png", "dish");
	ResourceManager::GetGraph().load("Resource/image/confetti.png", "confetti");
	//フォント
	ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");

	//ハイスコアの取得テスト
	setScore();
	//stageとスコアによって料理の画像を変える
	Vec2 dishImgPos = setDishImg();

	//エンティティ初期化
	counter_.setEndTime(300, 0);
	cloche_ = ECS::ResultArcheType::CreateClocheEntity("cloche", Vec2{ 650,400 }, *entityManager_);
	back_ = ECS::ResultArcheType::CreateBackEntity("game_bg", Vec2{ 0,0 }, *entityManager_);
	dish_ = ECS::ResultArcheType::CreateDishEntity("dish", dishImgPos, Vec2{ 512,512 }, Vec2{ 610,280 }, *entityManager_);

}

void Scene::Result::update()
{
	counter_.add();
	entityManager_->update();

	if (counter_.getCurrentCount() == 100) {
		for (int i = 0; i < 50; ++i) {
			confetties_.push_back(ECS::ResultArcheType::CreateConfettiEntity("confetti", Vec2_i{ 100 * (i % 5),0 }, Vec2_i{ 100,100 }, *entityManager_));
		}
	}
	if (counter_.getCurrentCount() >= 100) {
		cloche_->addComponent<ECS::FlyAway>(Vec2{ 1920,600 }, 3.f);
		//拡大
		dish_->addComponent<ECS::Expand>(Vec2{ 2.0f,2.0f }, Easing::ExpoIn, 20.f);
		if (dish_->getComponent<ECS::Expand>().isEaseEnd()) {
			//縮小
			dish_->addComponent<ECS::Expand>(Vec2{ 1.0f,1.0f }, Easing::ExpoOut, 10.f);
			DOUT << dish_->getComponent<ECS::Scale>().val.x << std::endl;
		}
	}

	if (counter_.isMax()) {
		DOUT << "BackToTitle" << std::endl;
		ON_SCENE_CHANGE(SceneName::SELECT, nullptr, StackPopFlag::ALL_CLEAR, true);
	}
}

void Scene::Result::draw()
{
	entityManager_->orderByDraw(ENTITY_GROUP::MAX);
	DrawFormatString(0, 0, 0xffffffff, "RESULT");
}

Scene::Result::~Result()
{
	entityManager_->allDestory();
}

Vec2 Scene::Result::setDishImg()
{
	Vec2 dishImgPos;

	//test
	score_ = 50;

	//スコアによってx座標を変える
	if (score_ >= 80) {
		dishImgPos.x = 0;
	}
	else if (score_ >= 50) {
		dishImgPos.x = 512;
	}
	else {
		dishImgPos.x = 1024;
	}
	//ステージによってy座標を変える
	dishImgPos.y = (static_cast<int>(stage_) - 1) * 512.f;

	return dishImgPos;
}

void Scene::Result::setScore()
{
	//BGM名からステージを割り出す
	if (bgmName_ == "stage1") {
		stage_ = ECS::StageHighScore::STAGE1;
	}
	else if (bgmName_ == "stage2") {
		stage_ = ECS::StageHighScore::STAGE2;
	}
	else if (bgmName_ == "stage3") {
		stage_ = ECS::StageHighScore::STAGE3;
	}
	//ハイスコアの読み込み
	score_ = ECS::ScoreArcheType::CreateSelectScoreEntity("font", Vec2{ 100,200 }, ECS::StageHighScore::STAGE2, *entityManager_)->
		getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE2);
}