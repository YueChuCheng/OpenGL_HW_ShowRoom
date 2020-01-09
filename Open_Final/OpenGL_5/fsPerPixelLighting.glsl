#version 130
// iTexLayer ���]�w�P�P�_�覡�A���]���B�i�H�䴩�줻�i�K��

#define LIGHT_NUM 4 //�O�w�ƶq
#define NONE_MAP 0
#define DIFFUSE_MAP 1
#define LIGHT_MAP 2
#define NORMAL_MAP 4

in vec3 v3N;
in vec3 v3L[LIGHT_NUM];
in vec3 v3E;

in vec2 DiffuseMapUV; // ��J Diffuse Map �K�Ϯy��
in vec2 LightMapUV;   // ��J Light Map �K�Ϯy��

uniform int iTexLayer;
uniform float fElapsedTime;

// �H�U���s�W������
uniform vec4 LightInView[LIGHT_NUM];	 // Light's position in View Space
uniform vec4 AmbientProduct[LIGHT_NUM];  // light's ambient  �P Object's ambient  �P ka �����n
uniform vec4 DiffuseProduct[LIGHT_NUM];  // light's diffuse  �P Object's diffuse  �P kd �����n
uniform vec4 SpecularProduct[LIGHT_NUM]; // light's specular �P Object's specular �P ks �����n
uniform float fShininess;
uniform int iLighting[LIGHT_NUM];
uniform float Cutoff[LIGHT_NUM];
uniform vec4 vObjectColor; // �N���󪺳�@�C��
uniform int LightNUM; // ���X���O

// For Texture Sampler
uniform sampler2D diffuMap; // �K�Ϫ��ѼƳ]�w
uniform sampler2D lightMap; // �K�Ϫ��ѼƳ]�w

void main()
{
	// ���ŧi diffuse �P specular
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 LightingColor = vec4(0.0, 0.0, 0.0, 1.0);
	for (int i = 0; i < LightNUM; i++)
	{
		if (iLighting[i] != 1)
		{
			if (iTexLayer == NONE_MAP)
				gl_FragColor = vObjectColor;
			else
				gl_FragColor = vObjectColor * texture2D(diffuMap, DiffuseMapUV);
		}
		else
		{

			//V 1. �p�� Ambient color : Ia = AmbientProduct = Ka * Material.ambient * La =
			vec4 ambient = AmbientProduct[0]; // m_sMaterial.ka * m_sMaterial.ambient * vLightI;

			// ���ƶǤJ�� Normal Dir
			vec3 vN = normalize(v3N);

			//V 2. ���ƶǤJ�� Light Dir
			vec3 vL = normalize(v3L[i]); // normalize light vector

			//V 5. �p�� L dot N
			float fLdotN = vL.x * vN.x + vL.y * vN.y + vL.z * vN.z;
			if (fLdotN >= Cutoff[i])
			{ // ���I�Q�����Ө�~�ݭn�p��

				//V Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
				if(Cutoff[i] > 0.0f) //spot light
					diffuse += pow(fLdotN , 40) * DiffuseProduct[i];
				else
					diffuse += fLdotN * DiffuseProduct[i]  ;

				// Specular color
				// Method 1: Phone Model
				// �p�� View Vector
				// ���ƶǤJ�� v3E , View Direction
				vec3 vV = normalize(v3E);

				//�p�� Light �� �Ϯg�� vRefL = 2 * fLdotN * vN - L
				// �P�ɧQ�� normalize �ন���V�q
				vec3 vRefL = normalize(2.0f * (fLdotN)*vN - vL);

				//   �p��  vReflectedL dot View
				float RdotV = vRefL.x * vV.x + vRefL.y * vV.y + vRefL.z * vV.z;

				//V Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
				if (RdotV > 0)
					specular += SpecularProduct[i] * pow(RdotV, fShininess);
			}
			

			LightingColor = ambient + diffuse + specular; // �p���C�� ambient + diffuse + specular;
			LightingColor.w += DiffuseProduct[i].w;		  //V �]�w���ǤJ���誺 alpha,	DiffuseProduct.w
														  //LightingColor.w = 1.0;	// �]�w alpha �� 1.0

			if (iTexLayer == NONE_MAP)
				gl_FragColor = LightingColor;
			else if (iTexLayer == DIFFUSE_MAP || iTexLayer == (DIFFUSE_MAP | NORMAL_MAP))
				gl_FragColor = LightingColor * texture2D(diffuMap, DiffuseMapUV);
			else if (iTexLayer == (DIFFUSE_MAP | LIGHT_MAP))
			{
				gl_FragColor = (0.65 * LightingColor * texture2D(diffuMap, DiffuseMapUV) + texture2D(diffuMap, DiffuseMapUV) *
																							   texture2D(lightMap, LightMapUV) * vec4(0.0f, 1.0f, 1.0f, 1.00));

				//			float t = 0.15 + 0.75 * abs(sin(fElapsedTime * 3.1415926 * 0.125));
				//			gl_FragColor =  ( 0.55 * LightingColor * texture2D(diffuMap, DiffuseMapUV)  + // texture2D(diffuMap, DiffuseMapUV) *
				//							texture2D(lightMap, LightMapUV) * vec4(0.0f, 1.0f, 1.0f, 1.0) * t);
			}
		}
	}
}
