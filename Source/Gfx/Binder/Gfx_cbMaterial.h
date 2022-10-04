//==============================================================================
// Filename: Gfx_cbMaterial.h
// Description: マテリアル情報用定数バッファ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_ConstantBuffer.h>

//===== 前方宣言 =====
struct MATERIAL_DATA;

//===== クラス定義 =====

//***** マテリアルバッファ *****
class CB_MATERIAL : public GfxBinder
{
public:

    //プロトタイプ宣言
    CB_MATERIAL(GfxMain& Gfx, const MATERIAL_DATA& Material);
    ~CB_MATERIAL() noexcept override;
    void Bind(GfxMain& Gfx) noexcept override;                            //バインド処理

private:

    //変数宣言
    static std::unique_ptr<GfxPixelCBuffer<MATERIAL_DATA>> m_pPcBuff;        //定数バッファのポインタ
    const MATERIAL_DATA& m_Material;                                    //マテリアル情報の提供先
};
