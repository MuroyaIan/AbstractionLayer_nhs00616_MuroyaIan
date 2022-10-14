//==============================================================================
// Filename: Gfx_Main.h
// Description: グラフィック処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Win/Win_Window.h>
#include <wrl.h>                    //COMポインタ
#include <dxgi1_6.h>
#include <d3dcompiler.h>            //シェーダ読込
#include <d3d11.h>
#include <d3d12.h>
#include <DirectXMath.h>

#ifdef _DEBUG

#include <Initguid.h>
#include <dxgidebug.h>    //DXGIデバッグ出力

#endif // _DEBUG

//===== 定数・マクロ定義 =====
#define ERROR_DX(hr) if(hr != S_OK) throw ERROR_EX(hr)    //DirectXエラー出力用

//===== クラス定義 =====

#if DX_11

//***** グラフィック *****
class GfxMain
{

    //--------------------------------------------------------------------------
    friend class GfxBinder;
    //--------------------------------------------------------------------------

public:

    //描画モード
    enum class DrawMode
    {
        DRAW_2D,
        DRAW_3D
    };

    //コピーNG
    GfxMain(const GfxMain&) = delete;
    GfxMain& operator=(const GfxMain&) = delete;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] hWindow  ウィンドウハンドル
    /// \param[in] fWidth   ウィンドウ幅
    /// \param[in] fHeight  ウィンドウ高さ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxMain(
        /*[in]*/ HWND hWindow,
        /*[in]*/ float fWidth,
        /*[in]*/ float fHeight);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxMain() noexcept;

    //--------------------------------------------------------------------------
    /// フレーム開始
    ///
    /// \param[in] r    色のrチャンネル
    /// \param[in] g    色のgチャンネル
    /// \param[in] b    色のbチャンネル
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BeginFrame(
        /*[in]*/ float r,
        /*[in]*/ float g,
        /*[in]*/ float b) const noexcept;

    //--------------------------------------------------------------------------
    /// フレームバッファ書込み
    ///
    /// \param[in] indexNum     描画する頂点の要素数
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void DrawIndexed(
        /*[in]*/ UINT indexNum) const noexcept(!IS_DEBUG);

    //--------------------------------------------------------------------------
    /// インスタンシング描画
    ///
    /// \param[in] indexNum     描画する頂点の要素数
    /// \param[in] instanceNum  描画するインスタンス数
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void DrawInstanced(
        /*[in]*/ UINT indexNum,
        /*[in]*/ UINT instanceNum) const noexcept(!IS_DEBUG);

    //--------------------------------------------------------------------------
    /// フレーム終了⇒描画開始
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void EndFrame() const;

    //--------------------------------------------------------------------------
    /// 描画モード設定
    ///
    /// \param[in] mode     指定する描画モード
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetDrawMode(
        /*[in]*/ DrawMode mode);

    //--------------------------------------------------------------------------
    /// ビュー行列設定
    ///
    /// \param[in] mtxView  指定するビュー行列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetViewMtx(
        /*[in]*/ DirectX::XMFLOAT4X4 mtxView) noexcept
    {
        m_mtxView = mtxView;
    }

    //--------------------------------------------------------------------------
    /// ビュー行列取得
    ///
    /// \return ビュー行列
    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
    {
        return m_mtxView;
    }

    //--------------------------------------------------------------------------
    /// 投影行列設定
    ///
    /// \param[in] mtxProj  指定する投影行列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetProjectionMtx(
        /*[in]*/ DirectX::XMFLOAT4X4 mtxProj) noexcept
    {
        m_mtxProjection = mtxProj;
    }

    //--------------------------------------------------------------------------
    /// 投影行列取得
    ///
    /// \return 投影行列
    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
    {
        return m_mtxProjection;
    }

#ifdef IMGUI

    //--------------------------------------------------------------------------
    /// ImGui描画ON/OFF
    ///
    /// \param[in] bEnable  描画切替フラグ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetImGuiMode(
        /*[in]*/ const bool bEnable) noexcept
    {
        m_bDrawImGui = bEnable;
    }

    //--------------------------------------------------------------------------
    /// ImGui描画状態確認
    ///
    /// \return bool型変数(描画中かどうか)
    //--------------------------------------------------------------------------
    bool IsImGuiEnabled() const noexcept
    {
        return m_bDrawImGui;
    }

