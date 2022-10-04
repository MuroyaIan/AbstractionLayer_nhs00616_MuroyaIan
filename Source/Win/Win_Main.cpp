//==============================================================================
// Filename: Win_Main.cpp
// Description: メインファイル（エントリポイント）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

// インクルード部
#include <Win/Win_Framework.h>
#include <Win/Win_Main.h>
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
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   //メモリリーク検出

        //アプリケーション実行
        return APP_64{}.Run();
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
