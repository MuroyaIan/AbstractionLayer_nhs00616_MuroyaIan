//==============================================================================
// Filename: VS_Instance_Phong.hlsl
// Description: Phongモデル用頂点シェーダ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//入力用構造体
struct VS_IN
{
    //--------------------------------------------------------------------------
    float3 pos : POSITION;              //座標
    float2 tex : TEXCOORD;              //UV座標
    float3 normal : NORMAL;             //法線
    matrix mtxWorld : WORLD_MTX;        //ワールド行列
    //--------------------------------------------------------------------------

    /// <summary>
    /// pos         頂点座標
    /// tex         UV座標
    /// normal      頂点法線
    /// mtxWorld    ワールド行列
    /// </summary>
};

//出力用構造体
struct VS_OUT
{
    //--------------------------------------------------------------------------
    float3 posWV : POSITION;
    float2 tex : TEXCOORD;
    float3 normalWV : NORMAL;
    matrix mtxView : MTX_V;
    float4 pos : SV_Position;
    //--------------------------------------------------------------------------

    /// <summary>
    /// posWV       頂点座標（ビュー変換後）
    /// tex         UV座標
    /// normalWV    頂点法線（ビュー変換後）
    /// mtxView     ビュー行列
    /// pos         システムバリュー
    /// </summary>
};

//定数バッファ（変換行列）
cbuffer CB_CAMERA : register(b1)
{
    matrix mtxView;     //ビュー行列
    matrix mtxProj;     //投影行列
};

//------------------------------------------------------------------------------
/// エントリーポイント
///
/// \param[in] psi  入力用構造体
///
/// \return 出力用構造体
//------------------------------------------------------------------------------
VS_OUT main(VS_IN vsi)
{
    VS_OUT vso;

    //座標計算
    vso.pos = float4(vsi.pos, 1.0f);
    vso.pos = mul(vso.pos, vsi.mtxWorld);
    vso.pos = mul(vso.pos, mtxView);
    vso.posWV = vso.pos.xyz;
    vso.pos = mul(vso.pos, mtxProj);

    //テクスチャ
    vso.tex = vsi.tex;

    //法線
    vso.normalWV = vsi.normal;
    vso.normalWV = mul(vso.normalWV, (float3x3) vsi.mtxWorld);
    vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

    //その他
    vso.mtxView = mtxView;

    return vso;
}
