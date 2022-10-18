//==============================================================================
// Filename: Win_Main.cpp
// Description: メインファイル（エントリポイント）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

// インクルード部
#include <Win/Win_Main.h>
#include <Win/Win_VerCheckWindow.h>
#include <Win/Win_App.h>

//エントリーポイント
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    try
    {
        //前処理
        UNREFERENCED_PARAMETER(hInstance);
        UNREFERENCED_PARAMETER(hPrevInstance);
        UNREFERENCED_PARAMETER(lpCmdLine);
        UNREFERENCED_PARAMETER(nCmdShow);

#ifdef _DEBUG

        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   //メモリリーク検出

#endif // _DEBUG

        //グラフィックスAPI設定
        WinCheckGfx Window;
        int wParam = Window.Update();
        if (wParam != 0 || WinCheckGfx::m_GfxID != WinCheckGfx::GFX_ID::aDX_11)
            return wParam;

        //アプリケーション実行
        wParam = App64{}.Run();

#ifdef _DEBUG

        //エラー処理
        IDXGIDebug1* pDebugDxgi;
        if (SUCCEEDED(DXGIGetDebugInterface1(0u, IID_PPV_ARGS(&pDebugDxgi)))) {
            pDebugDxgi->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
            pDebugDxgi->Release();
        }

#endif // _DEBUG

        //終了処理
        return wParam;
    }
    catch (const WinErrorProc& e)
    {
        //Windowエラー
        MessageBoxA(nullptr, e.what(), e.GetErrorType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        //標準エラー
        MessageBoxA(nullptr, e.what(), "標準的例外（エラー）", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        //未定義エラー
        MessageBoxA(nullptr, "詳細不明です。", "不明のエラー", MB_OK | MB_ICONEXCLAMATION);
    }

    //異常終了
    return -1;
}
