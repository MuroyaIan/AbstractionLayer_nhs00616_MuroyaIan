//==============================================================================
// Filename: Gfx_PipelineState.h
// Description: パイプラインステート処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>
#include <Tool/Tool_TexLoader.h>

//===== クラス定義 =====

//***** テクスチャ *****
class GfxPipelineState : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx              グラフィック処理の参照先
    /// \param[in] BytecodeVS       VSシェーダのバイナリデータ
    /// \param[in] BytecodePS       PSシェーダのバイナリデータ
    /// \param[in] aLayoutDesc      レイアウト設定の配列
    /// \param[in] pRootSignature   ルートシグネチャのポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxPipelineState(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ ID3DBlob& BytecodeVS,
        /*[in]*/ ID3DBlob& BytecodePS,
        /*[in]*/ std::vector<D3D12_INPUT_ELEMENT_DESC>& aLayoutDesc,
        /*[in]*/ ID3D12RootSignature* pRootSignature);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxPipelineState() noexcept override;

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ GfxMain& gfx) noexcept override;    //バインド処理

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pGfxPipelineState;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pGfxPipelineState     GFXパイプラインステートのポインタ
    /// </summary>
};
