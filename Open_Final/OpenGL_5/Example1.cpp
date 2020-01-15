#include "header/Angel.h"
#include "Common/TypeDefine.h"
#include "Common/CQuad.h"
#include "Common/CSmoothQuad.h"
#include "Common/CObjReader.h"
#include "Common/C2DBTN.h"
#include "Common/CSoildCube.h"
//#include "Common/test.h"
#include "Common/CCamera.h"
#include "Common/CSolidSphere.h"
#include "Common/CTexturePool.h"
#include "png_loader.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 600
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 40 // must be an even number

//test
bool bGoFree = true; //無限制走動


//UI介面
CQuad* CQ_uiBtn[4];



//遊戲變數
bool bfinish = false; //結束第一關遊戲


GLuint g_uiFTexID_Room1[28]; //貼圖
GLuint g_uiLightTexID_Room1; //light map 貼圖
GLuint g_uiNormalTexID_Room1; //normal map 貼圖
GLuint g_uiSphereCubeMap; //enviroment map 貼圖


GLuint g_uiFTexID_Room2[12]; //貼圖
GLuint g_uiFTexID_Room3[3]; //貼圖
GLuint g_uiNormalTexID_Room3; //normal map 貼圖
GLuint g_uiFTexID_Room4[4]; //貼圖
GLuint g_uiFTexID_Room5[2]; //貼圖
GLuint g_uiFTexID_Room6; //貼圖


//skybox
CQuad* CSFloor_skybox;
CQuad* CSCeiling_skybox;
CQuad* CQRightWall_skybox;
CQuad* CQLeftWall_skybox;
CQuad* CQBackWall_skybox;
CQuad* CQFrontWall_skybox;
GLuint g_uiFTexID_skybox[6]; //貼圖
CSolidSphere* CSSphere; //environment map

							  
//model
//CObjReader* g_tank; //坦克車
//CObjReader* g_Target; //標靶
//CObjReader* g_gun; //槍
//CObjReader* g_cat; //貓
CObjReader* g_balcony; //陽台
CObjReader* g_track; //卡車



//Room1 Wall
CQuad* CSFloor_Room1;
CQuad* CSCeiling_Room1;
	 
CQuad* CQRightWall_Room1;
CQuad* CQLeftWall_Room1;
CQuad* CQFrontWall_Room1;
CQuad* CQBackWall_Room1;

//Room1 decorate
CQuad* CSCube_glass; //test glass texture
CQuad* CQDoor1_Room1; //door
CQuad* CQDoor2_Room1; //door


//Room2 Wall
CQuad* CSFloor_Room2;
CQuad* CSCeiling_Room2;
CQuad* CQRightWall_Room2;
CQuad* CQLeftWall_Room2;
CQuad* CQBackWall_Room2;
CQuad* CQFrontWall_Room2;

//Room2 decorate
CSolidCube* CSDoor2; //door
CSolidSphere* g_Earth; //地球
CSolidSphere* g_Sun; //太陽
CSolidSphere* g_Moon; //月亮



//Room3 Wall
CQuad* CSFloor_Room3;
CQuad* CSCeiling_Room3;
CQuad* CQRightWall_Room3;
CQuad* CQLeftWall_Room3;
CQuad* CQBackWall_Room3;
CQuad* CQFrontWall_Room3;

//Room3 decorate
CSolidCube* CSDoor3; //door

//Room4 Wall
CQuad* CSFloor_Room4;
CQuad* CSCeiling_Room4;
CQuad* CQRightWall_Room4;
CQuad* CQLeftWall_Room4;
CQuad* CQBackWall_Room4;
CQuad* CQFrontWall_Room4;

//Room4 decorate
CSolidCube* CSDoor4; //door


//Room5 Wall
CQuad* CSFloor_Room5;
CQuad* CSCeiling_Room5;
CQuad* CQRightWall_Room5;
CQuad* CQLeftWall_Room5;
CQuad* CQBackWall_Room5;
CQuad* CQFrontWall_Room5;

//Room5 decorate
CSolidCube* CSDoor5; //door

//Room6 Wall
CQuad* CSFloor_Room6;
CQuad* CSCeiling_Room6;

CQuad* CQRightWall_Room6;
CQuad* CQLeftWall_Room6;
CQuad* CQFrontWall_Room6;
CQuad* CQBackWall_Room6;

//Room6 decorate
CQuad* CQDoor1_Room6; //door
CQuad* CQDoor2_Room6; //door


GLfloat g_fRadius = 120.0; //視覺範圍
GLfloat g_fTheta = 60.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;
GLfloat g_fCameraMoveX = 41.f;				// for camera movment
GLfloat g_fCameraMoveY = 7.0f;				// for camera movment
GLfloat g_fCameraMoveZ = 0.0f;				// for camera movment
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

float g_fLightR = 0.8f;
float g_fLightG = 0.8f;
float g_fLightB = 0.8f;




