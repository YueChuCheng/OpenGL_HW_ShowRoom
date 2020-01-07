#ifndef MODELPOOL_H
#define MODELPOOL_H

#include "../header/Angel.h"
#include "CShape.h"

class ModelPool : public CShape
{
public:
	ModelPool(const char* path);
	~ModelPool() {};

	void update(float dt, const LightSource* Lights);
	void update(float dt, vec4 vLightPos, color4 vLightI) {};
	void update(float dt, const LightSource& Lights) {};
	void update(float dt) {};

	GLuint getShaderHandle() { return _vbo; }
	void renderWithFlatShading(vec4 vLightPos, color4 vLight) {};//  vLightI: Light Intensity
	void renderWithGouraudShading(vec4 vLightPos, color4 vLight) {};//  vLightI: Light Intensity
	void renderWithFlatShading(const LightSource& Lights) {};
	void renderWithGouraudShading(const LightSource& Lights) {};

	void setLightNUM(int num); //設定被幾盞燈所影響

	void draw();
	void drawW();

protected:

};

#endif
