
struct OutputVS
{
    float4 svpos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D<float4> Tex : register(t0);
SamplerState Sampler : register(s0);

//�萔�o�b�t�@
cbuffer cbMatrix : register(b0)
{
    matrix mtxW;    //���[���h�s��
};
