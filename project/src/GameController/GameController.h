/**
 * @file GameController.hpp
 * @brief アプリケーションの処理を行います
 * @author tonarinohito
 * @date 2018/10/06
 * @par History
 - 2018/10/14 tonarinohito
 -# このクラスでシーンのスタックを監視するように変更
 */
#pragma once
#include "../ECS/ECS.hpp"
#include "Scene/SceneManager.hpp"
#include "Scene/Parameter.hpp"
#include <stack>
#define ENTITY_GROUP (ECS::Group)GameController::GameGroup
#define IS_AUTO_PLAY true
class GameController final : public Scene::IOnSceneChangeCallback
{
private:
	ECS::EntityManager entityManager_;
	std::stack<std::unique_ptr<Scene::AbstractScene>> sceneStack;	//シーンのスタック
	void resourceLoad();
public:

	/**
	*  @brief エンティティ属するのグループです
	* - 描画レイヤー、処理順、エンティティの取得に使います
	* - OrderByDraw使用時、番号が大きいほど手前に描画されます
	*/
	enum class GameGroup : ECS::Group
	{
		LAYER1,			//テスト用
		BACK,			//最背面背景
		BACK_OBJECT,	//背景の鍋や皿などの調理器具
		BACK_STAFF,		//後ろで働く従業員
		MASTER,			//おやっさん
		KITCHENWARE,	//調理台に上に載ったオブジェクト
		COOKING_AREA,	//調理台や床
		GIRL,			//女の子
		RECEIVE_STAFF,	//食材を受け取るスタッフ
		NOTE,			//音符(食材)
		EFFECT,			//演出用エフェクト
		UI,				//時計やスコアゲージ等のUI
		FADE,			//フェード用
		PAUSE_UI,		//ポーズ用UI
		TITLE_BG,		//タイトル画面背景
		TITLE_MESSAGE,	//タイトル画面の「PRESS ANY KEY」
		TITLE_LOGO,		//タイトルロゴ
		TOP_FADE,		//最前面フェード
		MAX,			//最大数-----------------------------------------ここより上に記述
	};
	GameController();
	~GameController() = default;
	/*!
	* @brief シーン変更(各シーンからコールバックされる)
	* @param parame 次のシーンに渡したい値。不要ならnullptrを指定します
	* @param scene 変更するシーンのenum
	* @param stackClear 現在のシーンのスタックをクリアするか
	*/
	void onSceneChange(const Scene::SceneName& scene, Parameter* parame, const Scene::StackPopFlag stackClear, const bool isInitialize) override;
	//!すべてのシーンスタックをクリアします
	void stackClear() override;
	//!Entityの更新処理を行います
	void update();
	//!Entityの描画を行います
	void draw();
};