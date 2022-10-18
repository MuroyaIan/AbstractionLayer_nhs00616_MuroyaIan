//==============================================================================
// Filename: Gfx_Main.cpp
// Description: グラフィック処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Gfx_Main.h>

#ifdef IMGUI
#
#   include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#   include <3rd_Party/ImGui/backends/imgui_impl_dx11.h>
#
#endif // IMGUI

//===== 追加ライブラリ =====
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dCompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//===== 静的メンバ変数 =====
GfxMain::API_MODE GfxMain::m_Api = GfxMain::API_MODE::aDX_11;    //GfxApiモード

//===== クラス実装 =====
GfxMain::GfxMain(WinWindow& Window) :
    m_Window(Window), m_pFactory(), m_pAdapter(), m_DrawMode(DRAW_MODE::DRAW_3D), m_mtxView(), m_mtxProjection()
{
    //エラーハンドル
    HRESULT hr{};

    //ファクトリ初期化
    wrl::ComPtr<IDXGIFactory> pFactory;

#ifdef _DEBUG

    hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&pFactory));    //デバッグ情報(DXGI)
    ERROR_DX(hr);

#else

    hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
    ERROR_DX(hr);

#endif // _DEBUG

    hr = pFactory->QueryInterface(IID_PPV_ARGS(&m_pFactory));
    ERROR_DX(hr);

    //GPUデバイス指定(高パフォーマンス)
    hr = m_pFactory->EnumAdapterByGpuPreference(0u, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_pAdapter));
    ERROR_DX(hr);

#ifdef _DEBUG

    //GPU情報取得
    IDXGIAdapter* pTmpAdapter = nullptr;
    for (size_t i = 0; ; i++) {

        //アダプターのポインタを取得
        hr = m_pFactory->EnumAdapters(static_cast<UINT>(i), &pTmpAdapter);
        if (hr == DXGI_ERROR_NOT_FOUND)
            break;

        //デバイス名出力
        DXGI_ADAPTER_DESC dad{};
        hr = pTmpAdapter->GetDesc(&dad);
        ERROR_DX(hr);
        std::wostringstream oss;
        oss << "Info : GPU(" << i << ") " << dad.Description << std::endl;
        PrintD(oss.str().c_str());

        //メモリ解放
        pTmpAdapter->Release();
        pTmpAdapter = nullptr;
    }

#endif // _DEBUG

    //行列初期化
    dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixIdentity());
    dx::XMStoreFloat4x4(&m_mtxProjection, dx::XMMatrixIdentity());
}

GfxMain::~GfxMain() noexcept
{
}

//描画モード設定
void GfxMain::SetDrawMode(DRAW_MODE Mode) noexcept
{
    m_DrawMode = Mode;
}
