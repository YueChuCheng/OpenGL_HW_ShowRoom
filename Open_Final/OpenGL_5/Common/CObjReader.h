#ifndef COBJREADER_H
#define COBJREADER_H
#include "../header/Angel.h"
#include "CShape.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

//#define QUAD_NUM 6		// 2 faces, 2 triangles/face 

class CObjReader : public CShape
{
private:
	vec4* _vec4Points;			//����I (vec4)
	vec3* _vec3Points_n;		//����I (vec3)
	vec3* _vec3Points_uv;		//����I (vec3)

public:
	CObjReader(char*);
	~CObjReader();
	mat4& GetTRSMatrix();

	
	void draw() ;
	void drawW() ;
	
	
	void update(float dt, point4 vLightPos, color4 vLight) {};
	void update(float dt, const LightSource& lights) {};
	void update(float dt, const LightSource* lights);
	void update(float dt) {};


	/*void Update(float dt, point4 vLightPos, color4 vLightI);
	void Update(float dt, const LightSource& Lights);	//�@�ӥ���
	void Update(float dt, const LightSource& Lights, const LightSource& Lights2);	//��ӥ���
	void Update(float dt, const LightSource& Lights, const LightSource& Lights2, const LightSource& Lights3);	//�T�ӥ���
	void Update(float dt, const LightSource& Lights, const LightSource& Lights2, const LightSource& Lights3, const LightSource& Lights4);	//�|�ӥ���
	void Update(float dt); // ���p��������ө�*/




	void RenderWithFlatShading(const LightSource& Lights);
	void RenderWithGouraudShading(const LightSource& Lights);


};

#endif