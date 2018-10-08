/**
 * @file System.hpp
 * @brief Dxlibの初期化とシステムのチェックを行います
 * @author tonarinohito
 * @date 2018/10/05
 */
#pragma once
#include <DxLib.h>
#include <assert.h>
#include <Windows.h>

/*!
@brief DXlibの処理を隠蔽します
*/
class System final
{
private:
	void systemInit()
	{
		//ウィンドウがノンアクティブ時は実行しない
		SetAlwaysRunFlag(false);
		//ログ消し
		SetOutApplicationLogValidFlag(false);
		//縦横比維持
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
		auto IsFullScreen = []()
		{
			int flag;
			flag = MessageBox(
				NULL,
				TEXT("フルスクリーンモードで起動しますか？"),
				TEXT("スクリーン設定"),
				MB_YESNO | MB_ICONQUESTION);
			if (flag == IDNO)
			{
				return true;
			}
			return false;
		};
		//ウィンドウモード
		ChangeWindowMode(IsFullScreen());
		//XAudio2を使用する
		SetEnableXAudioFlag(true);
		//ウインドウタイトルを変更
		SetMainWindowText("Game");
		//画面サイズ変更
		SetGraphMode(SCREEN_WIDIH, SCREEN_HEIGHT, 32);
		//ウィンドウモード変更
		//初期化
		DxLib_Init();
		assert(DxLib_IsInit());
		//裏画面設定
		SetDrawScreen(DX_SCREEN_BACK);
	}
	const bool processLoop() const
	{
		if (ScreenFlip() != 0) return false;
		if (ProcessMessage() != 0) return false;
		if (ClearDrawScreen() != 0) return false;
		return true;
	}
public:
	//!画面サイズ
	static constexpr int
	SCREEN_WIDIH = 1248,
	SCREEN_HEIGHT = 720;
	System()
	{
		systemInit();
	}
	~System()
	{
		DxLib_End();
	}
	//!@brief Dxlibの更新処理を行います
	[[nodiscard]] const bool isOk() const
	{
		return processLoop();
	}
};