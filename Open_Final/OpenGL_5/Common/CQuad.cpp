#include<cmath>
#include"CQuad.h"

CQuad::CQuad() {

	_iNumVtx = POINT_NUM;
	//_pPoints = NULL; _pNormals = NULL;
	_pTex = NULL;
	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];
	_pTex = new vec2[_iNumVtx];

//light map
#ifdef LIGHTMAP
	_pTex_light = new vec2[_iNumVtx];
#endif

//normal map
#ifdef NORMALMAP
	_pTex_normal = new vec2[_iNumVtx];
	_pTangent = new vec3[_iNumVtx];
#endif // NORMALMAP



	//�I�y��
	_pPoints[0] = vec4(-0.5f, 0.0f, 0.5f, 1.0f);
	_pPoints[1] = vec4(0.5f, 0.0f, 0.5f, 1.0f);
	_pPoints[2] = vec4(0.5f, 0.0f, -0.5f, 1.0f);
	_pPoints[3] = vec4(-0.5f, 0.0f, 0.5f, 1.0f);
	_pPoints[4] = vec4(0.5f, 0.0f, -0.5f, 1.0f);
	_pPoints[5] = vec4(-0.5f, 0.0f, -0.5f, 1.0f);

	//�k�V�q
	_pNormals[0] = vec3(0, 1.0f, 0);
	_pNormals[1] = vec3(0, 1.0f, 0);
	_pNormals[2] = vec3(0, 1.0f, 0);
	_pNormals[3] = vec3(0, 1.0f, 0);
	_pNormals[4] = vec3(0, 1.0f, 0);
	_pNormals[5] = vec3(0, 1.0f, 0);


	_pTex[0] = vec2(0.0f, 0.0f);
	_pTex[1] = vec2(1.0f, 0.0f);
	_pTex[2] = vec2(1.0f, 1.0f);
	_pTex[3] = vec2(0.0f, 0.0f);
	_pTex[4] = vec2(1.0f, 1.0f);
	_pTex[5] = vec2(0.0f, 1.0f);

	//light map
#ifdef LIGHTMAP

	_pTex_light[0] = vec2(0.0f, 0.0f);
	_pTex_light[1] = vec2(1.0f, 0.0f);
	_pTex_light[2] = vec2(1.0f, 1.0f);
	_pTex_light[3] = vec2(0.0f, 0.0f);
	_pTex_light[4] = vec2(1.0f, 1.0f);
	_pTex_light[5] = vec2(0.0f, 1.0f);

#endif // LIGHTMAP


#ifdef NORMALMAP
	_pTex_normal[0] = vec2(0.0f, 0.0f);
	_pTex_normal[1] = vec2(1.0f, 0.0f);
	_pTex_normal[2] = vec2(1.0f, 1.0f);
	_pTex_normal[3] = vec2(0.0f, 0.0f);
	_pTex_normal[4] = vec2(1.0f, 1.0f);
	_pTex_normal[5] = vec2(0.0f, 1.0f);

	for (int i = 0; i < 6; i += 3) { // �T�� vertex �@��
		float dU1 = _pTex[i + 1].x - _pTex[i].x;
		float dV1 = _pTex[i + 1].y - _pTex[i].y;
		float dU2 = _pTex[i + 2].x - _pTex[i].x;
		float dV2 = _pTex[i + 2].y - _pTex[i].y;
		float f = 1.0f / (dU1 * dV2 - dU2 * dV1);
		vec4 E1 = _pPoints[i + 1] - _pPoints[i];
		vec4 E2 = _pPoints[i + 2] - _pPoints[i];

		vec3 tangent;
		tangent.x = f * (dV2 * E1.x + E2.x * (-dV1));
		tangent.y = f * (dV2 * E1.y + E2.y * (-dV1));
		tangent.z = f * (dV2 * E1.z + E2.z * (-dV1));

		_pTangent[i] += tangent;
		_pTangent[i + 1] += tangent;
		_pTangent[i + 2] = tangent;
	}
	for (int i = 0; i < 6; i++)
		_pTangent[i] = normalize(_pTangent[i]);


#endif // NORMALMAP


	//�C��
	for (int i = 0; i < _iNumVtx; i++) _pColors[i] = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	setShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");

}

void CQuad::setNormal(vec3 vNormal) {
	_pNormals[0] = vNormal;
	_pNormals[1] = vNormal;
	_pNormals[2] = vNormal;
	_pNormals[3] = vNormal;
	_pNormals[4] = vNormal;
	_pNormals[5] = vNormal;
	
}