#endif // IMGUI

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSView;
    DirectX::XMFLOAT4X4 m_mtxView;
    DirectX::XMFLOAT4X4 m_mtxProjection;

#ifdef IMGUI

    bool m_bDrawImGui;

#endif // IMGUI

    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pDevice           デバイス
    /// m_pSwapChain        スワップチェーン
    /// m_pContext          コンテキスト
    /// m_pTargetView       ターゲットビュー
    /// m_pDSView           深度・ステンシルビュー
    /// m_mtxView           ビュー行列（カメラ）
    /// m_mtxProjection     投影行列
    /// m_bDrawImGui        ImGUI描画制御
    /// </summary>
};

#elif DX_12

//***** グラフィック *****
class GfxMain
{
    //--------------------------------------------------------------------------
    friend class GfxBinder;
    //--------------------------------------------------------------------------

public:

    //頂点情報用構造体（仮置き）
    struct Vertex
    {
        //----------------------------------------------------------------------
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 uv;
        //----------------------------------------------------------------------

        /// <summary>
        /// pos     頂点座標
        /// uv      UV座標
        /// </summary>
    };

    //描画モード
    enum class DRAW_MODE
    {
        DRAW_2D,
        DRAW_3D
    };

    //コピーNG
    GfxMain(const GfxMain&) = delete;
    GfxMain& operator=(const GfxMain&) = delete;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] hWindow  ウィンドウハンドル
    /// \param[in] fWidth   ウィンドウ幅
    /// \param[in] fHeight  ウィンドウ高さ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxMain(
        /*[in]*/ HWND hWindow,
        /*[in]*/ float fWidth,
        /*[in]*/ float fHeight);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxMain() noexcept;

    //--------------------------------------------------------------------------
    /// フレーム開始
    ///
    /// \param[in] r    色のrチャンネル
    /// \param[in] g    色のgチャンネル
    /// \param[in] b    色のbチャンネル
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BeginFrame(
        /*[in]*/ float r,
        /*[in]*/ float g,
        /*[in]*/ float b) noexcept;

    //--------------------------------------------------------------------------
    /// インスタンシング描画
    ///
    /// \param[in] indexNum     描画する頂点の要素数
    /// \param[in] instanceNum  描画するインスタンス数
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void DrawInstanced(
        /*[in]*/ UINT indexNum,
        /*[in]*/ UINT instanceNum) noexcept(!IS_DEBUG);

    //--------------------------------------------------------------------------
    /// フレーム終了⇒描画開始
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void EndFrame();

    //--------------------------------------------------------------------------
    /// ビュー行列設定
    ///
    /// \param[in] mtxView  指定するビュー行列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetViewMtx(
        /*[in]*/ DirectX::XMFLOAT4X4 mtxView) noexcept
    {
        m_mtxView = mtxView;
    }

    //--------------------------------------------------------------------------
    /// ビュー行列取得
    ///
    /// \return ビュー行列
    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
    {
        return m_mtxView;
    }

    //--------------------------------------------------------------------------
    /// 投影行列設定
    ///
    /// \param[in] mtxProj  指定する投影行列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetProjectionMtx(
        /*[in]*/ DirectX::XMFLOAT4X4 mtxProj) noexcept
    {
        m_mtxProjection = mtxProj;
    }

    //--------------------------------------------------------------------------
    /// 投影行列取得
    ///
    /// \return 投影行列
    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
    {
        return m_mtxProjection;
    }

#ifdef IMGUI

    //--------------------------------------------------------------------------
    /// ImGui描画ON/OFF
    ///
    /// \param[in] bEnable  描画切替フラグ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetImGuiMode(
        /*[in]*/ const bool bEnable) noexcept
    {
        m_bDrawImGui = bEnable;
    }

    //--------------------------------------------------------------------------
    /// ImGui描画状態確認
    ///
    /// \return bool型変数(描画中かどうか)
    //--------------------------------------------------------------------------
    bool IsImGuiEnabled() const noexcept
    {
        return m_bDrawImGui;
    }

