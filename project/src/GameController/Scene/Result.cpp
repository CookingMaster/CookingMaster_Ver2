#include "Result.h"
#include "../../System/System.hpp"
#include "../GameController.h"
#include "../../ArcheType/ResultArcheType.hpp"
#include "../../Components/ResultComponent.hpp"
#include "../../Components/UIComponents.hpp"
#include "../src/ArcheType/ArcheType.hpp"


Scene::Result::Result(IOnSceneChangeCallback * sceneTitleChange, [[maybe_unused]]Parameter * parame, ECS::EntityManager * entityManager)
	:
	AbstractScene(sceneTitleChange)
	, entityManager_(entityManager)
{
	if (parame != nullptr) {
		bgmName_ = (parame->get<std::string>("BGM_name"));
		score_ = (parame->get<int>("score"));
	}
}

void Scene::Result::initialize()
{
	//Fade
	isFadeOut_ = false;
	ResourceManager::GetGraph().load("Resource/image/fade_black.png", "fade");
	//画像
	ResourceManager::GetGraph().load("Resource/image/cloche.png", "cloche");
	ResourceManager::GetGraph().load("Resource/image/menuback.png", "result_back");
	ResourceManager::GetGraph().load("Resource/image/dish.png", "dish");
	ResourceManager::GetGraph().load("Resource/image/confetti.png", "confetti");
	ResourceManager::GetGraph().load("Resource/image/result_black.png", "black");
	//評価フォント
	ResourceManager::GetGraph().loadDiv("Resource/image/evaluation.png", "evaluation", 3, 1, 3, 598, 203);
	//フォント
	ResourceManager::GetGraph().load("Resource/image/score_combo.png", "scorecombo");
	ResourceManager::GetGraph().load("Resource/image/score_font.png", "scorefont");

	//プレイしたステージを割り出す
	setStage();
	//stageとスコアによって料理の画像を変える
	Vec2 dishImgPos = setDishImg();
	//スコアによって評価フォントの画像を変える
	int index = getEvaluationIndex();

	//カウンタ初期化
	counter_.reset();
	counter_.setEndTime(600, 0);
	//エンティティ初期化
	cloche_ = ECS::ResultArcheType::CreateClocheEntity(
		"cloche",
		Vec2{ System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f + 50.f },
		*entityManager_
	);
	dish_ = ECS::ResultArcheType::CreateDishEntity(
		"dish",
		dishImgPos,
		Vec2{ 512.f,512.f },
		Vec2{ System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f + 100.f },
		*entityManager_
	);
	back_ = ECS::ResultArcheType::CreateBackEntity(
		"result_back",
		Vec2{ 0,0 },
		*entityManager_
	);
	fade_ = ECS::ArcheType::CreateEntity(
		"fade",
		Vec2{ 0.f,0.f },
		*entityManager_,
		ENTITY_GROUP::FADE
	);
	evaluation_ = ECS::ResultArcheType::CreateEvaluationEntity(
		"evaluation",
		index,
		Vec2{ System::SCREEN_WIDIH / 2.f, 125.f },
		Vec2_i{ 598,203 },
		*entityManager_
	);
}

