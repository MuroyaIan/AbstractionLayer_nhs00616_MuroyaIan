//==============================================================================
// Filename: Gfx_vsdGrid.h
// Description: グリッド作成処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_VertexShaderData.h>
#include <Tool/Tool_Math.h>

//===== クラス定義 =====

//***** 板形状 *****
class VsdGrid
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// テッセレーション作成
    ///
    /// \param[in] divX     X軸分割数
    /// \param[in] divY     Y軸分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeTessellation(
        /*[in]*/ int divX,
        /*[in]*/ int divY)
    {
        //前処理
        namespace dx = DirectX;
        if (divX < 1)
            throw ERROR_EX2(S_OK, "板の横分割数は1以上にしてください。");
        if (divY < 1)
            throw ERROR_EX2(S_OK, "板の縦分割数は1以上にしてください。");

        //変数宣言
        constexpr float fWidth = 1.0f;
        constexpr float fHeight = 1.0f;     //横幅・縦幅
        const int nVtxNumX = divX + 1;
        const int nVtxNumY = divY + 1;      //横・縦頂点数

        //頂点作成
        std::vector<V> aData(nVtxNumX * nVtxNumY);
        const dx::XMVECTOR vBottomLeft =
            dx::XMVectorSet(-0.5f, -0.5f, 0.0f, 0.0f);  //板の左下頂点
        const float fUnitSizeX = fWidth / divX;
        const float fUnitSizeY = fHeight / divY;            //板の分割単位ごとの横幅・縦幅
        for (int y = 0, nIdxCnt = 0; y < nVtxNumY; y++) {
            const float posY = y * fUnitSizeY;              //行ごとの縦幅
            for (int x = 0; x < nVtxNumX; x++) {
                const dx::XMVECTOR vVertex = dx::XMVectorAdd(vBottomLeft,
                    dx::XMVectorSet(x * fUnitSizeX, posY, 0.0f, 0.0f));
                dx::XMStoreFloat3(&aData[nIdxCnt].m_pos, vVertex);
                nIdxCnt++;                                  //インデックスカウンタ
            }
        }

        //インデックス算出用ラムダ式
        const auto calcIdx = [nVtxNumX](int x, int y)
        { return static_cast<unsigned short>(y * nVtxNumX + x); };

        //インデックス作成
        std::vector<unsigned short> indices;
        indices.reserve(static_cast<unsigned short>(divX * divY * 4 + (divX + divY) * 2));  //サイズ指定
        for (int y = 0; y < divY; y++) {
            for (int x = 0; x < divX; x++) {

                //ユニットごとの頂点インデックスを算出
                const std::array<unsigned short, 4> aIndex = {
                    calcIdx(x, y),
                    calcIdx(x + 1, y),
                    calcIdx(x, y + 1),
                };

                //インデックス格納
                indices.emplace_back(aIndex[0]);
                indices.emplace_back(aIndex[1]);
                indices.emplace_back(aIndex[0]);
                indices.emplace_back(aIndex[2]);
            }
        }
        for (int x = 0, y = divY; x < divX; x++) {

            //インデックス格納（一番上の行）
            indices.emplace_back(calcIdx(x, y));
            indices.emplace_back(calcIdx(x + 1, y));
        }
        for (int x = divX, y = 0; y < divY; y++) {

            //インデックス格納（一番右の列）
            indices.emplace_back(calcIdx(x, y));
            indices.emplace_back(calcIdx(x, y + 1));
        }

        return VsData<V>(std::move(aData), std::move(indices), false);
    }

    //--------------------------------------------------------------------------
    /// データ作成
    ///
    /// \param[in] divX     X軸分割数
    /// \param[in] divY     Y軸分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData(
        /*[in]*/ int divX = 1,
        /*[in]*/ int divY = 1)
    {
        return MakeTessellation<V>(divX, divY);
    }

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    VsdGrid() noexcept {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~VsdGrid() noexcept {}

    //--------------------------------------------------------------------------
};
