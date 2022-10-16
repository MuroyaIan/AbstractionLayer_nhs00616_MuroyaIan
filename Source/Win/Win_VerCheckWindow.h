//==============================================================================
// Filename: Win_GfxCheckWindow.h
// Description: GfxApi確認
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Win/Win_Framework.h>

//===== クラス定義 =====
class WinCheckGfx
{
public:

	enum class GFX_ID
	{
		GFX_NONE,
		aDX_11,
		aDX_12,
		OPEN_GL,
		VULKAN
	};

	WinCheckGfx();
	~WinCheckGfx() noexcept(false);
	int Update();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		//ハンドル初期化
		static HFONT hFont = CreateFont(
			20, 0, 0, 0, 0, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			VARIABLE_PITCH | FF_DONTCARE, nullptr
		);
		static HWND hBtnRadio1;
		static HWND hBtnRadio2;
		static HWND hBtnRadio3;
		static HWND hBtnRadio4;

		//メッセージ処理
		switch (msg) {

			//終了処理
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;

			//生成処理
			case WM_CREATE:
			{
				//ラジオボタン作成
				HINSTANCE hAppInst = reinterpret_cast<LPCREATESTRUCT>(lp)->hInstance;
				hBtnRadio1 = CreateWindow(
					L"BUTTON", L"DirectX 11",
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					10, 0, 150, 30,
					hwnd, nullptr, hAppInst, nullptr
				);
				hBtnRadio2 = CreateWindow(
					L"BUTTON", L"DirectX 12",
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					10, 30, 150, 30,
					hwnd, nullptr, hAppInst, nullptr
				);
				hBtnRadio3 = CreateWindow(
					L"BUTTON", L"OpenGL",
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					10, 60, 150, 30,
					hwnd, nullptr, hAppInst, nullptr
				);
				hBtnRadio4 = CreateWindow(
					L"BUTTON", L"Vulkan",
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					10, 90, 150, 30,
					hwnd, nullptr, hAppInst, nullptr
				);

				//確定ボタン作成
				CreateWindow(
					L"BUTTON", L"確定",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					40, 120, 100, 30,
					hwnd, (HMENU)BTN_ID_CLICK, hAppInst, nullptr
				);
				return 0;
			}

			//RADIOボタン用設定（毎フレーム）
			case WM_CTLCOLORSTATIC:
			{
				//文字色・フォントを設定
				HDC hdcStatic = reinterpret_cast<HDC>(wp);
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SelectObject(hdcStatic, hFont);

				//ボタンそのものの背景色を変更
				return reinterpret_cast<LRESULT>(CreateSolidBrush(RGB(255, 255, 255)));
			}

			//コマンド処理
			case WM_COMMAND:

				//選択確認
				if (SendMessage(hBtnRadio1, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					m_GfxID = GFX_ID::aDX_11;
				if (SendMessage(hBtnRadio2, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					m_GfxID = GFX_ID::aDX_12;
				if (SendMessage(hBtnRadio3, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					m_GfxID = GFX_ID::OPEN_GL;
				if (SendMessage(hBtnRadio4, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					m_GfxID = GFX_ID::VULKAN;

				//クリック処理
				switch (LOWORD(wp)) {
					case BTN_ID_CLICK:
						PostMessage(m_hWindow, WM_CLOSE, NULL, NULL);
						break;
				}
				return 0;
		}
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	static GFX_ID m_GfxID;

private:

	//変数宣言
	static constexpr const LPCWSTR CLASS_NAME = L"WindowClass_GfxApiCheck";
	HINSTANCE m_hAppInst;
	static HWND m_hWindow;

	static constexpr long long BTN_ID_CLICK = 1;	//確定ボタンのID
};