void Scene::Result::update()
{
	//フェードイン
	if (!isFadeOut_ && fade_->getComponent<ECS::AlphaBlend>().alpha >= 0) {
		fade_->getComponent<ECS::AlphaBlend>().alpha -= 6;
		return;
	}

	counter_.add();
	entityManager_->update();

	if (counter_.getCurrentCount() == Timing::CONFETTI) {
		//紙吹雪
		if (score_ >= SCORE_GREAT) {
			for (int i = 0; i < 50; ++i) {
				confetties_.push_back(
					ECS::ResultArcheType::CreateConfettiEntity(
						"confetti",
						Vec2_i{ 100 * (i % 5),0 },
						Vec2_i{ 100,100 },
						*entityManager_
					)
				);
			}
		}
	}
	if (counter_.getCurrentCount() == Timing::CONFETTI) {
		//クロッシュ飛ばす
		cloche_->addComponent<ECS::FlyAway>(Vec2{ 1920,600 }, 3.f);
		//料理拡大
		dish_->addComponent<ECS::Expand>(
			Vec2{ 1.8f,1.8f },
			Easing::ExpoIn,
			20.f
			);
	}
	if (dish_->hasComponent<ECS::Expand>() && dish_->getComponent<ECS::Expand>().isEaseEnd()) {
		//料理縮小
		dish_->addComponent<ECS::Reduction>(
			Vec2{ 1.2f,1.2f },
			Easing::ExpoOut, 8.f
			);
	}
	if (counter_.getCurrentCount() == Timing::EVALUATION) {
		//評価フォント拡大
		evaluation_->addComponent<ECS::Expand>(Vec2{ 1.f,1.f }, Easing::ExpoOut, 5.f);
		//クロッシュ戻ってくるので移動コンポーネントを消す
		cloche_->removeComponent<ECS::FlyAway>();
	}
	if (counter_.getCurrentCount() == Timing::SCORE_BACK) {
		//スコア背景入場
		auto black = ECS::ArcheType::CreateEntity(
			"black",
			Vec2{ System::SCREEN_WIDIH / 2 - 500.f,-720.f },
			*entityManager_, ENTITY_GROUP::EFFECT);
		black->addComponent<ECS::EasingMove>(
			Easing::LinearIn,
			Vec2{ System::SCREEN_WIDIH / 2 - 500.f,0.f },
			10.f
			);
	}
	if (counter_.getCurrentCount() == Timing::SCOREFONT) {
		//スコアフォント入場
		ECS::ArcheType::CreateEntity(
			"scorecombo",
			Vec2{ System::SCREEN_WIDIH / 2.f - 500.f, System::SCREEN_HEIGHT / 2.f - 100.f },
			*entityManager_,
			ENTITY_GROUP::UI
		);
	}
	if (counter_.getCurrentCount() == Timing::SCORE) {
		//スコア出現
		ECS::ResultArcheType::CreateScoreEntity(
			"scorefont",
			Vec2{ System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f - 30.f},
			score_,
			*entityManager_
		);
	}
	if (counter_.getCurrentCount() == Timing::COMBO) {
		//コンボ出現
		ECS::ResultArcheType::CreateScoreEntity(
			"scorefont",
			Vec2{ System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f },
			score_,
			*entityManager_
		);
	}
	if (counter_.getCurrentCount() >= Timing::FADE_OUT) {
		isFadeOut_ = true;
	}
	if (isFadeOut_) {
		fade_->getComponent<ECS::SpriteDraw>().drawEnable();
		fade_->getComponent<ECS::AlphaBlend>().alpha += 6;
	}
	if (fade_->getComponent<ECS::AlphaBlend>().alpha >= 255) {
		DOUT << "BackToTitle" << std::endl;
		ON_SCENE_CHANGE(SceneName::SELECT, nullptr, StackPopFlag::ALL_CLEAR, true);
	}

}

void Scene::Result::draw()
{
	entityManager_->orderByDraw(ENTITY_GROUP::MAX);
}

Scene::Result::~Result()
{
	entityManager_->removeAll();
}

Vec2 Scene::Result::setDishImg()
{
	Vec2 dishImgPos;

	//スコアによってx座標を変える
	if (score_ >= SCORE_GREAT) {
		dishImgPos.x = 0;
	}
	else if (score_ >= SCORE_GOOD) {
		dishImgPos.x = 512;
	}
	else {
		dishImgPos.x = 1024;
	}
	//ステージによってy座標を変える
	dishImgPos.y = (static_cast<int>(stage_) - 1) * 512.f;

	return dishImgPos;
}

void Scene::Result::setStage()
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
	else {
		stage_ = ECS::StageHighScore::STAGE1;
	}
}

int Scene::Result::getEvaluationIndex() {
	if (score_ >= SCORE_GREAT) {
		return 2;
	}
	else if (score_ >= SCORE_GOOD) {
		return 1;
	}
	else {
		return 0;
	}
}
