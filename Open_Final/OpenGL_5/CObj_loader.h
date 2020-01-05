//#ifndef COBJ_LOADER_H
//#define COBJ_LOADER_H
//
//#include "Common/CShape.h"
//#include "Header/Angel.h"
//#include <vector>
//
//class CObj_loader : public CShape 
//{
//public:
//	CObj_loader(const char* path);
//	~CObj_loader();
//	bool LoadObjFromFileVertexAndNormal(const char* path, std::vector<float>* vertices, std::vector<float>* normals);
//	
//	void update(float dt, const LightSource* lights);
//	void update(float dt, vec4 vLightPos, color4 vLightI);
//	void update(float dt, const LightSource& Lights);
//	void update(float dt);
//
//	GLuint getShaderHandle() { return _vbo; }
//	void renderWithFlatShading(vec4 vLightPos, color4 vLight);//  vLightI: Light Intensity
//	void renderWithGouraudShading(vec4 vLightPos, color4 vLight);//  vLightI: Light Intensity
//	void renderWithFlatShading(const LightSource& Lights);
//	void renderWithGouraudShading(const LightSource& Lights);
//	void setVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor);
//	void setVtxColors(vec4 vFColor, vec4 vSColor);
//	void setNormal(vec3 vNormal); //更改法向量
//
//	void draw();
//	void drawW();
//
//	int point = 0; //有多少點
//
//private:
//
//};
//
//
//#endif



#ifndef OBJLOADER_H_
#define OBJLOADER_H_

#include <vector>
#include "header/Angel.h"

bool LoadObj(const char* path, std::vector<float>* vertices, std::vector<float>* normals);

#endif