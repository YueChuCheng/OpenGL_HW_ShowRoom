#include "header/Angel.h"
#include "Common/TypeDefine.h"
#include "Common/CQuad.h"
#include "Common/CSmoothQuad.h"
#include "Common/C2DBTN.h"
#include "Common/CSoildCube.h"
#include "Common/test.h"
#include "Common/CCamera.h"
#include "Common/CTexturePool.h"
#include "png_loader.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 600
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 40 // must be an even number


GLuint g_uiFTexID; //貼圖

//Room1 Wall
CQuad* CSFloor_Room1;
CQuad* CSCeiling_Room1;
	 
CQuad* CQRightWall_Room1;
CQuad* CQLeftWall_Room1;
CQuad* CQFrontWall_Room1;
CQuad* CQBackWall_Room1;



//Room2 Wall
CQuad* CSFloor_Room2;
CQuad* CSCeiling_Room2;
CQuad* CQRightWall_Room2;
CQuad* CQLeftWall_Room2;
CQuad* CQBackWall_Room2;
CQuad* CQFrontWall_Room2;

//Room3 Wall
CQuad* CSFloor_Room3;
CQuad* CSCeiling_Room3;
CQuad* CQRightWall_Room3;
CQuad* CQLeftWall_Room3;
CQuad* CQBackWall_Room3;
CQuad* CQFrontWall_Room3;

//Room4 Wall
CQuad* CSFloor_Room4;
CQuad* CSCeiling_Room4;
CQuad* CQRightWall_Room4;
CQuad* CQLeftWall_Room4;
CQuad* CQBackWall_Room4;
CQuad* CQFrontWall_Room4;


//Room5 Wall
CQuad* CSFloor_Room5;
CQuad* CSCeiling_Room5;
CQuad* CQRightWall_Room5;
CQuad* CQLeftWall_Room5;
CQuad* CQBackWall_Room5;
CQuad* CQFrontWall_Room5;

//Room6 Wall
CQuad* CSFloor_Room6;
CQuad* CSCeiling_Room6;

CQuad* CQRightWall_Room6;
CQuad* CQLeftWall_Room6;
CQuad* CQFrontWall_Room6;
CQuad* CQBackWall_Room6;


//button
GLfloat g_fRadius = 120.0; //視覺範圍
GLfloat g_fTheta = 60.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;
GLfloat g_fCameraMoveX = 0.f;				// for camera movment
GLfloat g_fCameraMoveY = 7.0f;				// for camera movment
GLfloat g_fCameraMoveZ = 0.f;				// for camera movment
mat4	g_matMoveDir;		// 鏡頭移動方向
point4  g_MoveDir;
point4  g_at;				// 鏡頭觀看方向
point4  g_eye;				// 鏡頭位置



//camera
point4  eye(g_fRadius* sin(g_fTheta)* sin(g_fPhi), g_fRadius* cos(g_fTheta), g_fRadius* sin(g_fTheta)* cos(g_fPhi), 1.0f);
point4  at(0.0f, 0.0f, 0.0f, 1.0f); 
point4 g_vUp(0.0, 1.0, 0.0, 0.0);
point4 front;
point4 right;

// Part 2 : for single light source
bool g_bAutoRotating = false;
float _fLightDelta = 0; //燈光旋轉，經過時間
float _fLightRadius = 6;//燈光位置
float _fLightTheta = 0;//燈光旋轉角度

float g_fLightR = 0.7f;
float g_fLightG = 0.7f;
float g_fLightB = 0.7f;




LightSource  Light_center_Room1 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius, 15.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	-0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  Light_center_Room2 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius, 15.0f, 40.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	-0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  Light_center_Room3 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius, 15.0f, 80.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	-0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};


LightSource  Light_center_Room4 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4 (_fLightRadius + 40.0f , 15.0f, 80.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	-0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  Light_center_Room5 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius + 40 , 15.0f, 40.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	-0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};


