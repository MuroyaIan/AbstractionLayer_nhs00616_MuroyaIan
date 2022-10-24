//==============================================================================
// Filename: Gfx_Inputlayout.h
// Description: 入力レイアウト処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>

//===== クラス定義 =====

//***** 入力レイアウト *****
class GfxInputLayout : public GfxBinder
{
public:

    //レイアウト情報
    struct LayoutInfo
    {
        //----------------------------------------------------------------------
        LPCSTR semanticName;
        UINT semanticIndex;
        DXGI_FORMAT format;
        UINT inputSlot;
        UINT alignedByteOffset;
        bool isPerInstanceData;
        UINT instanceDataStepRate;
        //----------------------------------------------------------------------

        /// <summary>
        /// semanticName            セマンティック名
        /// semanticIndex           セマンティックインデックス
        /// format                  レイアウトフォーマット
        /// inputSlot               入力スロット
        /// alignedByteOffset       バイトオフセット
        /// isPerVertexData         頂点ごとの情報かどうか
        /// instanceDataStepRate    インスタンス情報の使用レート
        /// </summary>
    };

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx                      グラフィック処理の参照先
    /// \param[in] layout                   レイアウト情報
    /// \param[in] pVertexShaderBytecode    頂点シェーダの情報
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxInputLayout(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<LayoutInfo>& layout,
        /*[in]*/ ID3DBlob* pVertexShaderBytecode);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxInputLayout() noexcept override;

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ GfxMain& gfx) noexcept override;

    //--------------------------------------------------------------------------
    /// レイアウト取得
    ///
    /// \return レイアウト配列の参照
    //--------------------------------------------------------------------------
    std::vector<D3D12_INPUT_ELEMENT_DESC>& GetLayout() noexcept
    {
        return m_aLayoutDesc;
    }

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    /// レイアウト作成
    ///
    /// \param[in] gfx                      グラフィック処理の参照先
    /// \param[in] layout                   レイアウト情報
    /// \param[in] pVertexShaderBytecode    頂点シェーダの情報
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateLayoutDX11(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<LayoutInfo>& layout,
        /*[in]*/ ID3DBlob* pVertexShaderBytecode);

    //--------------------------------------------------------------------------
    /// レイアウト作成
    ///
    /// \param[in] layout                   レイアウト情報
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateLayoutDX12(
        /*[in]*/ const std::vector<LayoutInfo>& layout);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_aLayoutDesc;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pInputLayout  レイアウトオブジェクトのポインタ(dx11)
    /// m_aLayoutDesc   レイアウト情報配列(dx12)
    /// </summary>
};