#endif // IMGUI

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// 頂点バッファ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindVertexBuffer();

    //--------------------------------------------------------------------------
    /// インデックスバッファ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindIndexBuffer();

    //--------------------------------------------------------------------------
    /// 頂点シェーダ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindVS();

    //--------------------------------------------------------------------------
    /// ピクセルシェーダ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindPS();

    //--------------------------------------------------------------------------
    /// レイアウト設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindLayout();

    //--------------------------------------------------------------------------
    /// ルールシグネチャ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindRootSignature();

    //--------------------------------------------------------------------------
    /// パイプラインステート設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindPipelineState();

    //--------------------------------------------------------------------------
    /// バッファヒープ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindBufferHeaps();

    //--------------------------------------------------------------------------
    /// テクスチャバッファ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindTextureBuffer();

    //--------------------------------------------------------------------------
    /// 定数バッファ設定
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindConstBuffer();

    //--------------------------------------------------------------------------
    /// 頂点情報作成（キューブ）
    ///
    /// \return 頂点情報の配列
    //--------------------------------------------------------------------------
    std::vector<Vertex> MakeBox();

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#ifdef IMGUI

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapImgui;
    bool m_bDrawImGui;

#endif // IMGUI

    Microsoft::WRL::ComPtr<IDXGIFactory6> m_pFactory;
    Microsoft::WRL::ComPtr<ID3D12Device> m_pDevice;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCmdAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCmdList;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pCmdQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_pSwapChain;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_pBackBuffers;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapsRTV;
    UINT m_RtvIncSize;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pDepthBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapDSV;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;
    UINT64 m_FenceVal;
    HANDLE m_FenceEvent;
    D3D12_RESOURCE_BARRIER m_ResourceBarrier;
    DirectX::XMFLOAT4X4 m_mtxView;
    DirectX::XMFLOAT4X4 m_mtxProjection;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_ViewVB;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pIndexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_ViewIB;
    Microsoft::WRL::ComPtr<ID3DBlob> m_pBlobVS;
    Microsoft::WRL::ComPtr<ID3DBlob> m_pBlobPS;
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_pLayouts;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pGfxPipelineState;
    D3D12_VIEWPORT m_Viewport;
    D3D12_RECT m_ScissorRect;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pTextureBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pConstBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pBufferHeaps;
    std::vector<DirectX::XMMATRIX> m_aMatrix;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pHeapImgui            Imgui用ヒープ
    /// m_bDrawImGui            Imgui描画制御
    /// m_pFactory              ファクトリ
    /// m_pDevice               デバイス
    /// m_pCmdAllocator         コマンドアロケータ
    /// m_pCmdList              コマンドリスト
    /// m_pCmdQueue             コマンドキュー
    /// m_pSwapChain            スワップチェーン
    /// m_pBackBuffers          バックバッファのポインタ配列
    /// m_pHeapsRTV             RTV用ヒープ
    /// m_RtvIncSize            RTV用ヒープのインクリメントサイズ
    /// m_pDepthBuffer          深度バッファ
    /// m_pHeapDSV              DSV用ヒープ
    /// m_pFence                フェンス
    /// m_FenceVal              フェンスの実行値
    /// m_FenceEvent            フェンス用イベント
    /// m_ResourceBarrier       リソースバリア
    /// m_mtxView               ビュー行列（カメラ）
    /// m_mtxProjection         投影行列
    /// m_pVertexBuffer         頂点バッファ
    /// m_ViewVB                頂点バッファビュー
    /// m_pIndexBuffer          インデックスバッファ
    /// m_ViewIB                インデックスバッファビュー
    /// m_pBlobVS               VSバイナリ
    /// m_pBlobPS               PSバイナリ
    /// m_pLayouts              インプットレイアウト
    /// m_pRootSignature        ルールシグネチャ
    /// m_pGfxPipelineState     GFXパイプラインステート
    /// m_Viewport              ビューポート
    /// m_ScissorRect           ビューのカット範囲
    /// m_pTextureBuffer        テクスチャバッファ
    /// m_pConstBuffer          定数バッファ（変換行列）
    /// m_pBufferHeaps          バッファ用ヒープ
    /// m_aMatrix               変換行列
    /// </summary>
};

#endif // GFX
