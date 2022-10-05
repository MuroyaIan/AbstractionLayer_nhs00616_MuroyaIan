//==============================================================================
// Filename: Gfx_vsdBox.h
// Description: ボックス作成処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_VertexShaderData.h>

//===== クラス定義 =====

//***** ボックス形状 *****
class VsdBox
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// データ作成
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData()
    {
        //長さ定義
        constexpr float size = 1.0f * 0.5f;     //ボックスの辺の長さ(標準1.0f)

        //頂点作成
        std::vector<V> aData(8);
        aData[0].m_pos = { -size, -size, -size };
        aData[1].m_pos = {  size, -size, -size };
        aData[2].m_pos = { -size,  size, -size };
        aData[3].m_pos = {  size,  size, -size };
        aData[4].m_pos = { -size, -size,  size };
        aData[5].m_pos = {  size, -size,  size };
        aData[6].m_pos = { -size,  size,  size };
        aData[7].m_pos = {  size,  size,  size };

        //インデックス作成
        const std::vector<unsigned short> indices = {
            0, 2, 1,    2, 3, 1,
            1, 3, 5,    3, 7, 5,
            2, 6, 3,    3, 6, 7,
            4, 5, 7,    4, 7, 6,
            0, 4, 2,    2, 4, 6,
            0, 1, 4,    1, 5, 4
        };

        return VsData<V>(std::move(aData), std::move(indices));
    }

    //--------------------------------------------------------------------------
    /// データ作成（テクスチャあり）
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData_Tex()
    {
        //長さ定義
        constexpr float size = 1.0f * 0.5f;     //ボックスの辺の長さ(標準1.0f)

        //頂点作成
        std::vector<V> aData(14);
        aData[0].m_pos  = { -size, -size, -size };
        aData[1].m_pos  = {  size, -size, -size };
        aData[2].m_pos  = { -size,  size, -size };
        aData[3].m_pos  = {  size,  size, -size };
        aData[4].m_pos  = { -size, -size,  size };
        aData[5].m_pos  = {  size, -size,  size };
        aData[6].m_pos  = { -size,  size,  size };
        aData[7].m_pos  = {  size,  size,  size };
        aData[8].m_pos  = { -size, -size, -size };
        aData[9].m_pos  = {  size, -size, -size };
        aData[10].m_pos = { -size, -size, -size };
        aData[11].m_pos = { -size, -size,  size };
        aData[12].m_pos = {  size, -size, -size };
        aData[13].m_pos = {  size, -size,  size };
        aData[0].m_uv  = { 2.0f / 3.0f, 0.0f / 4.0f };
        aData[1].m_uv  = { 1.0f / 3.0f, 0.0f / 4.0f };
        aData[2].m_uv  = { 2.0f / 3.0f, 1.0f / 4.0f };
        aData[3].m_uv  = { 1.0f / 3.0f, 1.0f / 4.0f };
        aData[4].m_uv  = { 2.0f / 3.0f, 3.0f / 4.0f };
        aData[5].m_uv  = { 1.0f / 3.0f, 3.0f / 4.0f };
        aData[6].m_uv  = { 2.0f / 3.0f, 2.0f / 4.0f };
        aData[7].m_uv  = { 1.0f / 3.0f, 2.0f / 4.0f };
        aData[8].m_uv  = { 2.0f / 3.0f, 4.0f / 4.0f };
        aData[9].m_uv  = { 1.0f / 3.0f, 4.0f / 4.0f };
        aData[10].m_uv = { 3.0f / 3.0f, 1.0f / 4.0f };
        aData[11].m_uv = { 3.0f / 3.0f, 2.0f / 4.0f };
        aData[12].m_uv = { 0.0f / 3.0f, 1.0f / 4.0f };
        aData[13].m_uv = { 0.0f / 3.0f, 2.0f / 4.0f };

        //インデックス作成
        const std::vector<unsigned short> indices = {
             0,  2,  1,         2,  3,  1,
             4,  8,  5,         5,  8,  9,
             2,  6,  3,         3,  6,  7,
             4,  5,  7,         4,  7,  6,
             2, 10, 11,         2, 11,  6,
            12,  3,  7,        12,  7, 13
        };

        return VsData<V>(std::move(aData), std::move(indices));
    }

    //--------------------------------------------------------------------------
    /// データ作成（モデル用）
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData_Model()
    {
        //長さ定義
        constexpr float size = 1.0f * 0.5f;     //ボックスの辺の長さ(標準1.0f)

        //頂点作成
        std::vector<V> aData(24);
        aData[0].m_pos  = { -size, -size, -size };
        aData[1].m_pos  = {  size, -size, -size };
        aData[2].m_pos  = { -size,  size, -size };
        aData[3].m_pos  = {  size,  size, -size };      //near
        aData[4].m_pos  = {  size, -size,  size };
        aData[5].m_pos  = { -size, -size,  size };
        aData[6].m_pos  = {  size,  size,  size };
        aData[7].m_pos  = { -size,  size,  size };      //far
        aData[8].m_pos  = { -size, -size,  size };
        aData[9].m_pos  = { -size, -size, -size };
        aData[10].m_pos = { -size,  size,  size };
        aData[11].m_pos = { -size,  size, -size };      //left
        aData[12].m_pos = {  size, -size, -size };
        aData[13].m_pos = {  size, -size,  size };
        aData[14].m_pos = {  size,  size, -size };
        aData[15].m_pos = {  size,  size,  size };      //right
        aData[16].m_pos = { -size, -size,  size };
        aData[17].m_pos = {  size, -size,  size };
        aData[18].m_pos = { -size, -size, -size };
        aData[19].m_pos = {  size, -size, -size };      //bottom
        aData[20].m_pos = { -size,  size, -size };
        aData[21].m_pos = {  size,  size, -size };
        aData[22].m_pos = { -size,  size,  size };
        aData[23].m_pos = {  size,  size,  size };      //top
        aData[0].m_uv  = { 1.0f / 3.0f, 1.0f / 4.0f };
        aData[1].m_uv  = { 2.0f / 3.0f, 1.0f / 4.0f };
        aData[2].m_uv  = { 1.0f / 3.0f, 0.0f / 4.0f };
        aData[3].m_uv  = { 2.0f / 3.0f, 0.0f / 4.0f };  //near
        aData[4].m_uv  = { 1.0f / 3.0f, 3.0f / 4.0f };
        aData[5].m_uv  = { 2.0f / 3.0f, 3.0f / 4.0f };
        aData[6].m_uv  = { 1.0f / 3.0f, 2.0f / 4.0f };
        aData[7].m_uv  = { 2.0f / 3.0f, 2.0f / 4.0f };  //far
        aData[8].m_uv  = { 2.0f / 3.0f, 2.0f / 4.0f };
        aData[9].m_uv  = { 3.0f / 3.0f, 2.0f / 4.0f };
        aData[10].m_uv = { 2.0f / 3.0f, 1.0f / 4.0f };
        aData[11].m_uv = { 3.0f / 3.0f, 1.0f / 4.0f };  //left
        aData[12].m_uv = { 0.0f / 3.0f, 2.0f / 4.0f };
        aData[13].m_uv = { 1.0f / 3.0f, 2.0f / 4.0f };
        aData[14].m_uv = { 0.0f / 3.0f, 1.0f / 4.0f };
        aData[15].m_uv = { 1.0f / 3.0f, 1.0f / 4.0f };  //right
        aData[16].m_uv = { 1.0f / 3.0f, 4.0f / 4.0f };
        aData[17].m_uv = { 2.0f / 3.0f, 4.0f / 4.0f };
        aData[18].m_uv = { 1.0f / 3.0f, 3.0f / 4.0f };
        aData[19].m_uv = { 2.0f / 3.0f, 3.0f / 4.0f };  //bottom
        aData[20].m_uv = { 1.0f / 3.0f, 2.0f / 4.0f };
        aData[21].m_uv = { 2.0f / 3.0f, 2.0f / 4.0f };
        aData[22].m_uv = { 1.0f / 3.0f, 1.0f / 4.0f };
        aData[23].m_uv = { 2.0f / 3.0f, 1.0f / 4.0f };  //top

        //インデックス作成
        const std::vector<unsigned short> indices = {
             0,  2,  1,         2,  3,  1,
             4,  6,  5,         6,  7,  5,
             8, 10,  9,        10, 11,  9,
            12, 14, 13,        14, 15, 13,
            16, 18, 17,        18, 19, 17,
            20, 22, 21,        22, 23, 21
        };

        VsData<V> vsd(std::move(aData), std::move(indices));
        vsd.SetVertexNormal();
        return vsd;
    }

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    VsdBox() noexcept {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~VsdBox() noexcept {}

    //--------------------------------------------------------------------------
};