void CQuad::renderWithFlatShading(vec4 vLightPos, color4 vLight) {
	// �]���C�@�ӥ��������I�� Normal ���ۦP�A�ҥH���B�èS���p�⦹�T�ӳ��I������ Normal
	vec4 vCentroid;
	for (int i = 0; i < _iNumVtx; i+=3)
	{
		vCentroid = (_pPoints[i] + _pPoints[i + 1] + _pPoints[i + 2]) / 3.0f;
		_pColors[i] = _pColors[i + 1] = _pColors[i + 2] = phongReflectionModel(vCentroid,_pNormals[i],vLightPos,vLight);

	}
	glBindBuffer(GL_ARRAY_BUFFER,_vbo);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec4)*_iNumVtx+ sizeof(vec3) * _iNumVtx , sizeof(vec4) * _iNumVtx ,_pColors );
}

void CQuad::renderWithFlatShading(const LightSource& Lights)
{
	vec4 vCentroid;
	for (int i = 0; i < _iNumVtx; i += 3) {
		// �p��T���Ϊ�����
		vCentroid = (_pPoints[i] + _pPoints[i + 1] + _pPoints[i + 2]) / 3.0f;
		_pColors[i] = _pColors[i + 1] = _pColors[i + 2] = phongReflectionModel(vCentroid, _pNormals[i], Lights.position, Lights.diffuse);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}

void CQuad::renderWithGouraudShading(vec4 vLightPos, color4 vLight) {

	_pColors[0] = _pColors[3] = phongReflectionModel(_pPoints[0],_pNormals[0],vLightPos,vLight);
	_pColors[2] = _pColors[4] = phongReflectionModel(_pPoints[2], _pNormals[2], vLightPos, vLight);
	_pColors[1] = phongReflectionModel(_pPoints[1], _pNormals[1], vLightPos, vLight);
	_pColors[5]  = phongReflectionModel(_pPoints[5], _pNormals[5], vLightPos, vLight);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color


}


void CQuad::renderWithGouraudShading(const LightSource& Lights)
{
	
	_pColors[0] = _pColors[3] = phongReflectionModel(_pPoints[0], _pNormals[0], Lights);
	_pColors[2] = _pColors[4] = phongReflectionModel(_pPoints[2], _pNormals[2], Lights);
	_pColors[1] = phongReflectionModel(_pPoints[1], _pNormals[1], Lights);
	_pColors[5] = phongReflectionModel(_pPoints[5], _pNormals[5], Lights);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}


// ���B�ҵ��� vLightPos �����O�@�ɮy�Ъ��T�w�����m
void CQuad::update(float dt, vec4 vLightPos, color4 vLightI)
{

	/*if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	_vLightInView = _mxView * vLightPos;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
	// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	_ambientProduct = _material.ka * _material.ambient * vLightI;
	_diffuseProduct = _material.kd * _material.diffuse * vLightI;
	_specularProduct = _material.ks * _material.specular * vLightI;*/


}

void CQuad::update(float dt, const LightSource* Lights)
{

	if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	/*vec3 vLightDir = _mxMVFinal * Lights[1].spotDirection;

	// 5. �p��ө����I�O�_���b spotCutoff ����
	GLfloat fLdotLDir = -(vLight.x * vLightDir.x + vLight.y * vLightDir.y + vLight.z * vLightDir.z);
	*/
	for (int i = 0; i < LIGHT_NUM_MAX; i++)
	{
		_vLightInView[i] = _mxView * Lights[i].position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
		_vSpotTarget[i] = _mxView * Lights[i].spotTarget;
		// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
		_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
		_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse ;
		_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
		_cutoff[i] = Lights[i].spotCutoff;

		
	}
	

}

void CQuad::update(float dt, const LightSource& Lights)
{

	/*if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	_vLightInView = _mxView * Lights.position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
	// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	_ambientProduct = _material.ka * _material.ambient * Lights.ambient;
	_diffuseProduct = _material.kd * _material.diffuse * Lights.diffuse;
	_specularProduct = _material.ks * _material.specular * Lights.specular;*/

}

void CQuad::update(float dt) {
	if (_viewUpdated||_TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_mxITMV = InverseTransposeMatrix(_mxMVFinal);
		_viewUpdated = _TRSUpdated = false;

	}

}

void CQuad::setVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor) {
	_pColors[3] = _pColors[0] = vLFColor;
	_pColors[1] = vLRColor;
	_pColors[4] = _pColors[2] = vTRColor;
	_pColors[5] = vTLColor;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color

}

void CQuad::setVtxColors(vec4 vFColor, vec4 vSColor) {
	_pColors[0] = _pColors[1] = _pColors[2] = vFColor;
	_pColors[3] = _pColors[4] = _pColors[5] = vSColor;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color

}

void CQuad::draw() {
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_NUM);

}
void CQuad::drawW() {
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_NUM);

}