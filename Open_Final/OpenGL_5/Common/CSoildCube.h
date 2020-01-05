#ifndef CSOILDCUBE_H
#define CSOILDCUBE_H

#include"../Header/Angel.h"
#include "CShape.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

#define POINT_CUBE_NUM 36

class CSolidCube : public CShape
{
public:
	CSolidCube();
	~CSolidCube();

	void update(float dt);
	void update(float dt, const LightSource& Lights);
	void update(float dt, const LightSource* Lights);
	void update(float dt, point4 vLightPos, color4 vLightI);

	void RenderWithFlatShading(point4 vLightPos, color4 vLightI) {};
	void RenderWithGouraudShading(point4 vLightPos, color4 vLightI) {};
	void RenderWithFlatShading(const LightSource& Lights) {};
	void RenderWithGouraudShading(const LightSource& Lights) {};
	void draw();
	void drawW();


private:

	vec4 _vertices[8];
	int _iIndex;

	void Quad(int a, int b, int c, int d);

};






#endif // !CSOILDCUBE_H

