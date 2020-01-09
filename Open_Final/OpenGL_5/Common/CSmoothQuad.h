#ifndef CSmoothQuad_H
#define CSmoothQuad_H
#include"TypeDefine.h"
#include"CQuad.h"

class CSmoothQuad
{
public:
	CSmoothQuad(int iSize = 6, float fYPos = 0.0f);
	~CSmoothQuad();
	void setShader();
	void setProjectionMatrix(mat4& mat);
	void setViewMatrix(mat4& mat);
	void setTRSMatrix(mat4 &mat);
	void setShadingMode(int iMode);//�]�w�C�@�檺��V�覡
	void update(float dt, point4 vLightPos, color4 vLightI);
	void update(float dt, const LightSource& Lights);
	void update(float dt, const LightSource* Lights);
	void draw();
	void setNormal(vec3 vNormal);//�]�w�k�V�q


	// For setting materials 
	void setMaterials(color4 ambient, color4 diffuse, color4 specular);
	void setKaKdKsShini(float ka, float kd, float ks, float shininess); // ka kd ks shininess

	
private:

	CQuad* _pSquares;
	int    _iGridSize;//���h�֮�
	float* _pfSquaresT; //�����C�@�檺��m
	GLuint _shaderHandle;
	float  _fYPos; //����

};
#endif
