// Phong reflection model
#version 130
#define LIGHT_NUM_MAX 4 //�O�w�ƶq

in vec4 vPosition;	// Vertex Position
in vec3 vNormal;    // Vertex Normal
in vec4 vVtxColor;  // Vertex Color
in vec2 vDiffuseMapCoord;
in vec2 vLightMapCoord;

out vec3 v3N; // ��X Normal �b���Y�y�ФU����V
out vec3 v3L[LIGHT_NUM_MAX]; // ��X Light Direction �b���Y�y�ФU����V
out vec3 v3E; // ��X View Direction �b���Y�y�ФU����V
out vec2 DiffuseMapUV;  // ��X�K�Ϯy��
out vec2 LightMapUV;    // ��X�K�Ϯy��
out vec3 lD[LIGHT_NUM_MAX];

uniform mat4  ModelView;   // Model View Matrix
uniform mat4  Projection;  // Projection Matrix
uniform vec4  LightInView[LIGHT_NUM_MAX]; // Light's position in View Space
uniform int iLightNUM; //�X���O��V
uniform vec4  SpotTarget[LIGHT_NUM_MAX];      //Spot���ӥؼЦ�m


void main()
{
	vec4 vPosInView = ModelView * vPosition;
	// �ثe�w�g�H���Y�y�Ь���¦, �ҥH View ����m�N�O (0,0,0), �ҥH��m���I���N�O View Dir
	v3E = -vPosInView.xyz;

	// �H�U���Ω�p��磌��i�� non-uniform scale �ɡA���� normal ���ץ��p��
	//		mat3 ITModelView = transpose(inverse(mat3(ModelView)); 
	//		vec3 v3N = normalize(ITModelView * vNormal); 
	v3N = (ModelView * vec4(vNormal, 0.0)).xyz;
	for(int i = 0 ; i < iLightNUM ; i++){
		v3L[i] = vec3(LightInView[i].xyz - vPosInView.xyz);
		lD[i]  = vec3(SpotTarget[i].xyz - LightInView[i].xyz);
	}
	gl_Position = Projection * vPosInView;
	DiffuseMapUV = vDiffuseMapCoord;
	LightMapUV  = vLightMapCoord;
}