LightSource  Light_center_Room6 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius + 40, 15.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	-0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  _Light1 = {

	color4(0.0, 0.0, 0.0, 1.0f), // ambient 
	color4(1.0, 0.0, 0.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(0.0, 17.0, -10.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 10.0f),		  //spotTarget
	vec3(0.0, 0.0, -10.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	1.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  _Light2 = {

	color4(0.0, 0.0, 0.0, 1.0f), // ambient 
	color4(0.0, 1.0, 0.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(-10.4, 17.0, 6.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 10.0f),		  //spotTarget
	vec3(-10.4, 17.0, 6.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	1.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  _Light3 = {

	color4(0.0, 0.0, 0.0, 1.0f), // ambient 
	color4(0.0, 0.0, 1.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(10.4, 17.0, 6.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 10.0f),		  //spotTarget
	vec3(10.4, 0.0, 6.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	1.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource Light_resulte_Room1[LIGHT_NUM] = { //最後燈光結果

	Light_center_Room1, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_Room2[LIGHT_NUM] = { //最後燈光結果

	Light_center_Room2, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_Room3[LIGHT_NUM] = { //最後燈光結果

	Light_center_Room3, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_Room4[LIGHT_NUM] = { //最後燈光結果

	Light_center_Room4, _Light1 ,  _Light2 ,  _Light3
};


LightSource Light_resulte_Room5[LIGHT_NUM] = { //最後燈光結果

	Light_center_Room5, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_Room6[LIGHT_NUM] = { //最後燈光結果

	Light_center_Room6, _Light1 ,  _Light2 ,  _Light3
};


// 函式的原型宣告
extern void IdleProcess();

void init_Room1() {

	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 0.0f; vT.y = 0.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	CSFloor_Room1 = new CQuad;
	CSFloor_Room1->SetTextureLayer(1);
	CSFloor_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room1->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSFloor_Room1->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room1->setColor(vec4(0.6f));
	
	CSFloor_Room1->setShader();
	
	

	vT.x = 0.0f; vT.y = 40.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room1 = new CQuad;
	CSCeiling_Room1->setNormal(vceilingNormal);
	CSCeiling_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room1->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room1->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room1->setShader();




	vT.x = -20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQLeftWall_Room1 = new CQuad;
	CQLeftWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room1->setShader();
	CQLeftWall_Room1->setColor(vec4(0.6f));
	CQLeftWall_Room1->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQRightWall_Room1 = new CQuad;
	CQRightWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room1->setShader();
	CQRightWall_Room1->setColor(vec4(0.6f));
	CQRightWall_Room1->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	CQFrontWall_Room1 = new CQuad;
	CQFrontWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room1->setShader();
	CQFrontWall_Room1->setColor(vec4(0.6f));
	CQFrontWall_Room1->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = -20.0f;
	mxT = Translate(vT);
	CQBackWall_Room1 = new CQuad;
	CQBackWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room1->setShader();
	CQBackWall_Room1->setColor(vec4(0.6f));
	CQBackWall_Room1->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


}

void init_Room2() {


	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 0.0f; vT.y = 0.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CSFloor_Room2 = new CQuad;
	CSFloor_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room2->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room2->setColor(vec4(0.6f));
	CSFloor_Room2->setShader();
	CSFloor_Room2->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));

	vT.x = 0.0f; vT.y = 40.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room2 = new CQuad;
	CSCeiling_Room2->setNormal(vceilingNormal);
	CSCeiling_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room2->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room2->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room2->setShader();



	vT.x = -20.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQLeftWall_Room2 = new CQuad;
	CQLeftWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room2->setShader();
	CQLeftWall_Room2->setColor(vec4(0.6f));
	CQLeftWall_Room2->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 20.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQRightWall_Room2 = new CQuad;
	CQRightWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room2->setShader();
	CQRightWall_Room2->setColor(vec4(0.6f));
	CQRightWall_Room2->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 61.0f;
	mxT = Translate(vT);
	CQFrontWall_Room2 = new CQuad;
	CQFrontWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room2->setShader();
	CQFrontWall_Room2->setColor(vec4(0.6f));
	CQFrontWall_Room2->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 21.0f;
	mxT = Translate(vT);
	CQBackWall_Room2 = new CQuad;
	CQBackWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room2->setShader();
	CQBackWall_Room2->setColor(vec4(0.6f));
	CQBackWall_Room2->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


}

void init_Room3() {

	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 0.0f; vT.y = 0.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CSFloor_Room3 = new CQuad;
	CSFloor_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room3->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room3->setColor(vec4(0.6f));
	CSFloor_Room3->setShader();
	CSFloor_Room3->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));

	vT.x = 0.0f; vT.y = 40.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room3 = new CQuad;
	CSCeiling_Room3->setNormal(vceilingNormal);
	CSCeiling_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room3->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room3->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room3->setShader();



	vT.x = -20.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQLeftWall_Room3 = new CQuad;
	CQLeftWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room3->setShader();
	CQLeftWall_Room3->setColor(vec4(0.6f));
	CQLeftWall_Room3->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 20.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQRightWall_Room3 = new CQuad;
	CQRightWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room3->setShader();
	CQRightWall_Room3->setColor(vec4(0.6f));
	CQRightWall_Room3->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 102.0f;
	mxT = Translate(vT);
	CQFrontWall_Room3 = new CQuad;
	CQFrontWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room3->setShader();
	CQFrontWall_Room3->setColor(vec4(0.6f));
	CQFrontWall_Room3->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 62.0f;
	mxT = Translate(vT);
	CQBackWall_Room3 = new CQuad;
	CQBackWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room3->setShader();
	CQBackWall_Room3->setColor(vec4(0.6f));
	CQBackWall_Room3->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


}



void init_Room4() {
	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 41.0f; vT.y = 0.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CSFloor_Room4 = new CQuad;
	CSFloor_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room4->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room4->setColor(vec4(0.6f));
	CSFloor_Room4->setShader();
	CSFloor_Room4->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));

	vT.x = 41.0f; vT.y = 40.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room4 = new CQuad;
	CSCeiling_Room4->setNormal(vceilingNormal);
	CSCeiling_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room4->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room4->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room4->setShader();



	vT.x = 21.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQLeftWall_Room4 = new CQuad;
	CQLeftWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room4->setShader();
	CQLeftWall_Room4->setColor(vec4(0.6f));
	CQLeftWall_Room4->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 61.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQRightWall_Room4 = new CQuad;
	CQRightWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room4->setShader();
	CQRightWall_Room4->setColor(vec4(0.6f));
	CQRightWall_Room4->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 102.0f;
	mxT = Translate(vT);
	CQFrontWall_Room4 = new CQuad;
	CQFrontWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room4->setShader();
	CQFrontWall_Room4->setColor(vec4(0.6f));
	CQFrontWall_Room4->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 62.0f;
	mxT = Translate(vT);
	CQBackWall_Room4 = new CQuad;
	CQBackWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room4->setShader();
	CQBackWall_Room4->setColor(vec4(0.6f));
	CQBackWall_Room4->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);



}


void init_Room5() {

	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 41.0f; vT.y = 0.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CSFloor_Room5 = new CQuad;
	CSFloor_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room5->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room5->setColor(vec4(0.6f));
	CSFloor_Room5->setShader();
	CSFloor_Room5->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));

	vT.x = 41.0f; vT.y = 40.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room5 = new CQuad;
	CSCeiling_Room5->setNormal(vceilingNormal);
	CSCeiling_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room5->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room5->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room5->setShader();



	vT.x = 21.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQLeftWall_Room5 = new CQuad;
	CQLeftWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room5->setShader();
	CQLeftWall_Room5->setColor(vec4(0.6f));
	CQLeftWall_Room5->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 61.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQRightWall_Room5 = new CQuad;
	CQRightWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room5->setShader();
	CQRightWall_Room5->setColor(vec4(0.6f));
	CQRightWall_Room5->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 61.0f;
	mxT = Translate(vT);
	CQFrontWall_Room5 = new CQuad;
	CQFrontWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room5->setShader();
	CQFrontWall_Room5->setColor(vec4(0.6f));
	CQFrontWall_Room5->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 21.0f;
	mxT = Translate(vT);
	CQBackWall_Room5 = new CQuad;
	CQBackWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room5->setShader();
	CQBackWall_Room5->setColor(vec4(0.6f));
	CQBackWall_Room5->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);

}


void init_Room6() {
	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 41.0f; vT.y = 0.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	CSFloor_Room6 = new CQuad;
	CSFloor_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room6->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSFloor_Room6->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room6->setColor(vec4(0.6f));
	CSFloor_Room6->setShader();



	vT.x = 41.0f; vT.y = 40.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room6 = new CQuad;
	CSCeiling_Room6->setNormal(vceilingNormal);
	CSCeiling_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room6->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room6->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room6->setShader();




	vT.x = 21.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQLeftWall_Room6 = new CQuad;
	CQLeftWall_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room6->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room6->setShader();
	CQLeftWall_Room6->setColor(vec4(0.6f));
	CQLeftWall_Room6->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 61.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQRightWall_Room6 = new CQuad;
	CQRightWall_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room6->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room6->setShader();
	CQRightWall_Room6->setColor(vec4(0.6f));
	CQRightWall_Room6->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	CQFrontWall_Room6 = new CQuad;
	CQFrontWall_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room6->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room6->setShader();
	CQFrontWall_Room6->setColor(vec4(0.6f));
	CQFrontWall_Room6->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = -20.0f;
	mxT = Translate(vT);
	CQBackWall_Room6 = new CQuad;
	CQBackWall_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room6->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room6->setShader();
	CQBackWall_Room6->setColor(vec4(0.6f));
	CQBackWall_Room6->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	
}




void init( void )
{
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);
	
	auto texturepool = CTexturePool::create();
	g_uiFTexID = texturepool->AddTexture("texture/checker.png");


	init_Room1();
	init_Room2();
	init_Room3();
	init_Room4();
	init_Room5();
	init_Room6();
	
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	
	CQRightWall_Room1->setProjectionMatrix(mpx);
	CQLeftWall_Room1->setProjectionMatrix(mpx);
	CQFrontWall_Room1->setProjectionMatrix(mpx);
	CQBackWall_Room1->setProjectionMatrix(mpx);
	CSFloor_Room1->setProjectionMatrix(mpx);
	CSCeiling_Room1->setProjectionMatrix(mpx);

	CQRightWall_Room2->setProjectionMatrix(mpx);
	CQLeftWall_Room2->setProjectionMatrix(mpx);
	CQFrontWall_Room2->setProjectionMatrix(mpx);
	CQBackWall_Room2->setProjectionMatrix(mpx);
	CSFloor_Room2->setProjectionMatrix(mpx);
	CSCeiling_Room2->setProjectionMatrix(mpx);

	CQRightWall_Room3->setProjectionMatrix(mpx);
	CQLeftWall_Room3->setProjectionMatrix(mpx);
	CQFrontWall_Room3->setProjectionMatrix(mpx);
	CQBackWall_Room3->setProjectionMatrix(mpx);
	CSFloor_Room3->setProjectionMatrix(mpx);
	CSCeiling_Room3->setProjectionMatrix(mpx);

	CQRightWall_Room4->setProjectionMatrix(mpx);
	CQLeftWall_Room4->setProjectionMatrix(mpx);
	CQFrontWall_Room4->setProjectionMatrix(mpx);
	CQBackWall_Room4->setProjectionMatrix(mpx);
	CSFloor_Room4->setProjectionMatrix(mpx);
	CSCeiling_Room4->setProjectionMatrix(mpx);


	CQRightWall_Room5->setProjectionMatrix(mpx);
	CQLeftWall_Room5->setProjectionMatrix(mpx);
	CQFrontWall_Room5->setProjectionMatrix(mpx);
	CQBackWall_Room5->setProjectionMatrix(mpx);
	CSFloor_Room5->setProjectionMatrix(mpx);
	CSCeiling_Room5->setProjectionMatrix(mpx);
	

	CQRightWall_Room6->setProjectionMatrix(mpx);
	CQLeftWall_Room6->setProjectionMatrix(mpx);
	CQFrontWall_Room6->setProjectionMatrix(mpx);
	CQBackWall_Room6->setProjectionMatrix(mpx);
	CSFloor_Room6->setProjectionMatrix(mpx);
	CSCeiling_Room6->setProjectionMatrix(mpx);

	
}

//----------------------------------------------------------------------------
void GL_Display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	

	CSCeiling_Room1->draw();
	CQRightWall_Room1->draw();
	CQLeftWall_Room1->draw();
	CQFrontWall_Room1->draw();
	
	CQBackWall_Room1->draw();


	glBindTexture(GL_TEXTURE_2D, g_uiFTexID);
	CSFloor_Room1->draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	

	/*CQRightWall_Room2->draw();
	CQLeftWall_Room2->draw();
	CQFrontWall_Room2->draw();
	CQBackWall_Room2->draw();
	CSFloor_Room2->draw();
	CSCeiling_Room2->draw();

	CQRightWall_Room3->draw();
	CQLeftWall_Room3->draw();
	CQFrontWall_Room3->draw();
	CQBackWall_Room3->draw();
	CSFloor_Room3->draw();
	CSCeiling_Room3->draw();
	
	CQRightWall_Room4->draw();
	CQLeftWall_Room4->draw();
	CQFrontWall_Room4->draw();
	CQBackWall_Room4->draw();
	CSFloor_Room4->draw();
	CSCeiling_Room4->draw();


	CQRightWall_Room5->draw();
	CQLeftWall_Room5->draw();
	CQFrontWall_Room5->draw();
	CQBackWall_Room5->draw();
	CSFloor_Room5->draw();
	CSCeiling_Room5->draw();


	CQRightWall_Room6->draw();
	CQLeftWall_Room6->draw();
	CQFrontWall_Room6->draw();
	CQBackWall_Room6->draw();
	CSFloor_Room6->draw();
	CSCeiling_Room6->draw();
	*/
	glutSwapBuffers();	// 交換 Frame Buffer
}

//----------------------------------------------------------------------------
// Part 2 : for single light source
void UpdateLightPosition(float dt)
{
	mat4 mxT;
	_fLightDelta += dt;
	_fLightTheta = _fLightDelta * (float)M_PI_2 ; //每一秒轉180度
	if (_fLightTheta>=(float)M_PI*2.0f) {
		_fLightTheta -= (float)M_PI * 2.0f;
		_fLightDelta -= 4.0f;
	}
	Light_resulte_Room1[0].position.x = _fLightRadius * cosf(_fLightTheta);
	Light_resulte_Room1[0].position.z = _fLightRadius * sinf(_fLightTheta);
	
	
}
//----------------------------------------------------------------------------

void onFrameMove(float delta)
{

	g_at = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi) + g_fCameraMoveX,
		g_fRadius * cos(g_fTheta) + g_fCameraMoveY,
		g_fRadius * sin(g_fTheta) * cos(g_fPhi) + g_fCameraMoveZ,
		1.0f);
	g_eye = vec4(g_fCameraMoveX, g_fCameraMoveY, g_fCameraMoveZ, 1.0f);	//第一人稱視角
	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(g_eye, g_at);


	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件

	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		
		CQRightWall_Room1->setViewMatrix(mvx);
		CQLeftWall_Room1->setViewMatrix(mvx);
		CQFrontWall_Room1->setViewMatrix(mvx);
		CQBackWall_Room1->setViewMatrix(mvx);
		CSCeiling_Room1->setViewMatrix(mvx);
		CSFloor_Room1->setViewMatrix(mvx);

		CQRightWall_Room2->setViewMatrix(mvx);
		CQLeftWall_Room2->setViewMatrix(mvx);
		CQFrontWall_Room2->setViewMatrix(mvx);
		CQBackWall_Room2->setViewMatrix(mvx);
		CSFloor_Room2->setViewMatrix(mvx);
		CSCeiling_Room2->setViewMatrix(mvx);

		CQRightWall_Room3->setViewMatrix(mvx);
		CQLeftWall_Room3->setViewMatrix(mvx);
		CQFrontWall_Room3->setViewMatrix(mvx);
		CQBackWall_Room3->setViewMatrix(mvx);
		CSFloor_Room3->setViewMatrix(mvx);
		CSCeiling_Room3->setViewMatrix(mvx);

		CQRightWall_Room4->setViewMatrix(mvx);
		CQLeftWall_Room4->setViewMatrix(mvx);
		CQFrontWall_Room4->setViewMatrix(mvx);
		CQBackWall_Room4->setViewMatrix(mvx);
		CSFloor_Room4->setViewMatrix(mvx);
		CSCeiling_Room4->setViewMatrix(mvx);


		CQRightWall_Room5->setViewMatrix(mvx);
		CQLeftWall_Room5->setViewMatrix(mvx);
		CQFrontWall_Room5->setViewMatrix(mvx);
		CQBackWall_Room5->setViewMatrix(mvx);
		CSFloor_Room5->setViewMatrix(mvx);
		CSCeiling_Room5->setViewMatrix(mvx);


		CQRightWall_Room6->setViewMatrix(mvx);
		CQLeftWall_Room6->setViewMatrix(mvx);
		CQFrontWall_Room6->setViewMatrix(mvx);
		CQBackWall_Room6->setViewMatrix(mvx);
		CSFloor_Room6->setViewMatrix(mvx);
		CSCeiling_Room6->setViewMatrix(mvx);


	}

	
	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}
	// 如果需要重新計算時，在這邊計算每一個物件的顏色


	
	CSFloor_Room1->update(delta, Light_resulte_Room1);
	CSCeiling_Room1->update(delta, Light_resulte_Room1);
	CQRightWall_Room1->update(delta, Light_resulte_Room1);
	CQLeftWall_Room1->update(delta, Light_resulte_Room1);
	CQFrontWall_Room1->update(delta, Light_resulte_Room1);
	CQBackWall_Room1->update(delta, Light_resulte_Room1);

	CQRightWall_Room2->update(delta, Light_resulte_Room2);
	CQLeftWall_Room2->update(delta, Light_resulte_Room2);
	CQFrontWall_Room2->update(delta, Light_resulte_Room2);
	CQBackWall_Room2->update(delta, Light_resulte_Room2);
	CSFloor_Room2->update(delta, Light_resulte_Room2);
	CSCeiling_Room2->update(delta, Light_resulte_Room2);


	CQRightWall_Room3->update(delta, Light_resulte_Room3);
	CQLeftWall_Room3->update(delta, Light_resulte_Room3);
	CQFrontWall_Room3->update(delta, Light_resulte_Room3);
	CQBackWall_Room3->update(delta, Light_resulte_Room3);
	CSFloor_Room3->update(delta, Light_resulte_Room3);
	CSCeiling_Room3->update(delta, Light_resulte_Room3);

	CQRightWall_Room4->update(delta, Light_resulte_Room4);
	CQLeftWall_Room4->update(delta, Light_resulte_Room4);
	CQFrontWall_Room4->update(delta, Light_resulte_Room4);
	CQBackWall_Room4->update(delta, Light_resulte_Room4);
	CSFloor_Room4->update(delta, Light_resulte_Room4);
	CSCeiling_Room4->update(delta, Light_resulte_Room4);
	

	CQRightWall_Room5->update(delta, Light_resulte_Room5);
	CQLeftWall_Room5->update(delta, Light_resulte_Room5);
	CQFrontWall_Room5->update(delta, Light_resulte_Room5);
	CQBackWall_Room5->update(delta, Light_resulte_Room5);
	CSFloor_Room5->update(delta, Light_resulte_Room5);
	CSCeiling_Room5->update(delta, Light_resulte_Room5);


	CQRightWall_Room6->update(delta, Light_resulte_Room6);
	CQLeftWall_Room6->update(delta, Light_resulte_Room6);
	CQFrontWall_Room6->update(delta, Light_resulte_Room6);
	CQBackWall_Room6->update(delta, Light_resulte_Room6);
	CSFloor_Room6->update(delta, Light_resulte_Room6);
	CSCeiling_Room6->update(delta, Light_resulte_Room6);

	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case 65://A
	case 97://a
		//燈光旋轉
		g_bAutoRotating = !g_bAutoRotating;
		break;

	case 82: //R
		if (Light_resulte_Room1[0].diffuse.x < 1.0f)
			Light_resulte_Room1[0].diffuse.x += 0.05f;
		else
			Light_resulte_Room1[0].diffuse.x = 0.95f;
		break;
	case 114: //r
		if (Light_resulte_Room1[0].diffuse.x > 0.0f)
			Light_resulte_Room1[0].diffuse.x -= 0.05f;
		else
			Light_resulte_Room1[0].diffuse.x = 0.0f;
		break;

	case 71: //G
		if (Light_resulte_Room1[0].diffuse.y < 1.0f)
			Light_resulte_Room1[0].diffuse.y += 0.05f;
		else
			Light_resulte_Room1[0].diffuse.y = 0.95f;
		break;

	case 103: //g
		if (Light_resulte_Room1[0].diffuse.y > 0.0f)
			Light_resulte_Room1[0].diffuse.y -= 0.05f;
		else
			Light_resulte_Room1[0].diffuse.y = 0.0f;
		break;

	case 66: //B
		if (Light_resulte_Room1[0].diffuse.z < 1.0f)
			Light_resulte_Room1[0].diffuse.z += 0.05f;
		else
			Light_resulte_Room1[0].diffuse.z = 0.95f;
		break;

	case 98: //b
		if (Light_resulte_Room1[0].diffuse.z > 0.0f)
			Light_resulte_Room1[0].diffuse.z -= 0.05f;
		else
			Light_resulte_Room1[0].diffuse.z = 0.0f;
		break;


	case 80://P
	case 112://p
		printf("( %f , %f , %f )\n", eye.x, eye.y, eye.z);
		break;

    case 033:
		glutIdleFunc(NULL);
		delete CSFloor_Room1;
		delete CSCeiling_Room1;
		delete CQRightWall_Room1;
		delete CQLeftWall_Room1;
		delete CQFrontWall_Room1;
		delete CQBackWall_Room1;
		
		delete CSCeiling_Room2;
		delete CSFloor_Room2;
		delete CQRightWall_Room2;
		delete CQLeftWall_Room2;
		delete CQFrontWall_Room2;
		delete CQBackWall_Room2;

		delete CSCeiling_Room3;
		delete CSFloor_Room3;
		delete CQRightWall_Room3;
		delete CQLeftWall_Room3;
		delete CQFrontWall_Room3;
		delete CQBackWall_Room3;

		delete CSCeiling_Room4;
		delete CSFloor_Room4;
		delete CQRightWall_Room4;
		delete CQLeftWall_Room4;
		delete CQFrontWall_Room4;
		delete CQBackWall_Room4;

		delete CSCeiling_Room5;
		delete CSFloor_Room5;
		delete CQRightWall_Room5;
		delete CQLeftWall_Room5;
		delete CQFrontWall_Room5;
		delete CQBackWall_Room5;

		delete CSCeiling_Room6;
		delete CSFloor_Room6;
		delete CQRightWall_Room6;
		delete CQLeftWall_Room6;
		delete CQFrontWall_Room6;
		delete CQBackWall_Room6;
		CCamera::getInstance()->destroyInstance();
		CTexturePool::getInstance()->destroyInstance();
        exit( EXIT_SUCCESS );
        break;
    }
}

