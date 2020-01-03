#include "header/Angel.h"
#include "Common/TypeDefine.h"
#include "Common/CQuad.h"
#include "Common/CSmoothQuad.h"
#include "Common/CCamera.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 40 // must be an even number


CQuad* CQ_leftWall, * CQ_rightWall;
CQuad* CQ_frontWall, * CQ_backWall;


CSmoothQuad* CFloor;
CSmoothQuad* CQ_ceiling;

GLfloat g_fRadius = 40.0; //視覺範圍
GLfloat g_fTheta = 60.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;

//camera
point4  eye(0.0f, 5.0f, 0.0f, 1.0f);
point4  at(g_fRadius* sin(g_fTheta)* sin(g_fPhi), g_fRadius* cos(g_fTheta), g_fRadius* sin(g_fTheta)* cos(g_fPhi), 1.0f);
point4 g_vUp(0.0, 1.0, 0.0, 0.0);
point4 front;
point4 right;

// Part 2 : for single light source
bool g_bAutoRotating = false;
float _fLightDelta = 0; //燈光旋轉，經過時間
float _fLightRadius = 10;//燈光位置
float _fLightTheta = 0;//燈光旋轉角度

float g_fLightR = 0.85f;
float g_fLightG = 0.85f;
float g_fLightB = 0.85f;

//light
#define LIGHT_NUM 2
vec4 vlight_Center(0.0f, 10.0f, 0.0f, 1.0f);
vec4 vlight_Center_Color(g_fLightR, g_fLightG, g_fLightB, 1.0f);



LightSource Light_resulte[LIGHT_NUM] = { //最後燈光結果

	Light_center , _Light1
};


LightSource  Light_center = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius, _fLightRadius, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	0.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};