LightSource  Light_center_Room1 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(1.9f, 1.9f, 1.9f, 1.0f), // diffuse
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
	color4(1.8, 1.8, 1.8, 1.0f), // diffuse
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
	color4(1.8, 1.8, 1.8, 1.0f), // diffuse
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
	color4(1.5f, 1.5f, 1.5f, 1.0f), // diffuse
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
	color4(10.0, 0.0, 0.0, 1.0f), // diffuse
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
	color4(10.0, 0.0, 0.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(-10.4, 17.0, -6.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 10.0f),		  //spotTarget
	vec3(-10.4, 0.0, 6.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.95f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource  _Light3 = {

	color4(0.0, 0.0, 0.0, 1.0f), // ambient 
	color4(0.0, 0.0, 10.0, 1.0f), // diffuse
	color4(0.0, 0.0, 0.0, 1.0f), // specular
	point4(10.4, 17.0, 6.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(10.0f, 0.0f, 6.0f),		  //spotTarget
	vec3(10.0f, 0.0f, 6.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.95f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};


LightSource  Light_environment = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(1.9f, 1.9f, 1.9f, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(40.0f, 5.0f, -500.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(_fLightRadius, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource Light_resulte_Room1[LIGHT_NUM_MAX] = { //最後燈光結果

	Light_center_Room1, _Light3 ,  _Light2 ,  _Light1
};

LightSource Light_resulte_Room2[LIGHT_NUM_MAX] = { //最後燈光結果

	Light_center_Room2, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_Room3[LIGHT_NUM_MAX] = { //最後燈光結果

	Light_center_Room3, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_Room4[LIGHT_NUM_MAX] = { //最後燈光結果

	Light_center_Room4, _Light1 ,  _Light2 ,  _Light3
};


LightSource Light_resulte_Room5[LIGHT_NUM_MAX] = { //最後燈光結果

	Light_center_Room5, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_Room6[LIGHT_NUM_MAX] = { //最後燈光結果

	Light_center_Room6, _Light1 ,  _Light2 ,  _Light3
};

LightSource Light_resulte_enviroment[LIGHT_NUM_MAX] = { //最後燈光結果

	Light_environment, Light_center_Room6 ,  _Light2 ,  _Light3
};




// 函式的原型宣告
extern void IdleProcess();

void init_Room1() {

	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 0.0f; vT.y = 0.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	CSFloor_Room1 = new CQuad;
	CSFloor_Room1->SetTextureLayer(DIFFUSE_MAP|LIGHT_MAP);
	CSFloor_Room1->SetTiling(4, 4);
	CSFloor_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room1->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSFloor_Room1->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room1->setColor(vec4(0.6f));
	CSFloor_Room1->SetIlightNUM(4);
	CSFloor_Room1->setShader();
	
	

	vT.x = 0.0f; vT.y = 40.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room1 = new CQuad;
	CSCeiling_Room1->SetTextureLayer(1);
	CSCeiling_Room1->SetTiling(4, 4);
	CSCeiling_Room1->setNormal(vceilingNormal);
	CSCeiling_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room1->setTRSMatrix(mxT * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room1->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room1->setShader();




	vT.x = -20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQLeftWall_Room1 = new CQuad;
	CQLeftWall_Room1->SetTextureLayer(DIFFUSE_MAP);
	//CQLeftWall_Room1->setShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	//CQLeftWall_Room1->SetTiling(2, 2);
	CQLeftWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room1->setShader();
	CQLeftWall_Room1->setColor(vec4(0.6f));
	CQLeftWall_Room1->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(-90.0f)  * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQRightWall_Room1 = new CQuad;
	CQRightWall_Room1->SetTextureLayer(DIFFUSE_MAP);
	//CQRightWall_Room1->SetTiling(2, 2);
	CQRightWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room1->setShader();
	CQRightWall_Room1->setColor(vec4(0.6f));
	CQRightWall_Room1->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	CQFrontWall_Room1 = new CQuad;
	CQFrontWall_Room1->SetTextureLayer(1);
	//CQFrontWall_Room1->SetTiling(2, 2);
	CQFrontWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room1->setShader();
	CQFrontWall_Room1->setColor(vec4(0.6f));
	CQFrontWall_Room1->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = -20.0f;
	mxT = Translate(vT);
	CQBackWall_Room1 = new CQuad;
	CQBackWall_Room1->SetTextureLayer(DIFFUSE_MAP);
	//CQBackWall_Room1->SetTiling(2, 2);
	CQBackWall_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room1->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room1->setShader();
	CQBackWall_Room1->setColor(vec4(0.6f));
	CQBackWall_Room1->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 10.0f; vT.z = 19.5f;
	mxT = Translate(vT);
	CQDoor1_Room1 = new CQuad;
	CQDoor1_Room1->SetTextureLayer(DIFFUSE_MAP);
	CQDoor1_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQDoor1_Room1->setShadingMode(GOURAUD_SHADING);
	CQDoor1_Room1->setShader();
	CQDoor1_Room1->setColor(vec4(0.6f));
	CQDoor1_Room1->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(13.0f, 4.0f, 13.0f));
	CQDoor1_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);



	vT.x = 19.5f; vT.y = 10.0f; vT.z = 0.5f;
	mxT = Translate(vT);
	CQDoor2_Room1 = new CQuad;
	CQDoor2_Room1->SetTextureLayer(DIFFUSE_MAP);
	CQDoor2_Room1->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQDoor2_Room1->setShadingMode(GOURAUD_SHADING);
	CQDoor2_Room1->setShader();
	CQDoor2_Room1->setColor(vec4(0.6f));
	CQDoor2_Room1->setTRSMatrix(mxT* RotateY(90.0f)* RotateX(-90.0f)* Scale(13.0f, 4.0f, 13.0f));
	CQDoor2_Room1->setKaKdKsShini(0, 0.8f, 0.5f, 1);

	

}

void init_Room2() {


	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 0.0f; vT.y = 0.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CSFloor_Room2 = new CQuad;
	CSFloor_Room2->SetTextureLayer(DIFFUSE_MAP);
	CSFloor_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSFloor_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSFloor_Room2->setShadingMode(GOURAUD_SHADING);
	CSFloor_Room2->setColor(vec4(0.6f));
	CSFloor_Room2->setShader();
	CSFloor_Room2->setTRSMatrix(mxT * RotateY(90.0f)* Scale(40.0f, 1, 40.0f));

	vT.x = 0.0f; vT.y = 40.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_Room2 = new CQuad;
	CSCeiling_Room2->SetTextureLayer(DIFFUSE_MAP);
	CSCeiling_Room2->setNormal(vceilingNormal);
	CSCeiling_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCeiling_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CSCeiling_Room2->setTRSMatrix(mxT * RotateY(180.0f) * Scale(40.0f, 1, 40.0f));
	CSCeiling_Room2->setShadingMode(GOURAUD_SHADING);
	CSCeiling_Room2->setShader();



	vT.x = -20.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQLeftWall_Room2 = new CQuad;
	CQLeftWall_Room2->SetTextureLayer(DIFFUSE_MAP);
	CQLeftWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room2->setShader();
	CQLeftWall_Room2->setColor(vec4(0.6f));
	CQLeftWall_Room2->setTRSMatrix(mxT  * RotateX(90.0f) * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	


	vT.x = 20.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQRightWall_Room2 = new CQuad;
	CQRightWall_Room2->SetTextureLayer(DIFFUSE_MAP);
	CQRightWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room2->setShader();
	CQRightWall_Room2->setColor(vec4(0.6f));
	CQRightWall_Room2->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 61.0f;
	mxT = Translate(vT);
	CQFrontWall_Room2 = new CQuad;
	CQFrontWall_Room2->SetTextureLayer(DIFFUSE_MAP);
	CQFrontWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room2->setShader();
	CQFrontWall_Room2->setColor(vec4(0.6f));
	CQFrontWall_Room2->setTRSMatrix(mxT * RotateZ(180.0f) * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 21.0f;
	mxT = Translate(vT);
	CQBackWall_Room2 = new CQuad;
	CQBackWall_Room2->SetTextureLayer(DIFFUSE_MAP);
	CQBackWall_Room2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room2->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room2->setShader();
	CQBackWall_Room2->setColor(vec4(0.6f));
	CQBackWall_Room2->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	
	vT.x = 0.0f; vT.y = 13.0f; vT.z = 62.0f;
	mxT = Translate(vT);
	CSDoor2 = new CSolidCube;
	CSDoor2->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSDoor2->setShadingMode(GOURAUD_SHADING);
	CSDoor2->setShader();
	CSDoor2->setColor(vec4(0.6f));
	CSDoor2->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(12.0f, 4.0f, 28.0f));
	CSDoor2->setKaKdKsShini(0, 0.8f, 0.5f, 1);


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
	CSFloor_Room3->SetTextureLayer(DIFFUSE_MAP);
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
	CSCeiling_Room3->SetTextureLayer(DIFFUSE_MAP);
	CSCeiling_Room3->setShader();



	vT.x = -20.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQLeftWall_Room3 = new CQuad;
	CQLeftWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room3->SetTextureLayer(DIFFUSE_MAP );
	CQLeftWall_Room3->setShader();
	CQLeftWall_Room3->SetTiling(2, 2);
	CQLeftWall_Room3->setColor(vec4(0.6f));
	CQLeftWall_Room3->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 20.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQRightWall_Room3 = new CQuad;
	CQRightWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room3->SetTextureLayer(DIFFUSE_MAP);
	CQRightWall_Room3->SetTiling(2, 2);
	CQRightWall_Room3->setShader();
	CQRightWall_Room3->setColor(vec4(0.6f));
	CQRightWall_Room3->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 102.0f;
	mxT = Translate(vT);
	CQFrontWall_Room3 = new CQuad;
	CQFrontWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room3->SetTextureLayer(DIFFUSE_MAP);
	CQFrontWall_Room3->SetTiling(2, 2);
	CQFrontWall_Room3->setShader();
	CQFrontWall_Room3->setColor(vec4(0.6f));
	CQFrontWall_Room3->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 0.0f; vT.y = 20.0f; vT.z = 62.0f;
	mxT = Translate(vT);
	CQBackWall_Room3 = new CQuad;
	CQBackWall_Room3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room3->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room3->SetTextureLayer(DIFFUSE_MAP);
	CQBackWall_Room3->SetTiling(2, 2);
	CQBackWall_Room3->setShader();
	CQBackWall_Room3->setColor(vec4(0.6f));
	CQBackWall_Room3->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room3->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	
	vT.x = 20.5f; vT.y = 13.0f; vT.z = 82.5f;
	mxT = Translate(vT);
	CSDoor3 = new CSolidCube;
	CSDoor3->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSDoor3->setShadingMode(GOURAUD_SHADING);
	CSDoor3->setShader();
	CSDoor3->setColor(vec4(0.6f));
	CSDoor3->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(4.0f, 12.0f, 28.0f));
	CSDoor3->setKaKdKsShini(0, 0.8f, 0.5f, 1);

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
	CSFloor_Room4->SetTextureLayer(DIFFUSE_MAP);
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
	CSCeiling_Room4->SetTextureLayer(DIFFUSE_MAP);
	CSCeiling_Room4->setShader();



	vT.x = 21.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQLeftWall_Room4 = new CQuad;
	CQLeftWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room4->SetTextureLayer(DIFFUSE_MAP);
	CQLeftWall_Room4->setShader();
	CQLeftWall_Room4->setColor(vec4(0.6f));
	CQLeftWall_Room4->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 61.0f; vT.y = 20.0f; vT.z = 82.0f;
	mxT = Translate(vT);
	CQRightWall_Room4 = new CQuad;
	CQRightWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room4->SetTextureLayer(DIFFUSE_MAP);
	CQRightWall_Room4->setShader();
	CQRightWall_Room4->setColor(vec4(0.6f));
	CQRightWall_Room4->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 102.0f;
	mxT = Translate(vT);
	CQFrontWall_Room4 = new CQuad;
	CQFrontWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room4->SetTextureLayer(DIFFUSE_MAP);
	CQFrontWall_Room4->setShader();
	CQFrontWall_Room4->setColor(vec4(0.6f));
	CQFrontWall_Room4->setTRSMatrix(mxT * RotateZ(180.0f) * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 62.0f;
	mxT = Translate(vT);
	CQBackWall_Room4 = new CQuad;
	CQBackWall_Room4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room4->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room4->SetTextureLayer(DIFFUSE_MAP);
	CQBackWall_Room4->setShader();
	CQBackWall_Room4->setColor(vec4(0.6f));
	CQBackWall_Room4->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room4->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 13.0f; vT.z = 62.0f;
	mxT = Translate(vT);
	CSDoor4 = new CSolidCube;
	CSDoor4->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSDoor4->setShadingMode(GOURAUD_SHADING);
	CSDoor4->setShader();
	CSDoor4->setColor(vec4(0.6f));
	CSDoor4->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(12.0f, 4.0f, 28.0f));
	CSDoor4->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	

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
	CSFloor_Room5->SetTextureLayer(DIFFUSE_MAP);
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
	CSCeiling_Room5->SetTextureLayer(DIFFUSE_MAP);
	CSCeiling_Room5->setShader();



	vT.x = 21.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQLeftWall_Room5 = new CQuad;
	CQLeftWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room5->SetTextureLayer(DIFFUSE_MAP);
	CQLeftWall_Room5->setShader();
	CQLeftWall_Room5->setColor(vec4(0.6f));
	CQLeftWall_Room5->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 61.0f; vT.y = 20.0f; vT.z = 41.0f;
	mxT = Translate(vT);
	CQRightWall_Room5 = new CQuad;
	CQRightWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQRightWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQRightWall_Room5->SetTextureLayer(DIFFUSE_MAP);
	CQRightWall_Room5->setShader();
	CQRightWall_Room5->setColor(vec4(0.6f));
	CQRightWall_Room5->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQRightWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 61.0f;
	mxT = Translate(vT);
	CQFrontWall_Room5 = new CQuad;
	CQFrontWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQFrontWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_Room5->SetTextureLayer(DIFFUSE_MAP);
	CQFrontWall_Room5->setShader();
	CQFrontWall_Room5->setColor(vec4(0.6f));
	CQFrontWall_Room5->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 41.0f; vT.y = 20.0f; vT.z = 21.0f;
	mxT = Translate(vT);
	CQBackWall_Room5 = new CQuad;
	CQBackWall_Room5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room5->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room5->SetTextureLayer(DIFFUSE_MAP);
	CQBackWall_Room5->setShader();
	CQBackWall_Room5->setColor(vec4(0.6f));
	CQBackWall_Room5->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQBackWall_Room5->setKaKdKsShini(0, 0.8f, 0.5f, 1);



	vT.x = 41.0f; vT.y = 13.0f; vT.z = 21.0f;
	mxT = Translate(vT);
	CSDoor5 = new CSolidCube;
	CSDoor5->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSDoor5->setShadingMode(GOURAUD_SHADING);
	CSDoor5->setShader();
	CSDoor5->setColor(vec4(0.6f));
	CSDoor5->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(12.0f, 4.0f, 28.0f));
	CSDoor5->setKaKdKsShini(0, 0.8f, 0.5f, 1);
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
	CSFloor_Room6->SetTextureLayer(DIFFUSE_MAP);
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
	CSCeiling_Room6->SetTextureLayer(DIFFUSE_MAP);
	CSCeiling_Room6->setShader();




	vT.x = 21.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQLeftWall_Room6 = new CQuad;
	CQLeftWall_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQLeftWall_Room6->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_Room6->SetTextureLayer(DIFFUSE_MAP);
	CQLeftWall_Room6->setShader();
	CQLeftWall_Room6->setColor(vec4(0.6f));
	CQLeftWall_Room6->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQLeftWall_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 61.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQRightWall_Room6 = new CQuad;
	CQRightWall_Room6->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 0.5f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
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
	CQFrontWall_Room6->SetTextureLayer(DIFFUSE_MAP);
	CQFrontWall_Room6->setShader();
	CQFrontWall_Room6->setColor(vec4(0.6f));
	CQFrontWall_Room6->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQFrontWall_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 33.5f; vT.y = 20.0f; vT.z = -20.0f;
	mxT = Translate(vT);
	CQBackWall_Room6 = new CQuad;
	CQBackWall_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQBackWall_Room6->setShadingMode(GOURAUD_SHADING);
	CQBackWall_Room6->SetTextureLayer(DIFFUSE_MAP);
	CQBackWall_Room6->setShader();
	CQBackWall_Room6->setColor(vec4(0.6f));
	CQBackWall_Room6->setTRSMatrix(mxT * RotateX(90.0f) * Scale(25.0f, 1, 40.0f));
	CQBackWall_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);

	
	
	vT.x = 21.5f; vT.y = 10.0f; vT.z = 0.5f;
	mxT = Translate(vT);
	CQDoor1_Room6 = new CQuad;
	CQDoor1_Room6->SetTextureLayer(DIFFUSE_MAP);
	CQDoor1_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQDoor1_Room6->setShadingMode(GOURAUD_SHADING);
	CQDoor1_Room6->setShader();
	CQDoor1_Room6->setColor(vec4(0.6f));
	CQDoor1_Room6->setTRSMatrix(mxT * RotateY(90.0f) * RotateX(90.0f) * Scale(13.0f, 4.0f, 13.0f));
	CQDoor1_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);



	vT.x = 41.0f; vT.y = 10.0f; vT.z = 19.5f;
	mxT = Translate(vT);
	CQDoor2_Room6 = new CQuad;
	CQDoor2_Room6->SetTextureLayer(DIFFUSE_MAP);
	CQDoor2_Room6->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.0f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQDoor2_Room6->setShadingMode(GOURAUD_SHADING);
	CQDoor2_Room6->setShader();
	CQDoor2_Room6->setColor(vec4(0.6f));
	CQDoor2_Room6->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(13.0f, 4.0f, 13.0f));
	CQDoor2_Room6->setKaKdKsShini(0, 0.8f, 0.5f, 1);

}


void init_UI(void) {
	mat4 mxT, mxS;
	vec4 vT, vColor;
	
	/*vT.x = 0.0f; vT.y = -0.5f; vT.z = 0.0f;
	mxT = Translate(vT);
	CQ_uiBtn[0] = new CQuad;
	CQ_uiBtn[0]->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_uiBtn[0]->setShadingMode(GOURAUD_SHADING);
	CQ_uiBtn[0]->SetTextureLayer(DIFFUSE_MAP);
	CQ_uiBtn[0]->setShader();
	CQ_uiBtn[0]->setColor(vec4(0.6f));
	CQ_uiBtn[0]->setTRSMatrix(RotateY(90) * mxT  * Scale(2.0f, 0.1f, 2.0f));
	CQ_uiBtn[0]->setKaKdKsShini(0, 0.8f, 0.5f, 1);*/

}


void init_skybox() {


	float skysize = 500; //天空大小
	float transform_x = 41.0f;
	float transform_y = -(skysize/2);
	vec4 ambient(100.0f, 100.0f, 100.0f );
	vec4 specular(0.0f, 0.0f, 0.0f);

	mat4 mxT, mxS;
	vec4 vT, vColor;

	vT.x = 0.0f + transform_x; vT.y = 0.0f + transform_y; vT.z = 0.0f;
	mxT = Translate(vT);
	CSFloor_skybox = new CQuad;
	CSFloor_skybox->setShaderName("vsSkyBox.glsl", "fsSkyBox.glsl");
	CSFloor_skybox->SetTextureLayer(DIFFUSE_MAP);
	CSFloor_skybox->setMaterials(ambient, vec4(0.85f, 0.85f, 0.85f, 1), specular);
	CSFloor_skybox->setKaKdKsShini(0, 0.8, 0, 0);
	CSFloor_skybox->setShadingMode(GOURAUD_SHADING);
	CSFloor_skybox->setColor(vec4(0.6f));
	CSFloor_skybox->setShader();
	CSFloor_skybox->setTRSMatrix(mxT * RotateY(90.0f) * Scale(skysize, 1, skysize));

	vT.x = 0.0f + transform_x; vT.y = skysize + transform_y; vT.z = 0.0f;
	mxT = Translate(vT);
	vec3 vceilingNormal = vec3(0.0f, -1.0f, 0.0f);
	CSCeiling_skybox = new CQuad;
	CSCeiling_skybox->setShaderName("vsSkyBox.glsl", "fsSkyBox.glsl");
	CSCeiling_skybox->SetTextureLayer(DIFFUSE_MAP);
	CSCeiling_skybox->setNormal(vceilingNormal);
	CSCeiling_skybox->setMaterials(ambient, vec4(0.85f, 0.85f, 0.85f, 1), specular);
	CSCeiling_skybox->setKaKdKsShini(0, 0.8, 0, 0);
	CSCeiling_skybox->setTRSMatrix(mxT * RotateY(90.0f) * RotateX(180.0f) * Scale(skysize, 1, skysize));
	CSCeiling_skybox->setShadingMode(GOURAUD_SHADING);
	CSCeiling_skybox->setShader();



	vT.x = -(skysize / 2) + transform_x; vT.y = (skysize/2) + transform_y; vT.z =0.0f;
	mxT = Translate(vT);
	CQLeftWall_skybox = new CQuad;
	CQLeftWall_skybox->setShaderName("vsSkyBox.glsl", "fsSkyBox.glsl");
	CQLeftWall_skybox->SetTextureLayer(DIFFUSE_MAP);
	CQLeftWall_skybox->setMaterials(ambient, vec4(0.85f, 0.85f, 0.85f, 1), specular);
	CQLeftWall_skybox->setShadingMode(GOURAUD_SHADING);
	CQLeftWall_skybox->setShader();
	CQLeftWall_skybox->setColor(vec4(0.6f));
	CQLeftWall_skybox->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(-90.0f) * Scale(skysize, 1, skysize));
	CQLeftWall_skybox->setKaKdKsShini(0, 0.8, 0, 0);



	vT.x = (skysize / 2) + transform_x; vT.y = (skysize / 2) + transform_y; vT.z = 0.0f;
	mxT = Translate(vT);
	CQRightWall_skybox = new CQuad;
	CQRightWall_skybox->setShaderName("vsSkyBox.glsl", "fsSkyBox.glsl");
	CQRightWall_skybox->SetTextureLayer(DIFFUSE_MAP);
	CQRightWall_skybox->setMaterials(ambient, vec4(0.85f, 0.85f, 0.85f, 1), specular);
	CQRightWall_skybox->setShadingMode(GOURAUD_SHADING);
	CQRightWall_skybox->setShader();
	CQRightWall_skybox->setColor(vec4(0.6f));
	CQRightWall_skybox->setTRSMatrix(mxT * RotateX(90.0f) * RotateZ(90.0f) * Scale(skysize, 1, skysize));
	CQRightWall_skybox->setKaKdKsShini(0, 0.8, 0, 0);


	vT.x = 0.0f + transform_x; vT.y = (skysize / 2) + transform_y; vT.z = (skysize / 2);
	mxT = Translate(vT);
	CQFrontWall_skybox = new CQuad;
	CQFrontWall_skybox->setShaderName("vsSkyBox.glsl", "fsSkyBox.glsl");
	CQFrontWall_skybox->SetTextureLayer(DIFFUSE_MAP);
	CQFrontWall_skybox->setMaterials(ambient, vec4(0.85f, 0.85f, 0.85f, 1), specular);
	CQFrontWall_skybox->setShadingMode(GOURAUD_SHADING);
	CQFrontWall_skybox->setShader();
	CQFrontWall_skybox->setColor(vec4(0.6f));
	CQFrontWall_skybox->setTRSMatrix(mxT * RotateZ(180.0f) * RotateX(-90.0f) * Scale(skysize, 1, skysize));
	CQFrontWall_skybox->setKaKdKsShini(0, 0.8, 0, 0);


	vT.x = 0.0f + transform_x; vT.y = (skysize / 2) + transform_y; vT.z = -(skysize / 2);
	mxT = Translate(vT);
	CQBackWall_skybox = new CQuad;
	CQBackWall_skybox->setShaderName("vsSkyBox.glsl", "fsSkyBox.glsl");
	CQBackWall_skybox->SetTextureLayer(DIFFUSE_MAP);
	CQBackWall_skybox->setMaterials(ambient, vec4(0.85f, 0.85f, 0.85f, 1), specular);
	CQBackWall_skybox->setShadingMode(GOURAUD_SHADING);
	CQBackWall_skybox->setShader();
	CQBackWall_skybox->setColor(vec4(0.6f));
	CQBackWall_skybox->setTRSMatrix(mxT * RotateX(90.0f) * Scale(skysize, 1, skysize));
	CQBackWall_skybox->setKaKdKsShini(0, 0.8, 0, 0);



}


void init( void )
{

	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);
	
	auto texturepool = CTexturePool::create();
	g_uiFTexID_Room1[0] = texturepool->AddTexture("texture/Room1/brick_sideWall.png");
	g_uiFTexID_Room1[1] = texturepool->AddTexture("texture/Masonry.Brick.png");
	g_uiFTexID_Room1[2] = texturepool->AddTexture("texture/Room1/tank.png");
	g_uiFTexID_Room1[3] = texturepool->AddTexture("texture/Room1/Labortory.png");
	g_uiFTexID_Room1[4] = texturepool->AddTexture("texture/Room1/brick_ceiling.png");
	g_uiFTexID_Room1[5] = texturepool->AddTexture("texture/Model_texture/track.png");
	g_uiFTexID_Room1[6] = texturepool->AddTexture("texture/Model_texture/track_normal.png");
	g_uiFTexID_Room1[7] = texturepool->AddTexture("texture/Door/1.png");
	g_uiFTexID_Room1[8] = texturepool->AddTexture("texture/Door/2.png");
	g_uiFTexID_Room1[9] = texturepool->AddTexture("texture/Door/3.png");
	g_uiFTexID_Room1[10] = texturepool->AddTexture("texture/Door/4.png");
	g_uiFTexID_Room1[11] = texturepool->AddTexture("texture/Door/5.png");
	g_uiFTexID_Room1[12] = texturepool->AddTexture("texture/Door/6.png");
	g_uiFTexID_Room1[13] = texturepool->AddTexture("texture/Door/7.png");
	g_uiFTexID_Room1[14] = texturepool->AddTexture("texture/Door/8.png");
	g_uiFTexID_Room1[15] = texturepool->AddTexture("texture/Door/9.png");
	g_uiFTexID_Room1[16] = texturepool->AddTexture("texture/Door/10.png");
	g_uiFTexID_Room1[17] = texturepool->AddTexture("texture/Door/11.png");
	g_uiFTexID_Room1[18] = texturepool->AddTexture("texture/Door/12.png");
	g_uiFTexID_Room1[19] = texturepool->AddTexture("texture/Door/13.png");
	g_uiFTexID_Room1[20] = texturepool->AddTexture("texture/Door/14.png");
	g_uiFTexID_Room1[21] = texturepool->AddTexture("texture/Door/15.png");
	g_uiFTexID_Room1[22] = texturepool->AddTexture("texture/Door/16.png");
	g_uiFTexID_Room1[23] = texturepool->AddTexture("texture/Door/17.png");
	g_uiFTexID_Room1[24] = texturepool->AddTexture("texture/Door/18.png");
	g_uiFTexID_Room1[25] = texturepool->AddTexture("texture/Door/19.png");
	g_uiFTexID_Room1[26] = texturepool->AddTexture("texture/Door/20.png");
	g_uiFTexID_Room1[27] = texturepool->AddTexture("texture/Door/21.png");



	g_uiFTexID_Room2[0] = texturepool->AddTexture("texture/Room2/PX.png");
	g_uiFTexID_Room2[1] = texturepool->AddTexture("texture/Room2/PZ.png");
	g_uiFTexID_Room2[2] = texturepool->AddTexture("texture/Room2/NX.png");
	g_uiFTexID_Room2[3] = texturepool->AddTexture("texture/Room2/NZ.png");
	g_uiFTexID_Room2[4] = texturepool->AddTexture("texture/Room2/PY.png");
	g_uiFTexID_Room2[5] = texturepool->AddTexture("texture/Room2/NY.png");
	g_uiFTexID_Room2[6] = texturepool->AddTexture("texture/Model_texture/earth.png");
	g_uiFTexID_Room2[7] = texturepool->AddTexture("texture/Model_texture/Moon.png");
	g_uiFTexID_Room2[8] = texturepool->AddTexture("texture/Model_texture/sun.png");

	g_uiFTexID_skybox[0] = texturepool->AddTexture("texture/SkyBox/Skybox_PX.png");
	g_uiFTexID_skybox[1] = texturepool->AddTexture("texture/SkyBox/Skybox_PZ.png");
	g_uiFTexID_skybox[2] = texturepool->AddTexture("texture/SkyBox/Skybox_NX.png");
	g_uiFTexID_skybox[3] = texturepool->AddTexture("texture/SkyBox/Skybox_NZ.png");
	g_uiFTexID_skybox[4] = texturepool->AddTexture("texture/SkyBox/Skybox_PY.png");
	g_uiFTexID_skybox[5] = texturepool->AddTexture("texture/SkyBox/Skybox_NY.png");

	g_uiFTexID_Room3[0] = texturepool->AddTexture("texture/Room3/brick_wall.png");
	g_uiFTexID_Room3[1] = texturepool->AddTexture("texture/Model_texture/TT.png");
	
	g_uiFTexID_Room4[0] = texturepool->AddTexture("texture/Room4/wallpaper.png");
	g_uiFTexID_Room4[1] = texturepool->AddTexture("texture/Room4/wood_grain.png");
	g_uiFTexID_Room4[2] = texturepool->AddTexture("texture/Model_texture/gun.png");
	g_uiFTexID_Room4[3] = texturepool->AddTexture("texture/Model_texture/gun_normal.png");

	g_uiFTexID_Room5[0] = texturepool->AddTexture("texture/Room5/wallpaper_flower.png");
	g_uiFTexID_Room5[1] = texturepool->AddTexture("texture/Model_texture/cat.png");

	g_uiFTexID_Room6 = texturepool->AddTexture("texture/Room6/wallpaper_leaf.png");

#ifdef LIGHTMAP
	g_uiLightTexID_Room1 = texturepool->AddTexture("texture/Room1/floor_mark.png");
#endif // LIGHTMAP

#ifdef NORMALMAP
	g_uiNormalTexID_Room1 = texturepool->AddTexture("texture/Room1WallNormal_1.png");
#endif // NORMALMAP
#ifdef CUBIC_MAP
	g_uiSphereCubeMap = CubeMap_load_SOIL();
#endif // CUBIC_MAP

	

	mat4 mxT, mxS;
	vec4 vColor ;

	//test glass 
	CSCube_glass = new CQuad;
	CSCube_glass->setShader();
	CSCube_glass->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	CSCube_glass->SetCubeMapTexName(1);
	CSCube_glass->_vT.x = 0.0f; CSCube_glass->_vT.y = 3.0f; CSCube_glass->_vT.z = 6.0f;
	mxT = Translate(CSCube_glass->_vT);
	CSCube_glass->_vS.x = CSCube_glass->_vS.y = CSCube_glass->_vS.z = 3.0f;
	mxS = Scale(CSCube_glass->_vS);
	CSCube_glass->setTRSMatrix(mxT *RotateZ(90.0f)* mxS);
	CSCube_glass->setShadingMode(GOURAUD_SHADING);
	CSCube_glass->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 0.5f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSCube_glass->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	
	CSSphere = new CSolidSphere(1.0f, 24.0f, 12.0f);
	//CSSphere->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	CSSphere->SetCubeMapTexName(1);
	CSSphere->SetViewPosition(eye);
	CSSphere->setShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	CSSphere->setShader();
	CSSphere->_vT.x = 53.0f; CSSphere->_vT.y = 5.0f; CSSphere->_vT.z = -21.5f;
	CSSphere->_vS.x = 1.5f; CSSphere->_vS.y = 1.5f; CSSphere->_vS.z = 1.5f;
	mxT = Translate(CSSphere->_vT);
	mxT._m[0][0] = 1.0f; mxT._m[1][1] = 1.0f; mxT._m[2][2] = 1.0f;
	CSSphere->setTRSMatrix(mxT* RotateY(180.0f)* RotateX(90.0f) * Scale(CSSphere->_vS));
	CSSphere->setShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	CSSphere->setMaterials(vec4(1.0,1.0,1.0,1.0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CSSphere->setKaKdKsShini(0.0f, 1.0f, 0.8f, 1);
	CSSphere->setColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	


	/*g_tank = new CObjReader("Model/test.obj");
	g_tank->SetTextureLayer(DIFFUSE_MAP); 
	g_tank->setShader();
	g_tank->_vT.x = 41.0f; g_tank->_vT.y = 0.0f; g_tank->_vT.z = 82.0f;
	mxT = Translate(g_tank->_vT);
	g_tank->_vS.x = g_tank->_vS.y = g_tank->_vS.z = 1.0f;
	mxS = Scale(g_tank->_vS);
	g_tank->setTRSMatrix(mxT *mxS);
	g_tank->setShadingMode(GOURAUD_SHADING);
	g_tank->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_tank->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	g_tank->SetIlightNUM(2);*/


	
	/*g_gun = new CObjReader("Model/gun.obj");
	g_gun->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_gun->setShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_gun->setShader();
	g_gun->_vT.x = 0.0f; g_gun->_vT.y = 5.0f; g_gun->_vT.z = -10.0f;
	mxT = Translate(g_gun->_vT);
	g_gun->_vS.x = g_gun->_vS.y = g_gun->_vS.z = 1.0f;
	mxS = Scale(g_gun->_vS);
	g_gun->setTRSMatrix(mxT* mxS);
	g_gun->setShadingMode(GOURAUD_SHADING);
	g_gun->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_gun->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	g_gun->SetIlightNUM(2);*/

	//g_Target = new CObjReader("Model/Target.obj");
	//g_Target->SetTextureLayer(DIFFUSE_MAP);
	//g_Target->setShader();
	//g_Target->_vT.x = 0.0f; //g_Target->_vT.y = 0.0f; //g_Target->_vT.z = 98.0f;
	//mxT = Translate(//g_Target->_vT);
	//g_Target->_vS.x = //g_Target->_vS.y = //g_Target->_vS.z = 0.1f;
	//mxS = Scale(//g_Target->_vS);
	//g_Target->setTRSMatrix(mxT* RotateX(-90)* RotateZ(180)* mxS);
	//g_Target->setShadingMode(GOURAUD_SHADING);
	//g_Target->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//g_Target->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	//g_Target->SetIlightNUM(2);


	

	/*g_cat = new CObjReader("Model/cat.obj");
	g_cat->SetTextureLayer(DIFFUSE_MAP);
	g_cat->setShader();
	g_cat->_vT.x = 41.0f; g_cat->_vT.y = 0.0f; g_cat->_vT.z = 41.0f;
	mxT = Translate(g_cat->_vT);
	g_cat->_vS.x = g_cat->_vS.y = g_cat->_vS.z = 0.25f;
	mxS = Scale(g_cat->_vS);
	g_cat->setTRSMatrix(mxT * RotateX(-90)* RotateZ(90)* mxS);
	g_cat->setShadingMode(GOURAUD_SHADING);
	g_cat->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_cat->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	g_cat->SetIlightNUM(2);*/

	
	g_balcony = new CObjReader("Model/balcony.obj");
	g_balcony->SetTextureLayer(DIFFUSE_MAP);
	g_balcony->setShader();
	g_balcony->_vT.x = 53.5f; g_balcony->_vT.y = -3.0f; g_balcony->_vT.z = -20.0f;
	mxT = Translate(g_balcony->_vT);
	g_balcony->_vS.x = g_balcony->_vS.y = g_balcony->_vS.z = 0.009f;
	mxS = Scale(g_balcony->_vS);
	g_balcony->setTRSMatrix(mxT * RotateY(180) * mxS);
	g_balcony->setShadingMode(GOURAUD_SHADING);
	g_balcony->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_balcony->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	g_balcony->SetIlightNUM(2);

	
	g_track = new CObjReader("Model/test.obj");
	g_track->SetTextureLayer(DIFFUSE_MAP );
	g_track->setShader();
	g_track->_vT.x = 10.4f; g_track->_vT.y = 0.0f; g_track->_vT.z = 6.0f;
	mxT = Translate(g_track->_vT);
	g_track->_vS.x = g_track->_vS.y = g_track->_vS.z = 1.0f;
	mxS = Scale(g_track->_vS);
	g_track->setTRSMatrix(mxT* mxS);
	g_track->setShadingMode(GOURAUD_SHADING);
	g_track->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_track->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	g_track->SetIlightNUM(3);

	g_Earth = new CSolidSphere();
	g_Earth->SetTextureLayer(DIFFUSE_MAP);
	g_Earth->setShader();
	g_Earth->_vT.x = 0.0f; g_Earth->_vT.y = 5.0f; g_Earth->_vT.z = 42.0f;
	mxT = Translate(g_Earth->_vT);
	g_Earth->_vS.x = g_Earth->_vS.y = g_Earth->_vS.z = 3.0f;
	mxS = Scale(g_Earth->_vS);
	g_Earth->setTRSMatrix(mxT * mxS);
	g_Earth->setShadingMode(GOURAUD_SHADING);
	g_Earth->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Earth->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	g_Sun = new CSolidSphere();
	g_Sun->SetTextureLayer(DIFFUSE_MAP);
	g_Sun->setShader();
	g_Sun->_vT.x = 0.0f; g_Sun->_vT.y = 5.0f; g_Sun->_vT.z = 48.0f;
	mxT = Translate(g_Sun->_vT);
	g_Sun->_vS.x = g_Sun->_vS.y = g_Sun->_vS.z = 3.0f;
	mxS = Scale(g_Sun->_vS);
	g_Sun->setTRSMatrix(mxT* mxS);
	g_Sun->setShadingMode(GOURAUD_SHADING);
	g_Sun->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Sun->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);


	g_Moon = new CSolidSphere();
	g_Moon->SetTextureLayer(DIFFUSE_MAP);
	g_Moon->setShader();
	g_Moon->_vT.x = 0.0f; g_Moon->_vT.y = 5.0f; g_Moon->_vT.z = 30.0f;
	mxT = Translate(g_Moon->_vT);
	g_Moon->_vS.x = g_Moon->_vS.y = g_Moon->_vS.z = 3.0f;
	mxS = Scale(g_Moon->_vS);
	g_Moon->setTRSMatrix(mxT* mxS);
	g_Moon->setShadingMode(GOURAUD_SHADING);
	g_Moon->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Moon->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	

	
	init_UI();
	init_skybox();
	init_Room1();
	init_Room2();
	init_Room3();
	init_Room4();
	init_Room5();
	init_Room6();
	
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	
	CSSphere->setProjectionMatrix(mpx);
	//g_tank->setProjectionMatrix(mpx);
	////g_Target->setProjectionMatrix(mpx);
	//CSCube_glass->setProjectionMatrix(mpx);
	//g_Sun->setProjectionMatrix(mpx);
	//g_Earth->setProjectionMatrix(mpx);
	//g_Moon->setProjectionMatrix(mpx);
	//g_gun->setProjectionMatrix(mpx);
	//g_cat->setProjectionMatrix(mpx);
	g_balcony->setProjectionMatrix(mpx);
	g_track->setProjectionMatrix(mpx);

	CQRightWall_skybox->setProjectionMatrix(mpx);
	CQLeftWall_skybox->setProjectionMatrix(mpx);
	CQFrontWall_skybox->setProjectionMatrix(mpx);
	CQBackWall_skybox->setProjectionMatrix(mpx);
	CSFloor_skybox->setProjectionMatrix(mpx);
	CSCeiling_skybox->setProjectionMatrix(mpx);



	CQRightWall_Room1->setProjectionMatrix(mpx);
	CQLeftWall_Room1->setProjectionMatrix(mpx);
	CQFrontWall_Room1->setProjectionMatrix(mpx);
	CQBackWall_Room1->setProjectionMatrix(mpx);
	CSFloor_Room1->setProjectionMatrix(mpx);
	CSCeiling_Room1->setProjectionMatrix(mpx);
	CQDoor1_Room1->setProjectionMatrix(mpx);
	CQDoor2_Room1->setProjectionMatrix(mpx);
	

	CQRightWall_Room2->setProjectionMatrix(mpx);
	CQLeftWall_Room2->setProjectionMatrix(mpx);
	CQFrontWall_Room2->setProjectionMatrix(mpx);
	CQBackWall_Room2->setProjectionMatrix(mpx);
	CSFloor_Room2->setProjectionMatrix(mpx);
	CSCeiling_Room2->setProjectionMatrix(mpx);
	CSDoor2->setProjectionMatrix(mpx);

	CQRightWall_Room3->setProjectionMatrix(mpx);
	CQLeftWall_Room3->setProjectionMatrix(mpx);
	CQFrontWall_Room3->setProjectionMatrix(mpx);
	CQBackWall_Room3->setProjectionMatrix(mpx);
	CSFloor_Room3->setProjectionMatrix(mpx);
	CSCeiling_Room3->setProjectionMatrix(mpx);
	CSDoor3->setProjectionMatrix(mpx);

	CQRightWall_Room4->setProjectionMatrix(mpx);
	CQLeftWall_Room4->setProjectionMatrix(mpx);
	CQFrontWall_Room4->setProjectionMatrix(mpx);
	CQBackWall_Room4->setProjectionMatrix(mpx);
	CSFloor_Room4->setProjectionMatrix(mpx);
	CSCeiling_Room4->setProjectionMatrix(mpx);
	CSDoor4->setProjectionMatrix(mpx);

	CQRightWall_Room5->setProjectionMatrix(mpx);
	CQLeftWall_Room5->setProjectionMatrix(mpx);
	CQFrontWall_Room5->setProjectionMatrix(mpx);
	CQBackWall_Room5->setProjectionMatrix(mpx);
	CSFloor_Room5->setProjectionMatrix(mpx);
	CSCeiling_Room5->setProjectionMatrix(mpx);
	CSDoor5->setProjectionMatrix(mpx);

	CQRightWall_Room6->setProjectionMatrix(mpx);
	CQLeftWall_Room6->setProjectionMatrix(mpx);
	CQFrontWall_Room6->setProjectionMatrix(mpx);
	CQBackWall_Room6->setProjectionMatrix(mpx);
	CSFloor_Room6->setProjectionMatrix(mpx);
	CSCeiling_Room6->setProjectionMatrix(mpx);
	CQDoor1_Room6->setProjectionMatrix(mpx);
	CQDoor2_Room6->setProjectionMatrix(mpx);
	
}

//----------------------------------------------------------------------------

int door_txCount = 7; //texture張數
void GL_Display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_skybox[0]);
	CQBackWall_skybox->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_skybox[1]);
	CQLeftWall_skybox->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_skybox[2]);
	CQFrontWall_skybox->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_skybox[3]);
	CQRightWall_skybox->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_skybox[4]);
	CSCeiling_skybox->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_skybox[5]);
	CSFloor_skybox->draw();




	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiSphereCubeMap);
	CSSphere->draw();


	
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[2]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room4[3]);
	g_gun->draw();*/

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[2]);
	g_track->draw();


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[0]);
	CQRightWall_Room1->draw();


	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[0]);
	CQFrontWall_Room1->draw();
	CQBackWall_Room1->draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[3]);
	CQLeftWall_Room1->draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[1]);
	glActiveTexture(GL_TEXTURE1);
	if (bfinish ==false)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
		glBindTexture(GL_TEXTURE_2D, g_uiLightTexID_Room1);
	
	CSFloor_Room1->draw();
	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[4]);
	CSCeiling_Room1->draw();*/

	

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[2]);
	g_tank->draw();*/

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room3[1]);
	g_Target->draw();*/


	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[0]);
	CQBackWall_Room2->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[1]);
	CQLeftWall_Room2->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[2]);
	CQFrontWall_Room2->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[3]);
	CQRightWall_Room2->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[4]);
	CSCeiling_Room2->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[5]);
	CSFloor_Room2->draw();
	CSDoor2->draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[8]);
	g_Sun->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[6]);
	g_Earth->draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room2[7]);
	g_Moon->draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room3[0]);
	CQLeftWall_Room3->draw();
	CQRightWall_Room3->draw();
	CQFrontWall_Room3->draw();
	CQBackWall_Room3->draw();
	CSFloor_Room3->draw();
	CSCeiling_Room3->draw();

	CSDoor3->draw();
	
	
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room4[0]);
	CQRightWall_Room4->draw();
	CQLeftWall_Room4->draw();
	CQFrontWall_Room4->draw();
	CQBackWall_Room4->draw();
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room4[1]);
	CSFloor_Room4->draw();
	CSCeiling_Room4->draw();
	CSDoor4->draw();

	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room5[0]);
	CQRightWall_Room5->draw();
	CQLeftWall_Room5->draw();
	CQFrontWall_Room5->draw();
	CQBackWall_Room5->draw();
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room4[1]);
	CSFloor_Room5->draw();
	CSCeiling_Room5->draw();
	CSDoor5->draw();

	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room5[1]);
	g_cat->draw();*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room6);
	CQLeftWall_Room6->draw();
	CQFrontWall_Room6->draw();
	CQBackWall_Room6->draw();
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room4[1]);
	CSFloor_Room6->draw();
	CSCeiling_Room6->draw();
	
	
	g_balcony->draw();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //blend GL_ONE_MINUS_SRC_ALPHA ：(1,1,1,1)-(sa , sa , sa , sa)
	//glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //blend GL_ONE_MINUS_SRC_ALPHA ：(1,1,1,1)-(sa , sa , sa , sa)
	//glDepthMask(GL_FALSE);
	//glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[1]);
	//CSCube_glass->draw();
	//glDisable(GL_BLEND);	// 關閉 Blending
	//glDepthMask(GL_TRUE);	// 開啟對 Z-Buffer 的寫入操作

	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //blend GL_ONE_MINUS_SRC_ALPHA ：(1,1,1,1)-(sa , sa , sa , sa)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID_Room1[door_txCount]);
	//CQDoor1_Room1->draw();
	//CQDoor2_Room1->draw();
	CQDoor1_Room6->draw();
	CQDoor2_Room6->draw();
	glActiveTexture(GL_TEXTURE0);
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, 0);
	CQRightWall_Room6->draw();
	glDisable(GL_BLEND);	// 關閉 Blending
	glDepthMask(GL_TRUE);	// 開啟對 Z-Buffer 的寫入操作


	
	
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

