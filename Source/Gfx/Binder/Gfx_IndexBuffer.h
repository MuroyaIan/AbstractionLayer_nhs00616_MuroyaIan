//==============================================================================
// Filename: Gfx_IndexBuffer.h
// Description: インデックスバッファ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>

//===== クラス定義 =====

//***** インデックスバッファ *****
class GfxIndexBuffer : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] indices  インデックスバッファ用データ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxIndexBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<unsigned short>& indices);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxIndexBuffer() noexcept override;

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
    /// インデックス数取得
    ///
    /// \return インデックス数
    //--------------------------------------------------------------------------
    UINT GetIndexNum() const noexcept
    {
        return m_count;
    }

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------
    UINT m_count;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_count         インデックス数
    /// m_pIndexBuffer  バッファのポインタ
    /// </summary>
};
