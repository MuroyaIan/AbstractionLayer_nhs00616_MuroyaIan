//==============================================================================
// Filename: Gfx_vsdPrism.h
// Description: 角柱形状作成処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_VertexShaderData.h>
#include <Tool/Tool_Math.h>

//===== クラス定義 =====

//***** 角柱形状 *****
class VsdPrism
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// テッセレーション作成
    ///
    /// \param[in] divNum   分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeTessellation(
        /*[in]*/ int divNum)
    {
        //前処理
        namespace dx = DirectX;
        if (divNum < 3)
            throw ERROR_EX2(S_OK, "分割数は3以上にしてください。");

        //変数宣言
        const dx::XMVECTOR vBase =
            dx::XMVectorSet(0.0f, -0.5f, 0.5f, 0.0f);   //基準用頂点ベクトル
        const dx::XMVECTOR vOffset =
            dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);    //角柱の高さ
        const float fDivAngle = 2.0f * F_PI / divNum;   //分割用回転角(ラジアン)
        std::vector<V> aData;

        //底面（下）の中心頂点作成
        aData.emplace_back();
        aData.back().m_pos = { 0.0f, -0.5f, 0.0f };
        const unsigned short usDownCtrIdx =
            static_cast<unsigned short>(aData.size() - 1);  //インデックス参照

        //底面（上）の中心頂点作成
        aData.emplace_back();
        aData.back().m_pos = { 0.0f, 0.5f, 0.0f };
        const unsigned short usUpCtrIdx = static_cast<unsigned short>(aData.size() - 1);

        //底面頂点作成
        for (int i = 0; i < divNum; i++) {

            //底面（下）
            {
                aData.emplace_back();
                dx::XMVECTOR vVertex =dx::XMVector3Transform(vBase,
                    dx::XMMatrixRotationY(fDivAngle * i));              //Y軸回転
                dx::XMStoreFloat3(&aData.back().m_pos, vVertex);
            }

            //底面（上）
            {
                aData.emplace_back();
                dx::XMVECTOR vVertex = dx::XMVector3Transform(vBase,
                    dx::XMMatrixRotationY(fDivAngle * i));              //Y軸回転
                vVertex = dx::XMVectorAdd(vVertex, vOffset);            //オフセット追加
                dx::XMStoreFloat3(&aData.back().m_pos, vVertex);
            }
        }

        //側面のインデックスリスト作成
        std::vector<unsigned short> indices;
        const unsigned short mod = static_cast<unsigned short>(divNum * 2);
        for (unsigned short i = 0; i < divNum; i++) {
            const unsigned short idx = i * 2;
            indices.push_back(idx + 2u);
            indices.push_back((idx + 2u) % mod + 2u);
            indices.push_back(idx + 3u);
            indices.push_back((idx + 2u) % mod + 2u);
            indices.push_back((idx + 3u) % mod + 2u);
            indices.push_back(idx + 3u);
        }

        //底面のインデックスリスト作成
        for (unsigned short i = 0; i < divNum; i++) {
            const unsigned short idx = i * 2;
            indices.push_back(idx + 2u);
            indices.push_back(usDownCtrIdx);
            indices.push_back((idx + 2u) % mod + 2u);   //底面（下）
            indices.push_back(usUpCtrIdx);
            indices.push_back(idx + 3u);
            indices.push_back((idx + 3u) % mod + 2u);   //底面（上）
        }

        return VsData<V>(std::move(aData), std::move(indices));
    }

    //--------------------------------------------------------------------------
    /// データ作成
    ///
    /// \param[in] divNum   分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData(
        /*[in]*/ int divNum = 24)
    {
        return MakeTessellation<V>(divNum);
    }

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    VsdPrism() noexcept {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~VsdPrism() noexcept {}

    //--------------------------------------------------------------------------
};