float door_delta; //計算幾秒換一次門
void onFrameMove(float delta)
{
	


	//計算0.2秒換一次門的貼圖
	door_delta += delta;
	if (door_delta>0.03f) //0.2秒換圖
	{
		door_txCount++;
		if (door_txCount == 28)
			door_txCount = 7;
			door_delta = 0.0f;
	}


	if ((Light_resulte_Room1[1].position.x < Light_resulte_Room1[2].position.x + 0.25f && Light_resulte_Room1[1].position.x > Light_resulte_Room1[2].position.x - 0.25f )&& (Light_resulte_Room1[1].position.z < Light_resulte_Room1[2].position.z + 0.25f && Light_resulte_Room1[1].position.z > Light_resulte_Room1[2].position.z - 0.25f)) { //燈光於相同範圍則結束遊戲

		bfinish = true;

	}



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

		CSSphere->SetViewPosition(camera->getViewPosition());
		CSSphere->setViewMatrix(mvx);
		CSCube_glass->setViewMatrix(mvx);
		

		float fAngle; //旋轉角度
		vec4 cameraPosition = camera->getViewPosition();
		/*g_tank->setViewMatrix(mvx);
		fAngle = g_tank->billboardCylindricalBegin(g_tank->_vT.x, g_tank->_vT.y, g_tank->_vT.z, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		g_tank->setTRSMatrix(Translate(g_tank->_vT) * RotateY(fAngle) * Scale(g_tank->_vS));*/
		
		//g_Target->setViewMatrix(mvx);
		/*g_Sun->setViewMatrix(mvx);
		g_Earth->setViewMatrix(mvx);
		g_Moon->setViewMatrix(mvx);
		g_gun->setViewMatrix(mvx);
		g_cat->setViewMatrix(mvx);*/
		g_balcony->setViewMatrix(mvx);
		g_track->setViewMatrix(mvx);
		CQRightWall_skybox->setViewMatrix(mvx);
		CQLeftWall_skybox->setViewMatrix(mvx);
		CQFrontWall_skybox->setViewMatrix(mvx);
		CQBackWall_skybox->setViewMatrix(mvx);
		CSFloor_skybox->setViewMatrix(mvx);
		CSCeiling_skybox->setViewMatrix(mvx);
		

		CQRightWall_Room1->setViewMatrix(mvx);
		CQLeftWall_Room1->setViewMatrix(mvx);
		CQFrontWall_Room1->setViewMatrix(mvx);
		CQBackWall_Room1->setViewMatrix(mvx);
		CSCeiling_Room1->setViewMatrix(mvx);
		CSFloor_Room1->setViewMatrix(mvx);
		CQDoor1_Room1->setViewMatrix(mvx);
		CQDoor2_Room1->setViewMatrix(mvx);
		

		CQRightWall_Room2->setViewMatrix(mvx);
		CQLeftWall_Room2->setViewMatrix(mvx);
		CQFrontWall_Room2->setViewMatrix(mvx);
		CQBackWall_Room2->setViewMatrix(mvx);
		CSFloor_Room2->setViewMatrix(mvx);
		CSCeiling_Room2->setViewMatrix(mvx);
		CSDoor2->setViewMatrix(mvx);

		CQRightWall_Room3->setViewMatrix(mvx);
		CQLeftWall_Room3->setViewMatrix(mvx);
		CQFrontWall_Room3->setViewMatrix(mvx);
		CQBackWall_Room3->setViewMatrix(mvx);
		CSFloor_Room3->setViewMatrix(mvx);
		CSCeiling_Room3->setViewMatrix(mvx);
		CSDoor3->setViewMatrix(mvx);

		CQRightWall_Room4->setViewMatrix(mvx);
		CQLeftWall_Room4->setViewMatrix(mvx);
		CQFrontWall_Room4->setViewMatrix(mvx);
		CQBackWall_Room4->setViewMatrix(mvx);
		CSFloor_Room4->setViewMatrix(mvx);
		CSCeiling_Room4->setViewMatrix(mvx);
		CSDoor4->setViewMatrix(mvx);

		CQRightWall_Room5->setViewMatrix(mvx);
		CQLeftWall_Room5->setViewMatrix(mvx);
		CQFrontWall_Room5->setViewMatrix(mvx);
		CQBackWall_Room5->setViewMatrix(mvx);
		CSFloor_Room5->setViewMatrix(mvx);
		CSCeiling_Room5->setViewMatrix(mvx);
		CSDoor5->setViewMatrix(mvx);


		CQRightWall_Room6->setViewMatrix(mvx);
		CQLeftWall_Room6->setViewMatrix(mvx);
		CQFrontWall_Room6->setViewMatrix(mvx);
		CQBackWall_Room6->setViewMatrix(mvx);
		CSFloor_Room6->setViewMatrix(mvx);
		CSCeiling_Room6->setViewMatrix(mvx);
		CQDoor1_Room6->setViewMatrix(mvx);
		CQDoor2_Room6->setViewMatrix(mvx);

	}

	
	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}
	// 如果需要重新計算時，在這邊計算每一個物件的顏色

	

	/*g_tank->update(delta, Light_resulte_Room4);
	CSCube_glass->update(delta, Light_resulte_Room1);
	g_gun->update(delta, Light_resulte_Room1);
	g_cat->update(delta, Light_resulte_Room5);*/
	g_balcony->update(delta, Light_resulte_Room1);
	g_track->update(delta, Light_resulte_Room1);

	CQRightWall_skybox->update(delta, Light_resulte_Room6);
	CQLeftWall_skybox->update(delta, Light_resulte_Room6);
	CQFrontWall_skybox->update(delta, Light_resulte_Room6);
	CQBackWall_skybox->update(delta, Light_resulte_Room6);
	CSFloor_skybox->update(delta, Light_resulte_Room6);
	CSCeiling_skybox->update(delta, Light_resulte_Room6);


	CSFloor_Room1->update(delta, Light_resulte_Room1);
	CSCeiling_Room1->update(delta, Light_resulte_Room1);
	CQRightWall_Room1->update(delta, Light_resulte_Room1);
	CQLeftWall_Room1->update(delta, Light_resulte_Room1);
	CQFrontWall_Room1->update(delta, Light_resulte_Room1);
	CQBackWall_Room1->update(delta, Light_resulte_Room1);
	CQDoor1_Room1->update(delta, Light_resulte_Room1);
	CQDoor2_Room1->update(delta, Light_resulte_Room1);


	g_Sun->update(delta, Light_resulte_Room2);
	g_Earth->update(delta, Light_resulte_Room2);
	g_Moon->update(delta, Light_resulte_Room2);
	CSSphere->update(delta, Light_resulte_enviroment);
	CQRightWall_Room2->update(delta, Light_resulte_Room2);
	CQLeftWall_Room2->update(delta, Light_resulte_Room2);
	CQFrontWall_Room2->update(delta, Light_resulte_Room2);
	CQBackWall_Room2->update(delta, Light_resulte_Room2);
	CSFloor_Room2->update(delta, Light_resulte_Room2);
	CSCeiling_Room2->update(delta, Light_resulte_Room2);
	CSDoor2->update(delta, Light_resulte_Room2);


	//g_Target->update(delta, Light_resulte_Room3);
	CQRightWall_Room3->update(delta, Light_resulte_Room3);
	CQLeftWall_Room3->update(delta, Light_resulte_Room3);
	CQFrontWall_Room3->update(delta, Light_resulte_Room3);
	CQBackWall_Room3->update(delta, Light_resulte_Room3);
	CSFloor_Room3->update(delta, Light_resulte_Room3);
	CSCeiling_Room3->update(delta, Light_resulte_Room3);
	CSDoor3->update(delta, Light_resulte_Room3);

	CQRightWall_Room4->update(delta, Light_resulte_Room4);
	CQLeftWall_Room4->update(delta, Light_resulte_Room4);
	CQFrontWall_Room4->update(delta, Light_resulte_Room4);
	CQBackWall_Room4->update(delta, Light_resulte_Room4);
	CSFloor_Room4->update(delta, Light_resulte_Room4);
	CSCeiling_Room4->update(delta, Light_resulte_Room4);
	CSDoor4->update(delta, Light_resulte_Room4);

	CQRightWall_Room5->update(delta, Light_resulte_Room5);
	CQLeftWall_Room5->update(delta, Light_resulte_Room5);
	CQFrontWall_Room5->update(delta, Light_resulte_Room5);
	CQBackWall_Room5->update(delta, Light_resulte_Room5);
	CSFloor_Room5->update(delta, Light_resulte_Room5);
	CSCeiling_Room5->update(delta, Light_resulte_Room5);
	CSDoor5->update(delta, Light_resulte_Room5);


	CQRightWall_Room6->update(delta, Light_resulte_Room6);
	CQLeftWall_Room6->update(delta, Light_resulte_Room6);
	CQFrontWall_Room6->update(delta, Light_resulte_Room6);
	CQBackWall_Room6->update(delta, Light_resulte_Room6);
	CSFloor_Room6->update(delta, Light_resulte_Room6);
	CSCeiling_Room6->update(delta, Light_resulte_Room6);
	CQDoor1_Room6->update(delta, Light_resulte_Room6);
	CQDoor2_Room6->update(delta, Light_resulte_Room6);

	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {

	case'F':
	case'f':
		bGoFree = !bGoFree;
		break;
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

    //遙控器車
	
		case '8': //上
			if (!bfinish) {
				Light_resulte_Room1[1].position.z += 1.0f;
				g_track->_vT.z += 1.0f;
				g_track->setTRSMatrix(Translate(g_track->_vT));
			}
			break;
		case '5': //下
			if (!bfinish) {
				Light_resulte_Room1[1].position.z -= 1.0f;
				g_track->_vT.z -= 1.0f;
				g_track->setTRSMatrix(Translate(g_track->_vT));
			}
			break;
		case '4'://左
			if (!bfinish) {
				Light_resulte_Room1[1].position.x += 1.0f;
				g_track->_vT.x += 1.0f;
				g_track->setTRSMatrix(Translate(g_track->_vT));
			}
			break;
		case '6'://右
			if (!bfinish) {
				Light_resulte_Room1[1].position.x -= 1.0f;
				g_track->_vT.x -= 1.0f;
				g_track->setTRSMatrix(Translate(g_track->_vT));
			}
			break;
	
    case 033:
		glutIdleFunc(NULL);

		delete CSSphere;
		/*delete g_tank;
		delete CSCube_glass;
		delete g_Target;
		delete g_Sun;
		delete g_Earth;
		delete g_Moon;
		delete g_gun;
		delete g_cat;*/
		delete g_balcony;
		delete g_track;
		delete CSCeiling_skybox;
		delete CSFloor_skybox;
		delete CQRightWall_skybox;
		delete CQLeftWall_skybox;
		delete CQFrontWall_skybox;
		delete CQBackWall_skybox;
		for (int i = 0; i < 4; i++)
			delete CQ_uiBtn[i];


		delete CSFloor_Room1;
		delete CSCeiling_Room1;
		delete CQRightWall_Room1;
		delete CQLeftWall_Room1;
		delete CQFrontWall_Room1;
		delete CQBackWall_Room1;
		delete CQDoor1_Room1;
		delete CQDoor2_Room1;

		
		delete CSCeiling_Room2;
		delete CSFloor_Room2;
		delete CQRightWall_Room2;
		delete CQLeftWall_Room2;
		delete CQFrontWall_Room2;
		delete CQBackWall_Room2;
		delete CSDoor2;

		delete CSCeiling_Room3;
		delete CSFloor_Room3;
		delete CQRightWall_Room3;
		delete CQLeftWall_Room3;
		delete CQFrontWall_Room3;
		delete CQBackWall_Room3;
		delete CSDoor3;

		delete CSCeiling_Room4;
		delete CSFloor_Room4;
		delete CQRightWall_Room4;
		delete CQLeftWall_Room4;
		delete CQFrontWall_Room4;
		delete CQBackWall_Room4;
		delete CSDoor4;

		delete CSCeiling_Room5;
		delete CSFloor_Room5;
		delete CQRightWall_Room5;
		delete CQLeftWall_Room5;
		delete CQFrontWall_Room5;
		delete CQBackWall_Room5;
		delete CSDoor5;

		delete CSCeiling_Room6;
		delete CSFloor_Room6;
		delete CQRightWall_Room6;
		delete CQLeftWall_Room6;
		delete CQFrontWall_Room6;
		delete CQBackWall_Room6;
		delete CQDoor1_Room6;
		delete CQDoor2_Room6;
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


bool bInRoom[13] = { false };  //哪間房間
float Room1_x_big = 18.5f;
float Room1_x_small = -18.5;
float Room1_y_big = 18.5f;
float Room1_y_small = -18.5f;


float Room2_x_big = 18.5f;
float Room2_x_small = -18.5f;
float Room2_y_big = 59.5f;
float Room2_y_small = 22.5f;


float Room3_x_big = 18.5f;
float Room3_x_small = -18.5f;
float Room3_y_big = 100.5f;
float Room3_y_small = 63.5f;



float Room4_x_big = 59.5f;
float Room4_x_small = 22.5f;
float Room4_y_big = 100.5f;
float Room4_y_small = 63.5f;

float Room5_x_big = 59.5f;
float Room5_x_small = 22.5f;
float Room5_y_big = 59.5f;
float Room5_y_small = 22.5f;


float Room6_x_big = 59.5f;
float Room6_x_small = 22.5;
float Room6_y_big = 18.5f;
float Room6_y_small = -18.5f;


float c1_x_big = 3.0f;
float c1_x_small = -5.0f;
float c1_y_big =23.0f;
float c1_y_small = 12.0f;

float c2_x_big = 3.0f;
float c2_x_small = -5.0f;
float c2_y_big = 64.0f;
float c2_y_small = 53.0f;

float c3_x_big = 25.0f;
float c3_x_small = 15.0f;
float c3_y_big = 89.0f;
float c3_y_small = 76.5f;

float c4_x_big = 44.0f;
float c4_x_small = 36.0f;
float c4_y_big = 64.0f;
float c4_y_small = 53.0f;


float c5_x_big = 44.0f;
float c5_x_small = 36.0f;
float c5_y_big = 23.0f;
float c5_y_small = 12.0f;

float c6_x_big = 25.0f;
float c6_x_small = 15.0f;
float c6_y_big = 12.0f;
float c6_y_small = -5.0f;

float c7_x_big = 59.0f;
float c7_x_small = 49.0f;
float c7_y_big = -15.0f;
float c7_y_small = -24.5f;




void reset_bInRoom() { //將bInRoom歸零以便重新判斷
	for (int i = 0; i < 13; i++)
	{
		bInRoom[i] = false;
	}
 }

void detectWhichPart() { //判斷目前位在哪個房間
	

	if ((g_fCameraMoveX <= c1_x_big && g_fCameraMoveX >= c1_x_small && g_fCameraMoveZ <= c1_y_big && g_fCameraMoveZ >= c1_y_small)) { //c1
		bInRoom[1] = true;
		printf("im in c1\n");
	}

	else if ((g_fCameraMoveX <= c2_x_big && g_fCameraMoveX >= c2_x_small && g_fCameraMoveZ <= c2_y_big && g_fCameraMoveZ >= c2_y_small)) { //c2
		bInRoom[3] = true;
		printf("im in c2\n");
	}

	else if ((g_fCameraMoveX <= c3_x_big && g_fCameraMoveX >= c3_x_small && g_fCameraMoveZ <= c3_y_big && g_fCameraMoveZ >= c3_y_small)) { //c3
		bInRoom[5] = true;
		printf("im in c3\n");
	}

	else if ((g_fCameraMoveX <= c4_x_big && g_fCameraMoveX >= c4_x_small && g_fCameraMoveZ <= c4_y_big && g_fCameraMoveZ >= c4_y_small)) { //c4
		bInRoom[7] = true;
		printf("im in c4\n");
	}


	else if ((g_fCameraMoveX <= c5_x_big && g_fCameraMoveX >= c5_x_small && g_fCameraMoveZ <= c5_y_big && g_fCameraMoveZ >= c5_y_small)) { //c5
		bInRoom[9] = true;
		printf("im in c5\n");
	}

	else if ((g_fCameraMoveX <= c6_x_big && g_fCameraMoveX >= c6_x_small && g_fCameraMoveZ <= c6_y_big && g_fCameraMoveZ >= c6_y_small)) { //c6
		bInRoom[11] = true;
		printf("im in c6\n");
	}


	else if ((g_fCameraMoveX <= c7_x_big && g_fCameraMoveX >= c7_x_small && g_fCameraMoveZ <= c7_y_big && g_fCameraMoveZ >= c7_y_small)) { //c7
		bInRoom[12] = true;
		printf("im in c7\n");
	}


	else if (g_fCameraMoveX <= Room1_x_big && g_fCameraMoveX >= Room1_x_small && g_fCameraMoveZ <= Room1_y_big && g_fCameraMoveZ >= Room1_y_small) { //room1
		bInRoom[0] = true;
		printf("im in room1\n");
	}

	else if (g_fCameraMoveX <= Room2_x_big && g_fCameraMoveX >= Room2_x_small && g_fCameraMoveZ <= Room2_y_big && g_fCameraMoveZ >= Room2_y_small) { //Room2
		bInRoom[2] = true;
		printf("im in Room2\n");
	}

	else if (g_fCameraMoveX <= Room3_x_big && g_fCameraMoveX >= Room3_x_small && g_fCameraMoveZ <= Room3_y_big && g_fCameraMoveZ >= Room3_y_small) { //Room3
		bInRoom[4] = true;
		printf("im in Room3\n");
	}

	else if (g_fCameraMoveX <= Room4_x_big && g_fCameraMoveX >= Room4_x_small && g_fCameraMoveZ <= Room4_y_big && g_fCameraMoveZ >= Room4_y_small) { //Room4
		bInRoom[6] = true;
		printf("im in Room4\n");
	}


	else if (g_fCameraMoveX <= Room5_x_big && g_fCameraMoveX >= Room5_x_small && g_fCameraMoveZ <= Room5_y_big && g_fCameraMoveZ >= Room5_y_small) { //Room5
		bInRoom[8] = true;
		printf("im in Room5\n");
	}


	else if (g_fCameraMoveX <= Room6_x_big && g_fCameraMoveX >= Room6_x_small && g_fCameraMoveZ <= Room6_y_big && g_fCameraMoveZ >= Room6_y_small) { //Room6
		bInRoom[10] = true;
		printf("im in Room6\n");
	}

}


void Win_SpecialKeyboard(int key, int x, int y) {

	front = normalize(at-eye);
	right = normalize(cross(front, g_vUp));
	
	reset_bInRoom(); //reset boolean
	detectWhichPart(); // detect in which part



	if (bInRoom[0]) { //若位於room1 時的範圍
		switch (key)
		{
		case GLUT_KEY_UP:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room1_x_big && g_fCameraMoveX >= Room1_x_small)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room1_y_big && g_fCameraMoveZ >= Room1_y_small)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room1_x_big) g_fCameraMoveX = Room1_x_big;
			else if (g_fCameraMoveX < Room1_x_small) g_fCameraMoveX = Room1_x_small;
			if (g_fCameraMoveZ >= Room1_y_big) g_fCameraMoveZ = Room1_y_big;
			else if (g_fCameraMoveZ <= Room1_y_small) g_fCameraMoveZ = Room1_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
			break;
		case GLUT_KEY_DOWN:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room1_x_big && g_fCameraMoveX >= Room1_x_small)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room1_y_big && g_fCameraMoveZ >= Room1_y_small)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room1_x_big) g_fCameraMoveX = Room1_x_big;
			else if (g_fCameraMoveX < Room1_x_small) g_fCameraMoveX = Room1_x_small;
			if (g_fCameraMoveZ >= Room1_y_big) g_fCameraMoveZ = Room1_y_big;
			else if (g_fCameraMoveZ <= Room1_y_small) g_fCameraMoveZ = Room1_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_LEFT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room1_x_big && g_fCameraMoveX >= Room1_x_small)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room1_y_big && g_fCameraMoveZ >= Room1_y_small)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room1_x_big) g_fCameraMoveX = Room1_x_big;
			else if (g_fCameraMoveX < Room1_x_small) g_fCameraMoveX = Room1_x_small;
			if (g_fCameraMoveZ >= Room1_y_big) g_fCameraMoveZ = Room1_y_big;
			else if (g_fCameraMoveZ <= Room1_y_small) g_fCameraMoveZ = Room1_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_RIGHT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room1_x_big && g_fCameraMoveX >= Room1_x_small)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room1_y_big && g_fCameraMoveZ >= Room1_y_small)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room1_x_big) g_fCameraMoveX = Room1_x_big;
			else if (g_fCameraMoveX < Room1_x_small) g_fCameraMoveX = Room1_x_small;
			if (g_fCameraMoveZ >= Room1_y_big) g_fCameraMoveZ = Room1_y_big;
			else if (g_fCameraMoveZ <= Room1_y_small) g_fCameraMoveZ = Room1_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
			break;



		default:
			break;
		}
	
	}

	else if (bInRoom[2]) { //若位於room2 時的範圍
		switch (key)
		{
		case GLUT_KEY_UP:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room2_x_big && g_fCameraMoveX >= Room2_x_small)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room2_y_big && g_fCameraMoveZ >= Room2_y_small)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room2_x_big) g_fCameraMoveX = Room2_x_big;
			else if (g_fCameraMoveX < Room2_x_small) g_fCameraMoveX = Room2_x_small;
			if (g_fCameraMoveZ >= Room2_y_big) g_fCameraMoveZ = Room2_y_big;
			else if (g_fCameraMoveZ <= Room2_y_small) g_fCameraMoveZ = Room2_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
			break;
		case GLUT_KEY_DOWN:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room2_x_big && g_fCameraMoveX >= Room2_x_small)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room2_y_big && g_fCameraMoveZ >= Room2_y_small)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room2_x_big) g_fCameraMoveX = Room2_x_big;
			else if (g_fCameraMoveX < Room2_x_small) g_fCameraMoveX = Room2_x_small;
			if (g_fCameraMoveZ >= Room2_y_big) g_fCameraMoveZ = Room2_y_big;
			else if (g_fCameraMoveZ <= Room2_y_small) g_fCameraMoveZ = Room2_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_LEFT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room2_x_big && g_fCameraMoveX >= Room2_x_small)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room2_y_big && g_fCameraMoveZ >= Room2_y_small)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room2_x_big) g_fCameraMoveX = Room2_x_big;
			else if (g_fCameraMoveX < Room2_x_small) g_fCameraMoveX = Room2_x_small;
			if (g_fCameraMoveZ >= Room2_y_big) g_fCameraMoveZ = Room2_y_big;
			else if (g_fCameraMoveZ <= Room2_y_small) g_fCameraMoveZ = Room2_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_RIGHT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= Room2_x_big && g_fCameraMoveX >= Room2_x_small)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			if (g_fCameraMoveZ <= Room2_y_big && g_fCameraMoveZ >= Room2_y_small)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

			//避免穿牆
			if (g_fCameraMoveX >= Room2_x_big) g_fCameraMoveX = Room2_x_big;
			else if (g_fCameraMoveX < Room2_x_small) g_fCameraMoveX = Room2_x_small;
			if (g_fCameraMoveZ >= Room2_y_big) g_fCameraMoveZ = Room2_y_big;
			else if (g_fCameraMoveZ <= Room2_y_small) g_fCameraMoveZ = Room2_y_small;
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
			break;



		default:
			break;
		}

	}


	else if (bInRoom[4]) { //若位於Room3 時的範圍
	switch (key)
	{
	case GLUT_KEY_UP:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room3_x_big && g_fCameraMoveX >= Room3_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room3_y_big && g_fCameraMoveZ >= Room3_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room3_x_big) g_fCameraMoveX = Room3_x_big;
		else if (g_fCameraMoveX < Room3_x_small) g_fCameraMoveX = Room3_x_small;
		if (g_fCameraMoveZ >= Room3_y_big) g_fCameraMoveZ = Room3_y_big;
		else if (g_fCameraMoveZ <= Room3_y_small) g_fCameraMoveZ = Room3_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;
	case GLUT_KEY_DOWN:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room3_x_big && g_fCameraMoveX >= Room3_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room3_y_big && g_fCameraMoveZ >= Room3_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room3_x_big) g_fCameraMoveX = Room3_x_big;
		else if (g_fCameraMoveX < Room3_x_small) g_fCameraMoveX = Room3_x_small;
		if (g_fCameraMoveZ >= Room3_y_big) g_fCameraMoveZ = Room3_y_big;
		else if (g_fCameraMoveZ <= Room3_y_small) g_fCameraMoveZ = Room3_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room3_x_big && g_fCameraMoveX >= Room3_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room3_y_big && g_fCameraMoveZ >= Room3_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room3_x_big) g_fCameraMoveX = Room3_x_big;
		else if (g_fCameraMoveX < Room3_x_small) g_fCameraMoveX = Room3_x_small;
		if (g_fCameraMoveZ >= Room3_y_big) g_fCameraMoveZ = Room3_y_big;
		else if (g_fCameraMoveZ <= Room3_y_small) g_fCameraMoveZ = Room3_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room3_x_big && g_fCameraMoveX >= Room3_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room3_y_big && g_fCameraMoveZ >= Room3_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room3_x_big) g_fCameraMoveX = Room3_x_big;
		else if (g_fCameraMoveX < Room3_x_small) g_fCameraMoveX = Room3_x_small;
		if (g_fCameraMoveZ >= Room3_y_big) g_fCameraMoveZ = Room3_y_big;
		else if (g_fCameraMoveZ <= Room3_y_small) g_fCameraMoveZ = Room3_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}


	else if (bInRoom[6]) { //若位於Room4 時的範圍
	switch (key)
	{
	case GLUT_KEY_UP:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room4_x_big && g_fCameraMoveX >= Room4_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room4_y_big && g_fCameraMoveZ >= Room4_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room4_x_big) g_fCameraMoveX = Room4_x_big;
		else if (g_fCameraMoveX < Room4_x_small) g_fCameraMoveX = Room4_x_small;
		if (g_fCameraMoveZ >= Room4_y_big) g_fCameraMoveZ = Room4_y_big;
		else if (g_fCameraMoveZ <= Room4_y_small) g_fCameraMoveZ = Room4_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;
	case GLUT_KEY_DOWN:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room4_x_big && g_fCameraMoveX >= Room4_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room4_y_big && g_fCameraMoveZ >= Room4_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room4_x_big) g_fCameraMoveX = Room4_x_big;
		else if (g_fCameraMoveX < Room4_x_small) g_fCameraMoveX = Room4_x_small;
		if (g_fCameraMoveZ >= Room4_y_big) g_fCameraMoveZ = Room4_y_big;
		else if (g_fCameraMoveZ <= Room4_y_small) g_fCameraMoveZ = Room4_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room4_x_big && g_fCameraMoveX >= Room4_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room4_y_big && g_fCameraMoveZ >= Room4_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room4_x_big) g_fCameraMoveX = Room4_x_big;
		else if (g_fCameraMoveX < Room4_x_small) g_fCameraMoveX = Room4_x_small;
		if (g_fCameraMoveZ >= Room4_y_big) g_fCameraMoveZ = Room4_y_big;
		else if (g_fCameraMoveZ <= Room4_y_small) g_fCameraMoveZ = Room4_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room4_x_big && g_fCameraMoveX >= Room4_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room4_y_big && g_fCameraMoveZ >= Room4_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room4_x_big) g_fCameraMoveX = Room4_x_big;
		else if (g_fCameraMoveX < Room4_x_small) g_fCameraMoveX = Room4_x_small;
		if (g_fCameraMoveZ >= Room4_y_big) g_fCameraMoveZ = Room4_y_big;
		else if (g_fCameraMoveZ <= Room4_y_small) g_fCameraMoveZ = Room4_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}


	else if (bInRoom[8]) { //若位於Room5 時的範圍
	switch (key)
	{
	case GLUT_KEY_UP:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room5_x_big && g_fCameraMoveX >= Room5_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room5_y_big && g_fCameraMoveZ >= Room5_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room5_x_big) g_fCameraMoveX = Room5_x_big;
		else if (g_fCameraMoveX < Room5_x_small) g_fCameraMoveX = Room5_x_small;
		if (g_fCameraMoveZ >= Room5_y_big) g_fCameraMoveZ = Room5_y_big;
		else if (g_fCameraMoveZ <= Room5_y_small) g_fCameraMoveZ = Room5_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;
	case GLUT_KEY_DOWN:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room5_x_big && g_fCameraMoveX >= Room5_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room5_y_big && g_fCameraMoveZ >= Room5_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room5_x_big) g_fCameraMoveX = Room5_x_big;
		else if (g_fCameraMoveX < Room5_x_small) g_fCameraMoveX = Room5_x_small;
		if (g_fCameraMoveZ >= Room5_y_big) g_fCameraMoveZ = Room5_y_big;
		else if (g_fCameraMoveZ <= Room5_y_small) g_fCameraMoveZ = Room5_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room5_x_big && g_fCameraMoveX >= Room5_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room5_y_big && g_fCameraMoveZ >= Room5_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room5_x_big) g_fCameraMoveX = Room5_x_big;
		else if (g_fCameraMoveX < Room5_x_small) g_fCameraMoveX = Room5_x_small;
		if (g_fCameraMoveZ >= Room5_y_big) g_fCameraMoveZ = Room5_y_big;
		else if (g_fCameraMoveZ <= Room5_y_small) g_fCameraMoveZ = Room5_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room5_x_big && g_fCameraMoveX >= Room5_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room5_y_big && g_fCameraMoveZ >= Room5_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room5_x_big) g_fCameraMoveX = Room5_x_big;
		else if (g_fCameraMoveX < Room5_x_small) g_fCameraMoveX = Room5_x_small;
		if (g_fCameraMoveZ >= Room5_y_big) g_fCameraMoveZ = Room5_y_big;
		else if (g_fCameraMoveZ <= Room5_y_small) g_fCameraMoveZ = Room5_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}



	else if (bInRoom[10]) { //若位於Room6 時的範圍
	switch (key)
	{
	case GLUT_KEY_UP:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room6_x_big && g_fCameraMoveX >= Room6_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room6_y_big && g_fCameraMoveZ >= Room6_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room6_x_big) g_fCameraMoveX = Room6_x_big;
		else if (g_fCameraMoveX < Room6_x_small) g_fCameraMoveX = Room6_x_small;
		if (g_fCameraMoveZ >= Room6_y_big) g_fCameraMoveZ = Room6_y_big;
		else if (g_fCameraMoveZ <= Room6_y_small) g_fCameraMoveZ = Room6_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;
	case GLUT_KEY_DOWN:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room6_x_big && g_fCameraMoveX >= Room6_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room6_y_big && g_fCameraMoveZ >= Room6_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room6_x_big) g_fCameraMoveX = Room6_x_big;
		else if (g_fCameraMoveX < Room6_x_small) g_fCameraMoveX = Room6_x_small;
		if (g_fCameraMoveZ >= Room6_y_big) g_fCameraMoveZ = Room6_y_big;
		else if (g_fCameraMoveZ <= Room6_y_small) g_fCameraMoveZ = Room6_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room6_x_big && g_fCameraMoveX >= Room6_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room6_y_big && g_fCameraMoveZ >= Room6_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room6_x_big) g_fCameraMoveX = Room6_x_big;
		else if (g_fCameraMoveX < Room6_x_small) g_fCameraMoveX = Room6_x_small;
		if (g_fCameraMoveZ >= Room6_y_big) g_fCameraMoveZ = Room6_y_big;
		else if (g_fCameraMoveZ <= Room6_y_small) g_fCameraMoveZ = Room6_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= Room6_x_big && g_fCameraMoveX >= Room6_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= Room6_y_big && g_fCameraMoveZ >= Room6_y_small)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= Room6_x_big) g_fCameraMoveX = Room6_x_big;
		else if (g_fCameraMoveX < Room6_x_small) g_fCameraMoveX = Room6_x_small;
		if (g_fCameraMoveZ >= Room6_y_big) g_fCameraMoveZ = Room6_y_big;
		else if (g_fCameraMoveZ <= Room6_y_small) g_fCameraMoveZ = Room6_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}




	
	else if (bInRoom[1]) { //若位於c1 時的範圍
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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_LEFT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= c1_x_big && g_fCameraMoveX >= c1_x_small)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
			if (g_fCameraMoveX >= c1_x_big) g_fCameraMoveX = c1_x_big;
			else if (g_fCameraMoveX <= c1_x_small) g_fCameraMoveX = c1_x_small;
			/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_RIGHT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= c1_x_big && g_fCameraMoveX >= c1_x_small)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
			if (g_fCameraMoveX >= 3.0) g_fCameraMoveX = c1_x_big;
			else if (g_fCameraMoveX <= c1_x_small) g_fCameraMoveX = c1_x_small;
			/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
			break;



		default:
			break;
		}

	}



	else if (bInRoom[3]) { //若位於c2 時的範圍
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c2_x_big && g_fCameraMoveX >= c2_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= c2_x_big) g_fCameraMoveX = c2_x_big;
		else if (g_fCameraMoveX <= c2_x_small) g_fCameraMoveX = c2_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c2_x_big && g_fCameraMoveX >= c2_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= 3.0) g_fCameraMoveX = c2_x_big;
		else if (g_fCameraMoveX <= c2_x_small) g_fCameraMoveX = c2_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}

	else if (bInRoom[5]) { //若位於c3 時的範圍
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c3_x_big && g_fCameraMoveX >= c3_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= c3_x_big) g_fCameraMoveX = c3_x_big;
		else if (g_fCameraMoveX <= c3_x_small) g_fCameraMoveX = c3_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c3_x_big && g_fCameraMoveX >= c3_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= 3.0) g_fCameraMoveX = c3_x_big;
		else if (g_fCameraMoveX <= c3_x_small) g_fCameraMoveX = c3_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}
	
	else if (bInRoom[7]) { //若位於c4 時的範圍
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c4_x_big && g_fCameraMoveX >= c4_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= c4_x_big) g_fCameraMoveX = c4_x_big;
		else if (g_fCameraMoveX <= c4_x_small) g_fCameraMoveX = c4_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c4_x_big && g_fCameraMoveX >= c4_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= 3.0) g_fCameraMoveX = c4_x_big;
		else if (g_fCameraMoveX <= c4_x_small) g_fCameraMoveX = c4_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}

	else if (bInRoom[9]) { //若位於c5 時的範圍
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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_LEFT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= c5_x_big && g_fCameraMoveX >= c5_x_small)
				g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
			/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
			if (g_fCameraMoveX >= c5_x_big) g_fCameraMoveX = c5_x_big;
			else if (g_fCameraMoveX <= c5_x_small) g_fCameraMoveX = c5_x_small;
			/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

			break;
		case GLUT_KEY_RIGHT:
			g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
			g_MoveDir = normalize(g_MoveDir);
			g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

			if (g_fCameraMoveX <= c5_x_big && g_fCameraMoveX >= c5_x_small)
				g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
			/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
				g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
			if (g_fCameraMoveX >= 3.0) g_fCameraMoveX = c5_x_big;
			else if (g_fCameraMoveX <= c5_x_small) g_fCameraMoveX = c5_x_small;
			/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
			else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
			break;



		default:
			break;
		}

	}

	else if (bInRoom[11]) { //若位於c6 時的範圍
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c6_x_big && g_fCameraMoveX >= c6_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= c6_x_big) g_fCameraMoveX = c6_x_big;
		else if (g_fCameraMoveX <= c6_x_small) g_fCameraMoveX = c6_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c6_x_big && g_fCameraMoveX >= c6_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= 3.0) g_fCameraMoveX = c6_x_big;
		else if (g_fCameraMoveX <= c6_x_small) g_fCameraMoveX = c6_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}

	else if (bInRoom[12]) { //若位於c7 時的範圍
	switch (key)
	{
	case GLUT_KEY_UP:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c7_x_big && g_fCameraMoveX >= c7_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		if (g_fCameraMoveZ <= c7_y_big && g_fCameraMoveZ >= c7_y_small)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);

		//避免穿牆
		if (g_fCameraMoveX >= c7_x_big) g_fCameraMoveX = c7_x_big;
		else if (g_fCameraMoveX < c7_x_small) g_fCameraMoveX = c7_x_small;
		if (g_fCameraMoveZ >= c7_y_big) g_fCameraMoveZ = c7_y_big;
		else if (g_fCameraMoveZ <= c7_y_small) g_fCameraMoveZ = c7_y_small;
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_LEFT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c7_x_big && g_fCameraMoveX >= c7_x_small)
			g_fCameraMoveX += (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ += (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= c7_x_big) g_fCameraMoveX = c7_x_big;
		else if (g_fCameraMoveX <= c7_x_small) g_fCameraMoveX = c7_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

		break;
	case GLUT_KEY_RIGHT:
		g_MoveDir = vec4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), 0.f, g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.f);
		g_MoveDir = normalize(g_MoveDir);
		g_matMoveDir = RotateY(90.f) * Translate(g_MoveDir.x, 0.f, g_MoveDir.z);

		if (g_fCameraMoveX <= c7_x_big && g_fCameraMoveX >= c7_x_small)
			g_fCameraMoveX -= (g_matMoveDir._m[0][3] * 3.0f);
		/*if (g_fCameraMoveZ <= 18.5f && g_fCameraMoveZ >= -18.5f)
			g_fCameraMoveZ -= (g_matMoveDir._m[2][3] * 3.0f);*/

			//避免穿牆
		if (g_fCameraMoveX >= 3.0) g_fCameraMoveX = c7_x_big;
		else if (g_fCameraMoveX <= c7_x_small) g_fCameraMoveX = c7_x_small;
		/*if (g_fCameraMoveZ > 18.5) g_fCameraMoveZ = 18.5f;
		else if (g_fCameraMoveZ < -18.5) g_fCameraMoveZ = -18.5f;*/
		printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
		break;



	default:
		break;
	}

	}


	if(bGoFree) //無範圍限制
	{
		
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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);

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
			printf("%f , %f\n", g_fCameraMoveX, g_fCameraMoveZ);
			break;



		default:
			break;
		}



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