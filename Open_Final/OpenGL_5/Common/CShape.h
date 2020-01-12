
#ifndef CSHAPE_H
#define CSHAPE_H
#define LIGHT_NUM_MAX 4 //燈泡數量


#include "../Header/Angel.h"
#include "TypeDefine.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

#define FLAT_SHADING 0
#define GOURAUD_SHADING 1


#define LIGHTING_WITHGPU
#define PERVERTEX_uiLighting
#define LIGHTMAP
#define NORMALMAP
#define CUBIC_MAP //environment map



class CShape
{
protected:
	vec4* _pPoints;
	vec3* _pNormals;
	vec4* _pColors;
	vec2* _pTex;


#ifdef LIGHTMAP
	vec2* _pTex_light; //light map
#endif // LIGHTMAP

#ifdef NORMALMAP
	vec2* _pTex_normal;		// 新增第三張貼圖 for example 4
	vec3* _pTangent;	// 新增 tangent vector for each vertex
#endif // NORMALMAP


#ifdef CUBIC_MAP
	GLuint  _uiTRS;			// TRSMatrix 傳入 Pixel Shader 的位置
	GLuint  _uiViewPos;		// ViewPoint 傳入 Pixel Shader 的位置
	point4  _v4Eye;			// Camera 的位置

	GLuint  _uiCubeMap;     // pixel shader 中的 CubeMap Texture Name 的位置
	GLuint  _uiCubeMapTexName;  // 傳入 pixel shader 中的 Cube map 的貼圖編號， 
#endif // CUBIC_MAP

	
	int _iNumVtx;

	GLfloat _Color[4]; //object's color

	char* _pVXshader, * _pFShader;

	//for vao
	GLuint _vao;
	
	//for shader
	GLuint _uiModelView, _uiProjection, _uiColor;
	GLuint _vbo;
	GLuint _uiBuffer;
	
	GLuint _uiShininess;
	
	GLuint _uiLightInView[LIGHT_NUM_MAX]; //光源在shader的位置
	GLuint _uiAmbient[LIGHT_NUM_MAX]; //light's ambient 與 object's ambient 與 ka的乘積
	GLuint _uiDiffuse[LIGHT_NUM_MAX];
	GLuint _uiSpecular[LIGHT_NUM_MAX];
	GLuint _uiLighting[LIGHT_NUM_MAX];
	float _uicutoff[LIGHT_NUM_MAX];
	GLuint _uiLightNUM; //渲染幾盞燈
	GLuint  m_uiTexLayer;	// 貼圖的層次，預設就是一層 diffuse


	
	point4 _vLightInView[LIGHT_NUM_MAX]; //光源在世界座標的位置
	point4 _vSpotTarget[LIGHT_NUM_MAX]; //光源在世界座標的位置
	color4 _ambientProduct[LIGHT_NUM_MAX];
	color4 _diffuseProduct[LIGHT_NUM_MAX];
	color4 _specularProduct[LIGHT_NUM_MAX];
	float _cutoff[LIGHT_NUM_MAX];
	int _iLightNUM = 1; //渲染幾盞燈 預設至少一盞
	int    m_iTexLayer;		// 設定貼圖的層次，0 表示沒有貼圖

	

	mat4 _mxView, _mxProjection, _mxTRS;
	mat4 _mxMVFinal;
	mat3 _mxMV3x3Final, _mxITMV; //使用在計算 物體旋轉後的新Normal
	mat3 _mxITView;
	bool _projUpdated = false, _viewUpdated = false, _TRSUpdated=false;

	Material _material;

	int _iMode; //上色模式,0: Flat shading, 1: Gouraud shading, 0 for default
	void createBufferObject();
	void drawingSetShader();
	void drawingWithoutSetShader();



public:

	float _fBillboardingAngle; //Billboarding 旋轉角度
	vec3 _vS; //紀錄大小
	vec4 _vT; //紀錄位移量
	float _fRotateAngle; //r紀錄旋轉角度


	CShape();
	virtual ~CShape();
	virtual void draw() = 0;
	virtual void drawW() = 0;
	virtual void update(float dt , point4 vLightPos , color4 vLight) = 0;
	virtual void update(float dt, const LightSource &lights) = 0;
	virtual void update(float dt, const LightSource* lights) = 0;
	virtual void update(float dt) = 0;

	void setShaderName(const char vxShader[],const char fsShader[]);
	void setShader(GLuint shaderHandle = MAX_UNSIGNED_INT);
	void setColor(vec4 vColor);
	void setViewMatrix(mat4& mat);
	void setProjectionMatrix(mat4& mat);
	void setTRSMatrix(mat4& mat);

	void SetTextureLayer(int texlayer);
	void setMaterials(color4 ambient, color4 diffuse, color4 specular);
	void setKaKdKsShini(float ka, float kd, float ks, float shininess);//ka kd ks shininess
	void SetTiling(float uTiling, float vTiling);  // 對 U軸 與 V軸 進行拼貼的縮放
	void SetIlightNUM(int iNum); //設定須受幾盞燈渲染

	void setShadingMode(int iMode) { _iMode = iMode; } //更改繪圖模式
	vec4 phongReflectionModel(vec4 vPoint, vec3 vNormal, vec4 vLightPos, color4 vLight);
	vec4 phongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource &Lights);

	int _iLighting[LIGHT_NUM_MAX]; //是否要打燈

	void setLightingDisable(int i /*編號第幾號的燈光*/) { 
		
			_iLighting[i] = 0;
		
	}

	float billboardCylindricalBegin(float camX, float camY, float camZ,float objPosX, float objPosY, float objPosZ);



#ifdef CUBIC_MAP
	void SetCubeMapTexName(GLuint uiTexName) {
		_uiCubeMapTexName = uiTexName;
	}
	void SetViewPosition(point4 vEye) {
		_v4Eye = vEye;
	}
#endif

};

#endif

