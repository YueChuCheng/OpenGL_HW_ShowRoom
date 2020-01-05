#ifndef MODELPOOL_H
#define MODELPOOL_H

#include "../header/Angel.h"
#include "CShape.h"

class ModelPool : public CShape
{
public:
	ModelPool(const char* path);
	~ModelPool() {};

	void update(float dt, const LightSource* Lights)
	{

		if (_viewUpdated || _TRSUpdated) {
			_mxMVFinal = _mxView * _mxTRS;
			_viewUpdated = _TRSUpdated = false;
		}
		/*vec3 vLightDir = _mxMVFinal * Lights[1].spotDirection;

		// 5. 計算照明的點是否落在 spotCutoff 之內
		GLfloat fLdotLDir = -(vLight.x * vLightDir.x + vLight.y * vLightDir.y + vLight.z * vLightDir.z);
		*/
		for (int i = 0; i < LIGHT_NUM; i++)
		{
			_vLightInView[i] = _mxView * Lights[i].position;		// 將 Light 轉換到鏡頭座標再傳入
			// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
			_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
			_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse;
			_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
			_cutoff[i] = Lights[i].spotCutoff;

		}


	}
	void update(float dt, vec4 vLightPos, color4 vLightI) {};
	void update(float dt, const LightSource& Lights) {};
	void update(float dt) {};

	GLuint getShaderHandle() { return _vbo; }
	void renderWithFlatShading(vec4 vLightPos, color4 vLight) {};//  vLightI: Light Intensity
	void renderWithGouraudShading(vec4 vLightPos, color4 vLight) {};//  vLightI: Light Intensity
	void renderWithFlatShading(const LightSource& Lights) {};
	void renderWithGouraudShading(const LightSource& Lights) {};

	int Num;

	void draw();
	void drawW();

protected:

};

#endif
