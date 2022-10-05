//==============================================================================
// Filename: Gfx_vsdSphere.h
// Description: 球体形状作成処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_VertexShaderData.h>
#include <Tool/Tool_Math.h>

//===== クラス定義 =====

//***** 球体形状 *****
class VsdSphere
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// テッセレーション作成
    ///
    /// \param[in] latDiv   緯度分割数
    /// \param[in] longDiv  経度分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeTessellation(
        /*[in]*/ int latDiv,
        /*[in]*/ int longDiv)
    {
        //前処理
        namespace dx = DirectX;
        if (latDiv < 3)
            throw ERROR_EX2(S_OK, "緯度分割数は3以上にしてください。");
        if (longDiv < 3)
            throw ERROR_EX2(S_OK, "経度分割数は3以上にしてください。");

        //変数宣言
        constexpr float fRadius = 0.5f;                     //半径
        const dx::XMVECTOR vBase =
            dx::XMVectorSet(0.0f, fRadius, 0.0f, 0.0f);     //基準用頂点ベクトル
        const float fLatAngle = F_PI / latDiv;              //緯度角(180で分割)
        const float fLongAngle = 2.0f * F_PI / longDiv;     //経度角(360で分割)

        //頂点作成
        std::vector<V> aData;
        for (int iLat = 1; iLat < latDiv; iLat++) {
            const dx::XMVECTOR vLatBase = dx::XMVector3Transform(vBase,
                dx::XMMatrixRotationX(fLatAngle * iLat));           //緯度の分割分ごとに
            for (int iLong = 0; iLong < longDiv; iLong++) {
                aData.emplace_back();
                dx::XMVECTOR vVertex = dx::XMVector3Transform(vLatBase,
                    dx::XMMatrixRotationY(fLongAngle * iLong));     //経度の分割数分の頂点を作成
                dx::XMStoreFloat3(&aData.back().m_pos, vVertex);
            }
        }

        //北極点頂点作成
        const unsigned short usIdxNP = (unsigned short)aData.size();
        aData.emplace_back();
        dx::XMStoreFloat3(&aData.back().m_pos, vBase);

        //南極点頂点作成
        const unsigned short usIdxSP = (unsigned short)aData.size();
        aData.emplace_back();
        dx::XMStoreFloat3(&aData.back().m_pos, dx::XMVectorNegate(vBase));

        //インデックス算出用ラムダ式
        const auto calcIdx = [longDiv](unsigned short iLat, unsigned short iLong)
        { return static_cast<unsigned short>(iLat * longDiv + iLong); };

        //インデックスリスト作成
        std::vector<unsigned short> indices;
        const unsigned short mod = static_cast<unsigned short>(longDiv);
        for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++) {
            for (unsigned short iLong = 0; iLong < longDiv; iLong++) {
                indices.emplace_back(calcIdx(iLat, iLong));
                indices.emplace_back(calcIdx(iLat + 1u, iLong));
                indices.emplace_back(calcIdx(iLat, (iLong + 1u) % mod));
                indices.emplace_back(calcIdx(iLat, (iLong + 1u) % mod));
                indices.emplace_back(calcIdx(iLat + 1u, iLong));
                indices.emplace_back(calcIdx(iLat + 1u, (iLong + 1u) % mod));
            }
        }

        //極点周りのインデックスリスト作成
        const unsigned short usLatDiv = static_cast<unsigned short>(latDiv);
        for (unsigned short iLong = 0; iLong < longDiv; iLong++) {

            //北極点
            indices.push_back(usIdxNP);
            indices.push_back(calcIdx(0u, iLong));
            indices.push_back(calcIdx(0u, (iLong + 1u) % mod));

            //南極点
            indices.push_back(calcIdx(usLatDiv - 2u, (iLong + 1u) % mod));
            indices.push_back(calcIdx(usLatDiv - 2u, iLong));
            indices.push_back(usIdxSP);
        }

        return VsData<V>(std::move(aData), std::move(indices));
    }

    //--------------------------------------------------------------------------
    /// データ作成
    ///
    /// \param[in] latDiv   緯度分割数
    /// \param[in] longDiv  経度分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData(
        /*[in]*/ int latDiv = 12,
        /*[in]*/ int longDiv = 12)
    {
        return MakeTessellation<V>(latDiv, longDiv);
    }

    //--------------------------------------------------------------------------
    /// データ作成（モデル用）
    ///
    /// \param[in] latDiv   緯度分割数
    /// \param[in] longDiv  経度分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData_Model(
        /*[in]*/ int latDiv = 12,
        /*[in]*/ int longDiv = 12)
    {
        //前処理
        namespace dx = DirectX;
        if (latDiv < 3)
            throw ERROR_EX2(S_OK, "緯度分割数は3以上にしてください。");
        if (longDiv < 3)
            throw ERROR_EX2(S_OK, "経度分割数は3以上にしてください。");

        //変数宣言
        constexpr float fRadius = 0.5f;                     //半径
        const dx::XMVECTOR vBase =
            dx::XMVectorSet(0.0f, fRadius, 0.0f, 0.0f);     //基準用頂点ベクトル
        const float fLatAngle = F_PI / latDiv;              //緯度角(180で分割)
        const float fLongAngle = 2.0f * F_PI / longDiv;     //経度角(360で分割)

        //頂点作成
        std::vector<V> aData;
        for (int iLat = 1; iLat < latDiv; iLat++) {
            const dx::XMVECTOR vLatBase = dx::XMVector3Transform(vBase,
                dx::XMMatrixRotationX(fLatAngle * iLat));           //緯度の分割分ごとに
            for (int iLong = 0; iLong < longDiv; iLong++) {
                aData.emplace_back();
                dx::XMVECTOR vVertex = dx::XMVector3Transform(vLatBase,
                    dx::XMMatrixRotationY(fLongAngle * iLong));     //経度の分割数分の頂点を作成
                dx::XMStoreFloat3(&aData.back().m_pos, vVertex);
            }
        }

        //北極点頂点作成
        const unsigned short usIdxNP = (unsigned short)aData.size();
        aData.emplace_back();
        dx::XMStoreFloat3(&aData.back().m_pos, vBase);

        //南極点頂点作成
        const unsigned short usIdxSP = (unsigned short)aData.size();
        aData.emplace_back();
        dx::XMStoreFloat3(&aData.back().m_pos, dx::XMVectorNegate(vBase));

        //インデックス算出用ラムダ式
        const auto calcIdx = [longDiv](unsigned short iLat, unsigned short iLong)
        { return static_cast<unsigned short>(iLat * longDiv + iLong); };

        //インデックスリスト作成
        std::vector<unsigned short> indices;
        const unsigned short mod = static_cast<unsigned short>(longDiv);
        for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++) {
            for (unsigned short iLong = 0; iLong < longDiv; iLong++) {
                indices.emplace_back(calcIdx(iLat, iLong));
                indices.emplace_back(calcIdx(iLat + 1u, iLong));
                indices.emplace_back(calcIdx(iLat, (iLong + 1u) % mod));
                indices.emplace_back(calcIdx(iLat, (iLong + 1u) % mod));
                indices.emplace_back(calcIdx(iLat + 1u, iLong));
                indices.emplace_back(calcIdx(iLat + 1u, (iLong + 1u) % mod));
            }
        }

        //極点周りのインデックスリスト作成
        const unsigned short usLatDiv = static_cast<unsigned short>(latDiv);
        for (unsigned short iLong = 0; iLong < longDiv; iLong++) {

            //北極点
            indices.push_back(usIdxNP);
            indices.push_back(calcIdx(0u, iLong));
            indices.push_back(calcIdx(0u, (iLong + 1u) % mod));

            //南極点
            indices.push_back(calcIdx(usLatDiv - 2u, (iLong + 1u) % mod));
            indices.push_back(calcIdx(usLatDiv - 2u, iLong));
            indices.push_back(usIdxSP);
        }

        //モデル用に頂点データ、インデックスを再作成
        std::vector<V> aModelData;
        for (auto& i : indices) {
            V vtx{};
            vtx.m_pos = aData[i].m_pos;
            aModelData.emplace_back(std::move(vtx));
        }
        for (size_t i = 0, Cnt = indices.size(); i < Cnt; i++)
            indices[i] = static_cast<unsigned short>(i);

        //UV作成（デバッグ用、テクスチャなし）
        for (size_t i = 0, Cnt = indices.size() / 3; i < Cnt; i++) {
            aModelData[i * 3 + 0].m_uv = { 0.0f, 0.0f };
            aModelData[i * 3 + 1].m_uv = { 1.0f, 0.0f };
            aModelData[i * 3 + 2].m_uv = { 0.0f, 1.0f };
        }

        VsData<V> vsd(std::move(aModelData), std::move(indices));
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
    VsdSphere() noexcept {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~VsdSphere() noexcept {}

    //--------------------------------------------------------------------------
};
