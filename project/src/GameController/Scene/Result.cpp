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
	//�摜
	ResourceManager::GetGraph().load("Resource/image/cloche.png", "cloche");
	ResourceManager::GetGraph().load("Resource/image/menuback.png", "result_back");
	ResourceManager::GetGraph().load("Resource/image/dish.png", "dish");
	ResourceManager::GetGraph().load("Resource/image/confetti.png", "confetti");
	//�]���t�H���g
	ResourceManager::GetGraph().loadDiv("Resource/image/evaluation.png", "evaluation", 3, 1, 3, 598, 203);
	//�t�H���g
	ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");

	//�v���C�����X�e�[�W������o��
	setStage();
	//stage�ƃX�R�A�ɂ���ė����̉摜��ς���
	Vec2 dishImgPos = setDishImg();
	//�G���e�B�e�B������
	counter_.reset();
	counter_.setEndTime(300, 0);
	cloche_ = ECS::ResultArcheType::CreateClocheEntity("cloche", Vec2{ System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f + 100.f }, *entityManager_);
	back_ = ECS::ResultArcheType::CreateBackEntity("result_back", Vec2{ 0,0 }, *entityManager_);
	dish_ = ECS::ResultArcheType::CreateDishEntity("dish", dishImgPos, Vec2{ 512.f,512.f }, Vec2{ System::SCREEN_WIDIH/2.f, System::SCREEN_HEIGHT/2.f + 100.f }, *entityManager_);
	fade_ = ECS::ArcheType::CreateEntity("fade", Vec2{ 0.f,0.f }, *entityManager_, ENTITY_GROUP::UI);
	int index = getEvaluationIndex();
	evaluation_ = ECS::ResultArcheType::CreateEvaluationEntity(
		"evaluation", index, Vec2{ System::SCREEN_WIDIH / 2.f, 120.f }, Vec2_i{ 598,203 } , *entityManager_
	);
}

void Scene::Result::update()
{
	if (!isFadeOut_ && fade_->getComponent<ECS::AlphaBlend>().alpha >= 0) {
		fade_->getComponent<ECS::AlphaBlend>().alpha -= 6;
		return;
	}

	counter_.add();
	entityManager_->update();

	if (counter_.getCurrentCount() == 100) {
		if (score_ >= SCORE_GREAT) {
			for (int i = 0; i < 50; ++i) {
				confetties_.push_back(ECS::ResultArcheType::CreateConfettiEntity("confetti", Vec2_i{ 100 * (i % 5),0 }, Vec2_i{ 100,100 }, *entityManager_));
			}
		}
	}
	if (counter_.getCurrentCount() >= 100) {
		cloche_->addComponent<ECS::FlyAway>(Vec2{ 1920,600 }, 3.f);
		//�g��
		dish_->addComponent<ECS::Expand>(Vec2{ 1.8f,1.8f }, Easing::ExpoIn, 20.f);
		if (dish_->getComponent<ECS::Expand>().isEaseEnd()) {
			//�k��
			dish_->addComponent<ECS::Reduction>(Vec2{ 1.2f,1.2f }, Easing::ExpoOut, 8.f);
		}
	}
	if (counter_.getCurrentCount() >= 140) {
		evaluation_->addComponent<ECS::Expand>(Vec2{ 1.f,1.f },Easing::ExpoOut, 5.f);
	}
	if (counter_.getCurrentCount() >= 250) {
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
	entityManager_->allDestory();
}

Vec2 Scene::Result::setDishImg()
{
	Vec2 dishImgPos;

	//�X�R�A�ɂ����x���W��ς���
	if (score_ >= SCORE_GREAT) {
		dishImgPos.x = 0;
	}
	else if (score_ >= SCORE_GOOD) {
		dishImgPos.x = 512;
	}
	else {
		dishImgPos.x = 1024;
	}
	//�X�e�[�W�ɂ����y���W��ς���
	dishImgPos.y = (static_cast<int>(stage_) - 1) * 512.f;

	return dishImgPos;
}

void Scene::Result::setStage()
{
	//BGM������X�e�[�W������o��
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
