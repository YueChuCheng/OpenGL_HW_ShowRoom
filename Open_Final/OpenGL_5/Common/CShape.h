
#ifndef CSHAPE_H
#define CSHAPE_H
#define LIGHT_NUM 4 //�O�w�ƶq


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
	vec2* _pTex_normal;		// �s�W�ĤT�i�K�� for example 4
	vec3* _pTangent;	// �s�W tangent vector for each vertex
#endif // NORMALMAP

	
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
	
	GLuint _uiLightInView[LIGHT_NUM]; //�����bshader����m
	GLuint _uiAmbient[LIGHT_NUM]; //light's ambient �P object's ambient �P ka�����n
	GLuint _uiDiffuse[LIGHT_NUM];
	GLuint _uiSpecular[LIGHT_NUM];
	GLuint _uiLighting[LIGHT_NUM];
	float _uicutoff[LIGHT_NUM];
	GLuint _uiLightNUM; //���X���O
	GLuint  m_uiTexLayer;	// �K�Ϫ��h���A�w�]�N�O�@�h diffuse


	int    m_iTexLayer;		// �]�w�K�Ϫ��h���A0 ��ܨS���K��
	point4 _vLightInView[LIGHT_NUM]; //�����b�@�ɮy�Ъ���m
	color4 _ambientProduct[LIGHT_NUM];
	color4 _diffuseProduct[LIGHT_NUM];
	color4 _specularProduct[LIGHT_NUM];
	int _vLightNUM = 4; //���X���O

	
	float _cutoff[LIGHT_NUM];

	
	
	mat4 _mxView, _mxProjection, _mxTRS;
	mat4 _mxMVFinal;
	mat3 _mxMV3x3Final, _mxITMV; //�ϥΦb�p�� �������᪺�sNormal
	mat3 _mxITView;
	bool _projUpdated = false, _viewUpdated = false, _TRSUpdated=false;

	Material _material;

	int _iMode; //�W��Ҧ�,0: Flat shading, 1: Gouraud shading, 0 for default
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
	void SetTiling(float uTiling, float vTiling);  // �� U�b �P V�b �i����K���Y��



	void setShadingMode(int iMode) { _iMode = iMode; } //���ø�ϼҦ�
	vec4 phongReflectionModel(vec4 vPoint, vec3 vNormal, vec4 vLightPos, color4 vLight);
	vec4 phongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource &Lights);

	int _iLighting[LIGHT_NUM]; //�O�_�n���O

	void setLightingDisable(int i /*�s���ĴX�����O��*/) { 
		
			_iLighting[i] = 0;
		
	}


};

#endif

