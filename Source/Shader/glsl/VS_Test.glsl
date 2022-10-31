
#version 440 core

//����
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aOffset;
//layout (location = 3) in mat4 aMtx;

//�o��
out vec2 TexCoord;

//�ϊ��s��
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//�G���g���[�|�C���g
void main()
{
    vec4 posW = model * vec4(aPos, 1.0);
    posW.xy += aOffset;
    gl_Position = projection * view * posW;
    TexCoord = aTexCoord;
    TexCoord.y = 1.0f - TexCoord.y;     //DirectX��OpenGL��UV���W���Ⴄ�ׂ̏C��
}
