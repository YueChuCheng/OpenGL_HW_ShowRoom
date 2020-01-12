// Phong reflection model
#version 150
#define LIGHT_NUM_MAX 4 //�O�w�ƶq
in vec4 vPosition;	// Vertex Position
in vec3 vNormal;    // Vertex Normal
in vec4 vVtxColor;  // Vertex Color
in vec2 vDiffuseMapCoord;
in vec2 vLightMapCoord;
in vec2 vNormalMapCoord;
in vec3 vTangentV;

out vec3 v3N[LIGHT_NUM_MAX]; // ��X Normal �b Tangent Space �y�ФU����V
out vec3 v3L[LIGHT_NUM_MAX]; // ��X Light Direction �b Tangent Space �y�ФU����V
out vec3 v3E[LIGHT_NUM_MAX]; // ��X View Direction �b Tangent Space �y�ФU����V
out vec2 DiffuseMapUV;  // ��X�K�Ϯy��
out vec2 LightMapUV;    // ��X�K�Ϯy��
out vec2 NormalMapUV;  // ��X�K�Ϯy��

uniform mat4  ModelView;   // Model View Matrix
uniform mat4  Projection;  // Projection Matrix
uniform vec4  LightInView[LIGHT_NUM_MAX]; // Light's position in View Space

void main()
{
	// Vertex �ഫ�� World Space
	vec4 vPosInView = ModelView * vPosition;

	// Building the matrix Eye Space -> Tangent Space
	// �H�U���Ω�p��磌��i�� non-uniform scale �ɡA���� normal ���ץ��p��
	mat3 ITModelView = transpose(inverse(mat3(ModelView))); 
	//	vec3 vN = normalize(ITModelView * vNormal); 

    // �p�� vertex normal �P vTangent �ഫ�쥿�T�� object local space �W
	vec3 vn = normalize(ITModelView * vNormal);
	vec3 vt = normalize(ITModelView * vTangentV);
	vec3 vb = cross(vn, vt);
	vec3 tmpV[LIGHT_NUM_MAX];
	// �N Light Vector �ഫ�� Tangent Space

	for(int i = 0 ; i < LIGHT_NUM_MAX ; i++){
		tmpV[i] = vec3(LightInView[i].xyz - vPosInView.xyz);
	}
	for(int i = 0 ; i < LIGHT_NUM_MAX ; i++){
		v3L[i].x = dot(tmpV[i], vt);
		v3L[i].y = dot(tmpV[i], vb);
		v3L[i].z = dot(tmpV[i], vn);
	}
	// �N Eye Vector �ഫ�� Tangent Space
	for(int i = 0 ; i < LIGHT_NUM_MAX ; i++){
		tmpV[i] = -vPosInView.xyz;
		v3E[i].x = dot(tmpV[i], vt);
		v3E[i].y = dot(tmpV[i], vb);
		v3E[i].z = dot(tmpV[i], vn);
	}
	gl_Position = Projection * vPosInView;
	DiffuseMapUV = vDiffuseMapCoord;
	LightMapUV  = vLightMapCoord;
	NormalMapUV = vNormalMapCoord;
}
