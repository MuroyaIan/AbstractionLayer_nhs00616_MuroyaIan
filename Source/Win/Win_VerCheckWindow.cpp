//==============================================================================
// Filename: Win_GfxCheckWindow.cpp
// Description: GfxApi確認
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_VerCheckWindow.h>
#include <Resource.h>

//===== 静的メンバ =====
HWND WinCheckGfx::m_hWindow{};
WinCheckGfx::GFX_ID WinCheckGfx::m_GfxID = GFX_ID::GFX_NONE;

//===== クラス実装 =====
WinCheckGfx::WinCheckGfx() : m_hAppInst(GetModuleHandle(nullptr))
{
    //ウィンドウクラス作成
    WNDCLASS winc;
    winc.style = CS_HREDRAW | CS_VREDRAW;
    winc.lpfnWndProc = WndProc;
    winc.cbClsExtra = 0;
    winc.cbWndExtra = 0;
    winc.hInstance = m_hAppInst;
    winc.hIcon = LoadIcon(m_hAppInst,
        MAKEINTRESOURCE(IDI_ABSTRACTIONLAYERNHS00616MUROYAIAN));    //アイコン
    winc.hCursor = LoadCursor(m_hAppInst, IDC_ARROW);               //カーソル
    winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    winc.lpszMenuName = nullptr;
    winc.lpszClassName = CLASS_NAME;
    if (!RegisterClass(&winc)) {

#ifdef _DEBUG

        PrintD(L"Error : ウィンドウクラス作成失敗\n");

#endif // _DEBUG

        throw - 1;
    }

    //ウィンドウ作成
    m_hWindow = CreateWindow(
        CLASS_NAME, L"グラフィックス初期設定",
        WS_CAPTION | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 200, 200,
        nullptr, nullptr, m_hAppInst, nullptr
    );
    if (!m_hWindow) {

#ifdef _DEBUG

        PrintD(L"Error : ウィンドウ作成失敗\n");

#endif // _DEBUG

        throw - 1;
    }
}

WinCheckGfx::~WinCheckGfx() noexcept(false)
{
    //クラス登録を解除
    if (!UnregisterClass(CLASS_NAME, m_hAppInst)) {

#ifdef _DEBUG

        PrintD(L"Error : クラス登録の解除が失敗\n");

#endif // _DEBUG

        throw -1;
    }
}

int WinCheckGfx::Update()
{
    //処理ループ
    MSG msg;
    while (GetMessage(&msg, nullptr, 0u, 0u))
        DispatchMessage(&msg);

    //ウィンドウを破棄
    if (!DestroyWindow(m_hWindow)) {

#ifdef _DEBUG

        PrintD(L"Error : ウィンドウの破棄が失敗\n");

#endif // _DEBUG

        throw - 1;
    }

    //確認終了
    return static_cast<int>(msg.wParam);
}