inline void ScreenToUICoordinate(int x, int y, vec2& pt) {

	pt.x = 2.0f * (float)x / SCREEN_SIZE - 1.0f;
	pt.y = -2.0f * (float)y / SCREEN_SIZE + 1.0f;
}


//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	



}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	
	front = normalize(at-eye);
	right = normalize(cross(front, g_vUp));
	

	switch (key)
	{
	case GLUT_KEY_UP:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);
		
			//if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			//if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);
		
			//避免穿牆
			/*if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		
		break;
	case GLUT_KEY_DOWN:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);
		
			//if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			//if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);
			
			//避免穿牆
			/*if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		
		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);
			
			//if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			//if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);
		
			//避免穿牆
			/*if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		
		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			//if (g_fCameraMoveX <= 18.5f && g_fCameraMoveX >= -18.5f)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			//if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			/*if (g_fCameraMoveX > 18.5) g_fCameraMoveX = 18.5f;
			else if (g_fCameraMoveX < -18.5) g_fCameraMoveX = -18.5f;
			if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		break;



	default:
		break;
	}

	


	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(eye, at);
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {
	g_fPhi = (float)-M_PI * (x - HALF_SIZE) / (HALF_SIZE); // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI * (float)y / SCREEN_SIZE;
	at = point4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);

	auto camera = CCamera::getInstance();
	camera->updateLookAt(at);
	
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
	g_fPhi = (float)-M_PI * (x - HALF_SIZE) / (HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI * (float)y / SCREEN_SIZE;
	at = point4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);

	auto camera = CCamera::getInstance();
	camera->updateLookAt(at);
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE,SCREEN_SIZE);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Lighting & Camera Control");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);
	glutKeyboardFunc(Win_Keyboard);	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc(Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);

	glutMainLoop();
	return 0;

	
}