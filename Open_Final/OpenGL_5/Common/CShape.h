
#ifndef CSHAPE_H
#define CSHAPE_H
#include "../Header/Angel.h"
#include "TypeDefine.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

#define FLAT_SHADING 0
#define GOURAUD_SHADING 1


#define LIGHTING_WITHGPU
#define PERVERTEX_uiLighting

class CShape
{
protected:
	vec4* _pPoints;
	vec3* _pNormals;
	vec4* _pColors;
	vec2* _pTex;
	int _iNumVtx;

	GLfloat _Color[4]; //object's color

	char* _pVXshader, * _pFShader;

	//for vao
	GLuint _vao;
	
	//for shader
	GLuint _uiModelView, _uiProjection, _uiColor;
	GLuint _vbo;
	GLuint _uiBuffer;

	point4 _vLightInView; //光源在世界座標的位置
	GLuint _uiLightInView; //光源在shader的位置
	GLuint _uiAmbient; //light's ambient 與 object's ambient 與 ka的乘積
	GLuint _uiDiffuse;
	GLuint _uiSpecular;
	GLuint _uiShininess;
	GLuint _uiLighting;

	LightSource _light;

	color4 _ambientProduct;
	color4 _diffuseProduct;
	color4 _specularProduct;
	int _iLighting; //是否要打燈
	
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
	CShape();
	virtual ~CShape();
	virtual void draw() = 0;
	virtual void drawW() = 0;
	virtual void update(float dt , point4 vLightPos , color4 vLight) = 0;
	virtual void update(float dt, const LightSource &lights) = 0;
	virtual void update(float dt) = 0;

	void setShaderName(const char vxShader[],const char fsShader[]);
	void setShader(GLuint shaderHandle = MAX_UNSIGNED_INT);
	void setColor(vec4 vColor);
	void setViewMatrix(mat4& mat);
	void setProjectionMatrix(mat4& mat);
	void setTRSMatrix(mat4& mat);

	void setMaterials(color4 ambient, color4 diffuse, color4 specular);
	void setKaKdKsShini(float ka, float kd, float ks, float shininess);//ka kd ks shininess

	void setShadingMode(int iMode) { _iMode = iMode; } //更改繪圖模式
	vec4 phongReflectionModel(vec4 vPoint, vec3 vNormal, vec4 vLightPos, color4 vLight);
	vec4 phongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource &Lights);

	void setLightingDisable() { _iLighting = 0; }


};

#endif

