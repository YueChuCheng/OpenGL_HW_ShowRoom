// Phong reflection model
#version 150
#define LIGHT_NUM_MAX 4 //燈泡數量
in vec4 vPosition;	// Vertex Position
in vec3 vNormal;    // Vertex Normal
in vec4 vVtxColor;  // Vertex Color
in vec2 vDiffuseMapCoord;
in vec2 vLightMapCoord;
in vec2 vNormalMapCoord;
in vec3 vTangentV;

out vec3 v3N[LIGHT_NUM_MAX]; // 輸出 Normal 在 Tangent Space 座標下的方向
out vec3 v3L[LIGHT_NUM_MAX]; // 輸出 Light Direction 在 Tangent Space 座標下的方向
out vec3 v3E[LIGHT_NUM_MAX]; // 輸出 View Direction 在 Tangent Space 座標下的方向
out vec2 DiffuseMapUV;  // 輸出貼圖座標
out vec2 LightMapUV;    // 輸出貼圖座標
out vec2 NormalMapUV;  // 輸出貼圖座標

uniform mat4  ModelView;   // Model View Matrix
uniform mat4  Projection;  // Projection Matrix
uniform vec4  LightInView[LIGHT_NUM_MAX]; // Light's position in View Space

void main()
{
	// Vertex 轉換到 World Space
	vec4 vPosInView = ModelView * vPosition;

	// Building the matrix Eye Space -> Tangent Space
	// 以下兩行用於計算對物件進行 non-uniform scale 時，物件 normal 的修正計算
	mat3 ITModelView = transpose(inverse(mat3(ModelView))); 
	//	vec3 vN = normalize(ITModelView * vNormal); 

    // 計算 vertex normal 與 vTangent 轉換到正確的 object local space 上
	vec3 vn = normalize(ITModelView * vNormal);
	vec3 vt = normalize(ITModelView * vTangentV);
	vec3 vb = cross(vn, vt);
	vec3 tmpV[LIGHT_NUM_MAX];
	// 將 Light Vector 轉換到 Tangent Space

	for(int i = 0 ; i < LIGHT_NUM_MAX ; i++){
		tmpV[i] = vec3(LightInView[i].xyz - vPosInView.xyz);
	}
	for(int i = 0 ; i < LIGHT_NUM_MAX ; i++){
		v3L[i].x = dot(tmpV[i], vt);
		v3L[i].y = dot(tmpV[i], vb);
		v3L[i].z = dot(tmpV[i], vn);
	}
	// 將 Eye Vector 轉換到 Tangent Space
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
