
#version 440 core

//����
in vec2 TexCoord;

//�o��
out vec4 FragColor;

//�O���[�o���ϐ�
uniform sampler2D texture1;

//�G���g���[�|�C���g
void main()
{
    FragColor = texture(texture1, TexCoord);
}