LightSource  _Light1 = {

	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, 0.5, 0.5, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(_fLightRadius, _fLightRadius, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};


//LightSource  _Light_end = {
//
//	color4(0, 0, 0, 1.0f), // ambient 
//	color4(0, 0, 0, 1.0f), // diffuse
//	color4(0, 0, 0, 1.0f), // specular
//	point4(_fLightRadius, _fLightRadius, 0.0f, 1.0f),   // position
//	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
//	vec3(0.0f, 0.0f, 0.0f),		  //spotTarget
//	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
//	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
//	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
//	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
//	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
//	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
//	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
//};


// 函式的原型宣告
extern void IdleProcess();

void init( void )
{
	mat4 mxT;
	vec4 vT, vColor;
	// 產生所需之 Model View 與 Projection Matrix

	
	
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);
	

	CFloor = new CSmoothQuad(GRID_SIZE);
	CFloor->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CFloor->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CFloor->setShadingMode(GOURAUD_SHADING);
	CFloor->setShader();
	
	vec3 test=vec3(0.0f, -1.0f, 0.0f);
	CQ_ceiling = new CSmoothQuad(GRID_SIZE,15.0f);
	CQ_ceiling->setNormal(test);
	CQ_ceiling->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_ceiling->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	CQ_ceiling->setShadingMode(GOURAUD_SHADING);
	CQ_ceiling->setShader();
	
	


	vT.x = -20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQ_leftWall = new CQuad;
	CQ_leftWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_leftWall->setShadingMode(GOURAUD_SHADING);
	CQ_leftWall->setShader();
	CQ_leftWall->setColor(vec4(0.6f));
	CQ_leftWall->setTRSMatrix(mxT * RotateZ(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_leftWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);


	vT.x = 20.0f; vT.y = 20.0f; vT.z = 0;
	mxT = Translate(vT);
	CQ_rightWall = new CQuad;
	CQ_rightWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_rightWall->setShadingMode(GOURAUD_SHADING);
	CQ_rightWall->setShader();
	CQ_rightWall->setColor(vec4(0.6f));
	CQ_rightWall->setTRSMatrix(mxT * RotateZ(90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_rightWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 20.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	CQ_frontWall = new CQuad;
	CQ_frontWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_frontWall->setShadingMode(GOURAUD_SHADING);
	CQ_frontWall->setShader();
	CQ_frontWall->setColor(vec4(0.6f));
	CQ_frontWall->setTRSMatrix(mxT * RotateX(-90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_frontWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 20.0f; vT.z = -20.0f;
	mxT = Translate(vT);
	CQ_backWall = new CQuad;
	CQ_backWall->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	CQ_backWall->setShadingMode(GOURAUD_SHADING);
	CQ_backWall->setShader();
	CQ_backWall->setColor(vec4(0.6f));
	CQ_backWall->setTRSMatrix(mxT * RotateX(90.0f) * Scale(40.0f, 1, 40.0f));
	CQ_backWall->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	


	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	
	CQ_leftWall->setProjectionMatrix(mpx);
	CQ_rightWall->setProjectionMatrix(mpx);
	CQ_frontWall->setProjectionMatrix(mpx);
	CQ_backWall->setProjectionMatrix(mpx);
	CFloor->setProjectionMatrix(mpx);
	CQ_ceiling->setProjectionMatrix(mpx);



}

//----------------------------------------------------------------------------
void GL_Display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	CFloor->draw();
	CQ_ceiling->draw();
	CQ_leftWall->draw();
	CQ_rightWall->draw();
	CQ_frontWall->draw();
	CQ_backWall->draw();

	
	glutSwapBuffers();	// 交換 Frame Buffer
}

//----------------------------------------------------------------------------
// Part 2 : for single light source
void UpdateLightPosition(float dt)
{
	mat4 mxT;
	_fLightDelta += dt;
	_fLightTheta = _fLightDelta * (float)M_PI_2 * 0.5; //每一秒轉180度
	if (_fLightTheta>=(float)M_PI*2.0f) {
		_fLightTheta -= (float)M_PI * 2.0f;
		_fLightDelta -= 8.0f;
	}
	Light_center.position.x = _fLightRadius * cosf(_fLightTheta);
	Light_center.position.z = _fLightRadius * sinf(_fLightTheta);
	
	
}
//----------------------------------------------------------------------------

void onFrameMove(float delta)
{

	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		CFloor->setViewMatrix(mvx);
		CQ_leftWall->setViewMatrix(mvx);
		CQ_rightWall->setViewMatrix(mvx);
		CQ_frontWall->setViewMatrix(mvx);
		CQ_backWall->setViewMatrix(mvx);
		CQ_ceiling->setViewMatrix(mvx);
	
	
	}

	
	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}
	// 如果需要重新計算時，在這邊計算每一個物件的顏色





	/*CFloor->update(delta, vlight_Center,vlight_Center_Color);
	CQ_ceiling->update(delta, vlight_Center, vlight_Center_Color);
	CQ_leftWall->update(delta, vlight_Center, vlight_Center_Color);
	CQ_rightWall->update(delta, vlight_Center, vlight_Center_Color);
	CQ_frontWall->update(delta, vlight_Center, vlight_Center_Color);
	CQ_backWall->update(delta, vlight_Center, vlight_Center_Color);*/
	
	CFloor->update(delta, Light_resulte);
	CQ_ceiling->update(delta, Light_resulte);
	CQ_leftWall->update(delta, Light_resulte);
	CQ_rightWall->update(delta, Light_resulte);
	CQ_frontWall->update(delta, Light_resulte);
	CQ_backWall->update(delta, Light_resulte);


	/*CFloor->update(delta, Light_center);
	CQ_ceiling->update(delta, Light_center);
	CQ_leftWall->update(delta, Light_center);
	CQ_rightWall->update(delta, Light_center);
	CQ_frontWall->update(delta, Light_center);
	CQ_backWall->update(delta, Light_center);*/
	


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

	//case 82: //R
	//	if (Light_center.diffuse.x < 1.0f)
	//		Light_center.diffuse.x += 0.05f;
	//	else
	//		Light_center.diffuse.x = 0.95f;
	//	break;
	//case 114: //r
	//	if (Light_center.diffuse.x > 0.0f)
	//		Light_center.diffuse.x -= 0.05f;
	//	else
	//		Light_center.diffuse.x = 0.01f;
	//	break;

	//case 71: //G
	//	if (Light_center.diffuse.y < 1.0f)
	//		Light_center.diffuse.y += 0.05f;
	//	else
	//		Light_center.diffuse.y = 0.95f;
	//	break;

	//case 103: //g
	//	if (Light_center.diffuse.y > 0.0f)
	//		Light_center.diffuse.y -= 0.05f;
	//	else
	//		Light_center.diffuse.y = 0.01;
	//	break;

	//case 66: //B
	//	if (Light_center.diffuse.z < 1.0f)
	//		Light_center.diffuse.z += 0.05f;
	//	else
	//		Light_center.diffuse.z = 0.95f;
	//	break;

	//case 98: //b
	//	if (Light_center.diffuse.z > 0.0f)
	//		Light_center.diffuse.z -= 0.05f;
	//	else
	//		Light_center.diffuse.z = 0.01;
	//	break;


	case 80://P
	case 112://p
		printf("( %f , %f , %f )\n", eye.x, eye.y, eye.z);
		break;

    case 033:
		delete CFloor;
		delete CQ_ceiling;
		delete CQ_leftWall;
		delete CQ_rightWall;
		delete CQ_frontWall;
		delete CQ_backWall;
		CCamera::getInstance()->destroyInstance();
        exit( EXIT_SUCCESS );
        break;
    }
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
		if (eye.x+ front.x<18.0f && eye.x + front.x > -18.0f)
		{
			eye += point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at += point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if ( eye.z + front.z < 18.0f && eye.z + front.z > -18.0f)
		{
			eye += point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
			at += point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
		}
		
		
		break;
	case GLUT_KEY_DOWN:
		if (eye.x - front.x<18.0f && eye.x - front.x > -18.0f)
		{
			eye -= point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at -= point4(front.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if (eye.z - front.z < 18.0f && eye.z - front.z > -18.0f)
		{
			eye -= point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
			at -= point4(0.0f, 0.0f, front.z, 0.0f) * 3.0f;
		}
		break;
	case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵
		
		if (eye.x - right.x<18.0f && eye.x - right.x > -18.0f)
		{
			eye -= point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at -= point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if (eye.z - right.z < 18.0f && eye.z - right.z > -18.0f)
		{
			eye -= point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
			at -= point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
		}
		
		break;
	case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵
		if (eye.x + right.x<18.0f && eye.x + right.x > -18.0f)
		{
			eye += point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
			at += point4(right.x, 0.0f, 0.0f, 0.0f) * 3.0f;
		}

		if (eye.z + right.z < 18.0f && eye.z + right.z > -18.0f)
		{
			eye += point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
			at += point4(0.0f, 0.0f, right.z, 0.0f) * 3.0f;
		}
		break;

	default:
		break;
	}

	if (eye.x > 18.0f)
		eye.x = 17.9;

	if (eye.x < -18.0f)
		eye.x = -17.9;

	if (eye.z > 18.0f)
		eye.z = 17.9;

	if (eye.z < -18.0f)
		eye.z = -17.9;

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