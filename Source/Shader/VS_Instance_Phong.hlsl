//==============================================================================
// Filename: VS_Instance_Phong.hlsl
// Description: Phong���f���p���_�V�F�[�_
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//���͗p�\����
struct VS_IN
{
    //--------------------------------------------------------------------------
    float3 pos : POSITION;              //���W
    float2 tex : TEXCOORD;              //UV���W
    float3 normal : NORMAL;             //�@��
    matrix mtxWorld : WORLD_MTX;        //���[���h�s��
    //--------------------------------------------------------------------------

    /// <summary>
    /// pos         ���_���W
    /// tex         UV���W
    /// normal      ���_�@��
    /// mtxWorld    ���[���h�s��
    /// </summary>
};

//�o�͗p�\����
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
    /// posWV       ���_���W�i�r���[�ϊ���j
    /// tex         UV���W
    /// normalWV    ���_�@���i�r���[�ϊ���j
    /// mtxView     �r���[�s��
    /// pos         �V�X�e���o�����[
    /// </summary>
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer CB_CAMERA : register(b1)
{
    matrix mtxView;     //�r���[�s��
    matrix mtxProj;     //���e�s��
};

//------------------------------------------------------------------------------
/// �G���g���[�|�C���g
///
/// \param[in] psi  ���͗p�\����
///
/// \return �o�͗p�\����
//------------------------------------------------------------------------------
VS_OUT main(VS_IN vsi)
{
    VS_OUT vso;

    //���W�v�Z
    vso.pos = float4(vsi.pos, 1.0f);
    vso.pos = mul(vso.pos, vsi.mtxWorld);
    vso.pos = mul(vso.pos, mtxView);
    vso.posWV = vso.pos.xyz;
    vso.pos = mul(vso.pos, mtxProj);

    //�e�N�X�`��
    vso.tex = vsi.tex;

    //�@��
    vso.normalWV = vsi.normal;
    vso.normalWV = mul(vso.normalWV, (float3x3) vsi.mtxWorld);
    vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

    //���̑�
    vso.mtxView = mtxView;

    return vso;
}
