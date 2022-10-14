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
#include <DirectXMath.h>

#if DX_11

#include <d3d11.h>

#elif DX_12

#include <d3d12.h>

#endif // GFX

#ifdef _DEBUG

#include <Initguid.h>
#include <dxgidebug.h>	//DXGIデバッグ出力
#define DX_DEBUG_OUTPUT

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
        /*[in]*/ DirectX::XMFLOAT4X4 mtxProj) noexcept        //投影行列へのアクセス
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
        /*[in]*/ const bool bEnable) noexcept                    //ImGui描画ON/OFF
    {
        m_bDrawImGui = bEnable;
    }

    //--------------------------------------------------------------------------
    /// ImGui描画状態確認
    ///
    /// \return bool型変数(描画中かどうか)
    //--------------------------------------------------------------------------
    bool IsImGuiEnabled() const noexcept                            //ImGui描画状態確認
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
public:

    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 uv;
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

    //プロトタイプ宣言
    GfxMain(HWND hWindow, float fWidth, float fHeight);
    ~GfxMain() noexcept;
    void BeginFrame(float R, float G, float B) noexcept;					//フレーム開始
    void DrawIndexed(UINT IndexNum) noexcept;								//フレームバッファ書込み
    void DrawInstanced(UINT IndexNum, UINT InstanceNum) const noexcept;		//インスタンシング描画
    void EndFrame();														//フレーム終了⇒描画開始
    void SetDrawMode(DRAW_MODE Mode);										//描画モード設定

    void SetViewMtx(DirectX::XMFLOAT4X4 mtxView) noexcept			//ビュー行列へのアクセス
    {
        m_mtxView = mtxView;
    }
    DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
    {
        return m_mtxView;
    }
    void SetProjectionMtx(DirectX::XMFLOAT4X4 mtxProj) noexcept		//投影行列へのアクセス
    {
        m_mtxProjection = mtxProj;
    }
    DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
    {
        return m_mtxProjection;
    }

#ifdef IMGUI

    void SetImGuiMode(const bool bEnable) noexcept					//ImGui描画ON/OFF
    {
        m_bDrawImGui = bEnable;
    }

    bool IsImGuiEnabled() const noexcept							//ImGui描画状態確認
    {
        return m_bDrawImGui;
    }

#endif // IMGUI

private:

    //変数宣言
    Microsoft::WRL::ComPtr<IDXGIFactory6> m_pFactory;						//ファクトリ
    Microsoft::WRL::ComPtr<ID3D12Device> m_pDevice;							//デバイス
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCmdAllocator;			//コマンドアロケータ
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCmdList;			//コマンドリスト
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pCmdQueue;					//コマンドキュー
    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_pSwapChain;					//スワップチェーン
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_pBackBuffers;		//バックバッファのポインタ配列
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapsRTV;				//RTV用ヒープ
    UINT m_RtvIncSize;														//RTV用ヒープのインクリメントサイズ
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pDepthBuffer;					//深度バッファ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapDSV;				//DSV用ヒープ
    Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;							//フェンス
    UINT64 m_FenceVal;														//フェンスの実行値
    HANDLE m_FenceEvent;													//フェンス用イベント
    D3D12_RESOURCE_BARRIER m_ResourceBarrier;								//リソースバリア

    DirectX::XMFLOAT4X4 m_mtxView;			//ビュー行列（カメラ）
    DirectX::XMFLOAT4X4 m_mtxProjection;	//投影行列


    Microsoft::WRL::ComPtr<ID3D12Resource> m_pVertexBuffer;					//頂点バッファ
    D3D12_VERTEX_BUFFER_VIEW m_ViewVB;										//頂点バッファビュー
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pIndexBuffer;					//インデックスバッファ
    D3D12_INDEX_BUFFER_VIEW m_ViewIB;										//インデックスバッファビュー
    Microsoft::WRL::ComPtr<ID3DBlob> m_pBlobVS;								//VSバイナリ
    Microsoft::WRL::ComPtr<ID3DBlob> m_pBlobPS;								//PSバイナリ
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_pLayouts;						//インプットレイアウト
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature;			//ルールシグネチャ
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pGfxPipelineState;		//GFXパイプラインステート
    D3D12_VIEWPORT m_Viewport;
    D3D12_RECT m_ScissorRect;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_pTextureBuffer;				//テクスチャバッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pConstBuffer;					//定数バッファ（変換行列）
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pBufferHeaps;			//バッファ用ヒープ
    std::vector<DirectX::XMMATRIX> m_aMatrix;								//変換行列

    //プロトタイプ宣言
    void BindVertexBuffer();
    void BindIndexBuffer();
    void BindVS();
    void BindPS();
    void BindLayout();
    void BindRootSignature();
    void BindPipelineState();

    void BindBufferHeaps();
    void BindTextureBuffer();
    void BindConstBuffer();
    std::vector<Vertex> MakeBox();

#ifdef IMGUI

    bool m_bDrawImGui;						//ImGUI描画制御

#endif // IMGUI

    //権限指定
    friend class GfxBinder;
};

#endif // GFX
