#version 130

in vec2 DiffuseMapUV; // ��J Diffuse Map �K�Ϯy��

// For Texture Sampler
uniform sampler2D diffuMap; // �K�Ϫ��ѼƳ]�w

void main()
{
		gl_FragColor = texture2D(diffuMap, DiffuseMapUV);
}
