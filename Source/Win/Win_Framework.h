//==============================================================================
// Filename: Win_Framework.h
// Description: プロジェクト専用のインクルードファイル
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====

// Windows プラットフォーム定義 ヘッダファイル
#include <targetver.h>

// 使用しない機能の制限
#define WIN32_LEAN_AND_MEAN     // Windows ヘッダからほとんど使用されていない部分を除外する
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

// Windows ヘッダファイル
#include <windows.h>

// C ランタイム ヘッダファイル
#include <cstdlib>
#include <malloc.h>
#include <tchar.h>
#include <memory>           // スマートポインタ

// 配列系ヘッダファイル
#include <array>            // 固定配列
#include <vector>           // 動的配列
#include <list>             // リスト
#include <map>              // 連想配列
#include <algorithm>

// 文字列系ヘッダファイル
#include <string>           // 文字列
#include <iostream>         // 文字列入出力
#include <sstream>          // 文字列ストリーム

//===== 定数・マクロ定義 =====
//#define DIRECTX_11                  //dx11
//#define DIRECTX_12                  //dx12
#define IMGUI                       //ImGui使用
constexpr bool IS_DEBUG = true;     //デバッグモード
