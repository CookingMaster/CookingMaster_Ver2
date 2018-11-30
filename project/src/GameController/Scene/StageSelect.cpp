#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
#include "../src/Class/Sound.hpp"
#include "../src/ArcheType/TestArcheType.hpp"

namespace Scene
{
	void StageSelect::selectStageMove()
	{
		pointEntityMove.pos = &point_->getComponent<ECS::Position>();
		//ステージ選択時の挙動
		if (!pointEntityMove.isOptionSelected)
		{
			//下方向
			if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
			{
				if (pointEntityMove.selectNum < UIMap_.size() - 1)
				{
					++pointEntityMove.selectNum;
					pointEntityMove.pos->val.y = UIMap_[pointEntityMove.selectNum]->getComponent<ECS::Position>().val.y;
				}
				else
				{
					pointEntityMove.pos->val.y = UIMap_.front()->getComponent<ECS::Position>().val.y;
					pointEntityMove.selectNum = 0;
				}
			}
			//上方向
			if (Input::Get().getKeyFrame(KEY_INPUT_UP) == 1)
			{
				if (pointEntityMove.selectNum > 0)
				{
					--pointEntityMove.selectNum;
					pointEntityMove.pos->val.y = UIMap_[pointEntityMove.selectNum]->getComponent<ECS::Position>().val.y;
				}
				else
				{
					pointEntityMove.pos->val.y = UIMap_.back()->getComponent<ECS::Position>().val.y;
					pointEntityMove.selectNum = UIMap_.size() - 1;
				}
			}
		}
		//オプション時の操作
		else
		{
			auto& bgmBarPos = bgmSlider_.barEntity->getComponent<ECS::Position>();
			auto& seBarPos = seSlider_.barEntity->getComponent<ECS::Position>();
			
			if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
			{
				pointEntityMove.isOptionSelected = false;
				bgmSlider_.isSelect = false;
				seSlider_.isSelect = false;
				std::ofstream ofs("Resource/system/gain.bin");
				ofs << bgmSlider_.volume <<  seSlider_.volume;
			}

			if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1)
			{
				bgmSlider_.isSelect = !bgmSlider_.isSelect;
				seSlider_.isSelect = !seSlider_.isSelect;
			}

			//BGMスライダー
			if (bgmSlider_.isSelect)
			{
				if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) >= 1 
					&& !(bgmSlider_.volume >= MasterSound::MAX_GAIN))
				{
					bgmBarPos.val.x++;
					bgmSlider_.volume += 0.005f;
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) >= 1 
					&& !(bgmSlider_.volume <= MasterSound::MIN_GAIN))
				{
					bgmBarPos.val.x--;
					bgmSlider_.volume -= 0.005f;
				}
			}
			//SEスライダー
			if (seSlider_.isSelect)
			{
				if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) >= 1 
					&& !(seSlider_.volume >= MasterSound::MAX_GAIN))
				{
					seBarPos.val.x++;
					seSlider_.volume += 0.005f;
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) >= 1
					&& !(seSlider_.volume <= MasterSound::MIN_GAIN))
				{
					seBarPos.val.x--;
					seSlider_.volume -= 0.005f;
				}
			}
		}
		//バーの位置をセットMasterSound::MIN_GAIN
		constexpr float MAX_GAUGE_SIZE = 270;
		bgmSlider_.barEntity->getComponent<ECS::Position>().val.x = 
			bgmSlider_.volume * MAX_GAUGE_SIZE + (bgmSlider_.gaugeEntity->getComponent<ECS::Position>().val.x) -MAX_GAUGE_SIZE / 2;
		seSlider_.barEntity->getComponent<ECS::Position>().val.x =
			seSlider_.volume * MAX_GAUGE_SIZE + (seSlider_.gaugeEntity->getComponent<ECS::Position>().val.x) - MAX_GAUGE_SIZE / 2;
	}

	void StageSelect::selectStage()
	{
		UIReset();
		switch (pointEntityMove.selectNum)
		{
		case 0:
		{
			if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
			{
				auto stage_name = std::make_unique<Parameter>();
				stage_name->add<std::string>("BGM_name", "Stage1");
				ResourceManager::GetSound().load("Resource/sound/Let'sCooking.wav", "Stage1", SoundType::BGM);
				ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
			}
			cookMap_[pointEntityMove.selectNum]->getComponent<ECS::SpriteDraw>().drawEnable();
			break;
		}
		case 1:
		{
			if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
			{
				auto stage_name = std::make_unique<Parameter>();
				stage_name->add<std::string>("BGM_name", "Stage2");
				ResourceManager::GetSound().load("Resource/sound/act_bgm.wav", "Stage2", SoundType::BGM);
				ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
			}
			cookMap_[pointEntityMove.selectNum]->getComponent<ECS::SpriteDraw>().drawEnable();
			break;
		}

		case 2:
		{
			if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
			{
				auto stage_name = std::make_unique<Parameter>();
				stage_name->add<std::string>("BGM_name", "Stage3");
				ResourceManager::GetSound().load("Resource/sound/Grass.wav", "Stage3", SoundType::BGM);
				ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
			}
			cookMap_[pointEntityMove.selectNum]->getComponent<ECS::SpriteDraw>().drawEnable();
			break;
		}
		case 3:
		{
			if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1
				&& !pointEntityMove.isOptionSelected)
			{
				Sound se("onion");
				se.play(false, true);
				pointEntityMove.isOptionSelected = true;
				bgmSlider_.isSelect = true;
				seSlider_.isSelect = false;
			}
			bgmSlider_.gaugeEntity->getComponent<ECS::SpriteDraw>().drawEnable();
			seSlider_.gaugeEntity->getComponent<ECS::SpriteDraw>().drawEnable();
			bgmSlider_.barEntity->getComponent<ECS::SpriteDraw>().drawEnable();
			seSlider_.barEntity->getComponent<ECS::SpriteDraw>().drawEnable();
			break;
		}
		}
	}
	void StageSelect::UIReset()
	{
		for (auto& it : cookMap_)
		{
			it->getComponent<ECS::SpriteDraw>().drawDisable();
		}
		bgmSlider_.gaugeEntity->getComponent<ECS::SpriteDraw>().drawDisable();
		seSlider_.gaugeEntity->getComponent<ECS::SpriteDraw>().drawDisable();
		bgmSlider_.barEntity->getComponent<ECS::SpriteDraw>().drawDisable();
		seSlider_.barEntity->getComponent<ECS::SpriteDraw>().drawDisable();
	}

	
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
	{
		ResourceManager::GetSound().load("Resource/sound/onion.ogg", "onion", SoundType::SE);
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		//仮料理画像とUI
		ResourceManager::GetGraph().load("Resource/image/cook.png", "tkg");
		ResourceManager::GetGraph().load("Resource/image/cook2.png", "tkg2");
		ResourceManager::GetGraph().load("Resource/image/cook3.png", "tkg3");
		//スライダー
		ResourceManager::GetGraph().load("Resource/image/slider.png", "slider");
		ResourceManager::GetGraph().load("Resource/image/slider_bar.png", "slider_bar");
		ResourceManager::GetGraph().load("Resource/image/1280.png", "back");
		ResourceManager::GetGraph().load("Resource/image/book.png", "book");
		ResourceManager::GetGraph().load("Resource/image/point.png", "point");
		ResourceManager::GetGraph().load("Resource/image/option.png", "option");
		ResourceManager::GetGraph().load("Resource/image/stage.png", "stage");
		ResourceManager::GetSound().load("Resource/sound/Welcome.ogg", "selectBGM",SoundType::BGM);
	}

	void StageSelect::initialize()
	{
		//ハイスコアの取得テスト
		score_ = ECS::ScoreArcheType::CreateSelectScoreEntity("font", Vec2{ 100,200 }, ECS::StageHighScore::STAGE2, *entityManager_)->
			getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE2);

		ECS::ArcheType::CreateGameBG("back",  Vec2{ 0.f,0.f},*entityManager_);

		auto book_ = ECS::ArcheType::CreateEntity("book",  Vec2{ System::SCREEN_WIDIH / 2, System::SCREEN_HEIGHT * 0.6f },
			*entityManager_,  ENTITY_GROUP::BACK);
		auto bookPos = book_->getComponent<ECS::Position>();

		//ボタンUI
		UIMap_.push_back(ECS::ArcheType::CreateEntity("stage", Vec2{ bookPos.val.x * 0.7f, bookPos.val.y - UI_HEIGHT * 2 }, *entityManager_, ENTITY_GROUP::UI));
		UIMap_.push_back(ECS::ArcheType::CreateEntity("stage", Vec2{ bookPos.val.x * 0.7f, bookPos.val.y - UI_HEIGHT }, *entityManager_, ENTITY_GROUP::UI));
		UIMap_.push_back(ECS::ArcheType::CreateEntity("stage", Vec2{ bookPos.val.x * 0.7f, bookPos.val.y }, *entityManager_, ENTITY_GROUP::UI));
		UIMap_.push_back(ECS::ArcheType::CreateEntity("option",Vec2{ bookPos.val.x * 0.7f, bookPos.val.y + UI_HEIGHT}, *entityManager_, ENTITY_GROUP::UI));
		//料理画像
		cookMap_.push_back(ECS::ArcheType::CreateEntity("tkg",  Vec2{ bookPos.val.x * 1.3f, bookPos.val.y - 60}, *entityManager_, ENTITY_GROUP::UI));
		cookMap_.push_back(ECS::ArcheType::CreateEntity("tkg2", Vec2{ bookPos.val.x * 1.3f, bookPos.val.y - 60 }, *entityManager_, ENTITY_GROUP::UI));
		cookMap_.push_back(ECS::ArcheType::CreateEntity("tkg3", Vec2{ bookPos.val.x * 1.3f, bookPos.val.y - 60 }, *entityManager_, ENTITY_GROUP::UI));
		//スライダー
		{
			bgmSlider_.gaugeEntity = ECS::ArcheType::CreateEntity("slider", Vec2{ bookPos.val.x * 1.4f,  bookPos.val.y - UI_HEIGHT }, *entityManager_, ENTITY_GROUP::UI);
			bgmSlider_.barEntity = ECS::ArcheType::CreateEntity("slider_bar", Vec2{ bookPos.val.x * 1.4f,  bookPos.val.y - UI_HEIGHT }, *entityManager_, ENTITY_GROUP::UI);
			seSlider_.gaugeEntity = ECS::ArcheType::CreateEntity("slider", Vec2{ bookPos.val.x * 1.4f, bookPos.val.y }, *entityManager_, ENTITY_GROUP::UI);
			seSlider_.barEntity = ECS::ArcheType::CreateEntity("slider_bar", Vec2{ bookPos.val.x * 1.4f, bookPos.val.y }, *entityManager_, ENTITY_GROUP::UI);
			bgmSlider_.type = Slider::BGM;
			seSlider_.type = Slider::SE;
			bgmSlider_.volume = MasterSound::Get().getBGMGain();
			seSlider_.volume = MasterSound::Get().getSEGain();
		}
		UIReset();
		point_ = ECS::ArcheType::CreateEntity("point", Vec2{ bookPos.val.x * 0.4f, bookPos.val.y - UI_HEIGHT * 2 }, *entityManager_, ENTITY_GROUP::UI);

		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		entityManager_->update();
		MasterSound::Get().setAllBGMGain(bgmSlider_.volume);
		MasterSound::Get().setAllSEGain(seSlider_.volume);
		bgmSlider_.select();
		seSlider_.select();
		selectStageMove();
		selectStage();
	}
	void StageSelect::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "セレクト画面");
		DrawFormatString(0, 100, 0xffffffff, "%d", score_);
		
	}

	StageSelect::~StageSelect()
	{
		entityManager_->allDestory();
		ResourceManager::GetSound().remove("selectBGM");
	}
	void StageSelect::Slider::select()
	{
		if (isSelect)
		{
			barEntity->getComponent<ECS::Scale>().val = Vec2{ 1.5f,1.5f };
		}
		else
		{
			barEntity->getComponent<ECS::Scale>().val = Vec2{ 1.f,1.f };
		}
	}
}// namespace StageSelect