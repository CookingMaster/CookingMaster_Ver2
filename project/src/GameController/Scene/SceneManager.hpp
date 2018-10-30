/**
* @file SceneManager.hpp
* @brief シーン構築に必要な基底クラスや列挙型です
* @author tonarinohito
* @date 2018/10/14
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Utility.hpp"
#include "../Scene/Parameter.hpp"

namespace Scene
{
	/**
	* @briefシーンの状態を表します
	* - BACK_TO_SCENEを指定した場合前のシーンに戻ります。その場合は第三引数をtrueにして下さい
	* - 
	*/
	enum class SceneName
	{
		TITLE,
		GAME,
		PAUSE,
		RESULT,
		BACK_TO_SCENE	//前のスタック(シーン)が残っていれば戻る
	};

	/**
	* @briefシーンの消去フラグです
	* -
	*/
	enum class StackPopFlag
	{
		POP,		//現在のシーンをポップします
		NON,		//現在のシーンをスタック
		ALL_CLEAR	//すべてのスタックをクリアします
	};
	//!シーン変更時のコールバックです
	class IOnSceneChangeCallback
	{
	public:
		IOnSceneChangeCallback() = default;
		virtual ~IOnSceneChangeCallback() = default;
		/*!
		* @brief シーン変更(各シーンからコールバックされる)
		* @param scene 変更するシーンのenum
		* @param parame 次のシーンに渡したい値。不要ならnullptrを指定します
		* @param stackClear 現在のシーンのスタックをクリアするか
		*/
		virtual void onSceneChange(const SceneName& scene, const Parameter* parame, const StackPopFlag stackClear) = 0;
		//!スタックオールクリア
		virtual void stackClear() = 0;
	};

	//!シーンの基底クラスです
	class AbstractScene
	{
	public:
		AbstractScene(IOnSceneChangeCallback* sceneCallback)
		{
			callBack = sceneCallback;
		}
		virtual ~AbstractScene() = default;
		virtual void update() = 0;
		virtual void draw() = 0;
		IOnSceneChangeCallback& getCallBack() const { return *callBack; }
	private:
		IOnSceneChangeCallback* callBack;
	};
}