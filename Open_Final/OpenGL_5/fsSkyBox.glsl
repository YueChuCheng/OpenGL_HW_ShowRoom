#version 130

in vec2 DiffuseMapUV; // 輸入 Diffuse Map 貼圖座標

// For Texture Sampler
uniform sampler2D diffuMap; // 貼圖的參數設定

void main()
{
		gl_FragColor = texture2D(diffuMap, DiffuseMapUV);
}
