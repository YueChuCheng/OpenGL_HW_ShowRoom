#include "test.h"
#include "../CObj_loader.h"

ModelPool::ModelPool(const char* path) {
	std::vector<float> vertices, normals;

	LoadObj(path, &vertices, &normals);

	//Print(vertices.size() / 3);
	_iNumVtx = vertices.size() / 3;
	_pPoints = NULL; _pNormals = NULL; _pTex = NULL;

	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];
	_pTex = new vec2[_iNumVtx];

	for (int i = 0; i < _iNumVtx; i++)
	{
		_pPoints[i] = vec4(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0f);
		_pNormals[i] = vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		_pColors[i] = vec4(0.5f, 0.5f, 0.5f, 1);

		
	}
	//Print(m_pNormals);
	setShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");

	/*setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	setKaKdKsShini(0, 0.8f, 0.5f, 1);

	setShader();
	setShadingMode(GOURAUD_SHADING);*/
}

void ModelPool::draw()
{
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
}

void ModelPool::drawW()
{
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
}




void ModelPool::setLightNUM(int num) {

	_iLightNUM = num;

}



void ModelPool::update(float dt, const LightSource* Lights)
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
		// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
		_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
		_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse;
		_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
		_cutoff[i] = Lights[i].spotCutoff;

	}


}