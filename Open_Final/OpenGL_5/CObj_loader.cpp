//#include "CObj_loader.h"
//
//
//CObj_loader::CObj_loader(const char* path)
//{
//	std::vector<float> vertices, normals;
//	LoadObjFromFileVertexAndNormal(path, &vertices, &normals);
//	
//	_iNumVtx = vertices.size() / 3;
//
//	_pPoints = NULL; _pNormals = NULL; _pTex = NULL;
//
//	_pPoints = new vec4[_iNumVtx];
//	_pNormals = new vec3[_iNumVtx];
//	_pColors = new vec4[_iNumVtx];
//	_pTex = new vec2[_iNumVtx];
//
//	//_pPoints[0].x = vertices->at(0);
//	for (int i = 0; i < _iNumVtx; i += 3)
//	{
//		_pPoints[i] = vec4(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0f);
//		_pNormals[i] = vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
//		_pColors[i] = vec4(0.5f, 0.5f, 0.5f, 1);
//
//	}
//
//	setShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
//
//	
//
//
//	setMaterials(vec4(0), vec4(1.0f, 1.0, 1.0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	setKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
//
//	setShader();
//	setShadingMode(GOURAUD_SHADING);
//
//}
//
//CObj_loader::~CObj_loader()
//{
//}
//
//
//bool CObj_loader::LoadObjFromFileVertexAndNormal(const char* path, std::vector<float>* vertices, std::vector<float>* normals) {
//	char lineHeader[1000];
//	std::vector<float> temp_vertices;
//	std::vector<float> temp_normals;
//	std::vector<unsigned int> indices;
//	float vertex[3], normal[3];
//	unsigned int index[3][3], f = 0;
//	FILE* objfile = fopen(path, "r");
//
//	if (objfile == NULL) {
//		printf("Impossible to open the file !\n");
//		return false;
//	}
//
//	while (fscanf(objfile, "%s", lineHeader) != EOF) {
//		if (strcmp(lineHeader, "v") == 0) {
//			fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
//			temp_vertices.push_back(vertex[0]);
//			temp_vertices.push_back(vertex[1]);
//			temp_vertices.push_back(vertex[2]);
//		}
//		else if (strcmp(lineHeader, "vn") == 0) {
//			fscanf(objfile, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
//			temp_normals.push_back(normal[0]);
//			temp_normals.push_back(normal[1]);
//			temp_normals.push_back(normal[2]);
//		}
//		else if (strcmp(lineHeader, "f") == 0) {
//			int matches = fscanf(objfile, "%u/%u/%u %u/%u/%u %u/%u/%u", &index[0][0], &index[1][0], &index[2][0], &index[0][1], &index[1][1], &index[2][1], &index[0][2], &index[1][2], &index[2][2]);
//			//printf("matches=%d,f=%u\n", matches, f++);
//			if (matches != 9) {
//				printf("Impossible to open the file !\n");
//				return false;
//			}
//			indices.push_back(index[0][0]);
//			indices.push_back(index[2][0]);
//			indices.push_back(index[0][1]);
//			indices.push_back(index[2][1]);
//			indices.push_back(index[0][2]);
//			indices.push_back(index[2][2]);
//		}
//	}
//	//Print(indices.size());
//	for (unsigned int i = 0; i < indices.size(); i += 2) {
//		vertices->push_back(temp_vertices[(indices[i] - 1) * 3]);
//		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 1]);
//		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 2]);
//		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3]);
//		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3 + 1]);
//		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3 + 2]);
//	}
//}
//
//
//
//void CObj_loader::update(float dt, vec4 vLightPos, color4 vLightI)
//{
//
//	/*if (_viewUpdated || _TRSUpdated) {
//		_mxMVFinal = _mxView * _mxTRS;
//		_viewUpdated = _TRSUpdated = false;
//	}
//	_vLightInView = _mxView * vLightPos;		// 將 Light 轉換到鏡頭座標再傳入
//	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
//	_ambientProduct = _material.ka * _material.ambient * vLightI;
//	_diffuseProduct = _material.kd * _material.diffuse * vLightI;
//	_specularProduct = _material.ks * _material.specular * vLightI;*/
//
//
//}
//
//void CObj_loader::update(float dt, const LightSource* Lights)
//{
//
//	if (_viewUpdated || _TRSUpdated) {
//		_mxMVFinal = _mxView * _mxTRS;
//		_viewUpdated = _TRSUpdated = false;
//	}
//	/*vec3 vLightDir = _mxMVFinal * Lights[1].spotDirection;
//
//	// 5. 計算照明的點是否落在 spotCutoff 之內
//	GLfloat fLdotLDir = -(vLight.x * vLightDir.x + vLight.y * vLightDir.y + vLight.z * vLightDir.z);
//	*/
//	for (int i = 0; i < LIGHT_NUM_MAX; i++)
//	{
//		_vLightInView[i] = _mxView * Lights[i].position;		// 將 Light 轉換到鏡頭座標再傳入
//		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
//		_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
//		_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse;
//		_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
//		_cutoff[i] = Lights[i].spotCutoff;
//
//	}
//
//
//}
//
//void CObj_loader::update(float dt, const LightSource& Lights)
//{
//
//	/*if (_viewUpdated || _TRSUpdated) {
//		_mxMVFinal = _mxView * _mxTRS;
//		_viewUpdated = _TRSUpdated = false;
//	}
//	_vLightInView = _mxView * Lights.position;		// 將 Light 轉換到鏡頭座標再傳入
//	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
//	_ambientProduct = _material.ka * _material.ambient * Lights.ambient;
//	_diffuseProduct = _material.kd * _material.diffuse * Lights.diffuse;
//	_specularProduct = _material.ks * _material.specular * Lights.specular;*/
//
//}
//
//void CObj_loader::update(float dt) {
//	if (_viewUpdated || _TRSUpdated) {
//		_mxMVFinal = _mxView * _mxTRS;
//		_mxITMV = InverseTransposeMatrix(_mxMVFinal);
//		_viewUpdated = _TRSUpdated = false;
//
//	}
//
//}
//
//void CObj_loader::setVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor) {
//	_pColors[3] = _pColors[0] = vLFColor;
//	_pColors[1] = vLRColor;
//	_pColors[4] = _pColors[2] = vTRColor;
//	_pColors[5] = vTLColor;
//
//	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
//
//}
//
//void CObj_loader::setVtxColors(vec4 vFColor, vec4 vSColor) {
//	_pColors[0] = _pColors[1] = _pColors[2] = vFColor;
//	_pColors[3] = _pColors[4] = _pColors[5] = vSColor;
//
//	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
//
//}
//
//void CObj_loader::draw() {
//	drawingSetShader();
//	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
//
//}
//void CObj_loader::drawW() {
//	drawingWithoutSetShader();
//	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
//
//}


#include "CObj_loader.h"

bool LoadObj(const char* path, std::vector<float>* vertices, std::vector<float>* normals) {
	char lineHeader[1000];
	std::vector<float> temp_vertices;
	std::vector<float> temp_normals;
	std::vector<unsigned int> indices;

	std::vector<unsigned int> index_vertice , index_normal;
	
	
	float vertex[3], normal[3];
	unsigned int index[3][3], f = 0;
	FILE* objfile = fopen(path, "r");

	if (objfile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (fscanf(objfile, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {
			fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			temp_vertices.push_back(vertex[0]);
			temp_vertices.push_back(vertex[1]);
			temp_vertices.push_back(vertex[2]);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			fscanf(objfile, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal[0]);
			temp_normals.push_back(normal[1]);
			temp_normals.push_back(normal[2]);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			int matches = fscanf(objfile, "%u/%u/%u %u/%u/%u %u/%u/%u", &index[0][0], &index[1][0], &index[2][0], &index[0][1], &index[1][1], &index[2][1], &index[0][2], &index[1][2], &index[2][2]);
			//printf("matches=%d,f=%u\n", matches, f++);
			if (matches != 9) {
				printf("Impossible to open the file !\n");
				return false;
			}

			/*index_vertice.push_back(index[0][0]);
			index_vertice.push_back(index[0][1]);
			index_vertice.push_back(index[0][2]);

			index_normal.push_back(index[2][0]);
			index_normal.push_back(index[2][1]);
			index_normal.push_back(index[2][2]);*/

			indices.push_back(index[0][0]);
			indices.push_back(index[2][0]);
			indices.push_back(index[0][1]);
			indices.push_back(index[2][1]);
			indices.push_back(index[0][2]);
			indices.push_back(index[2][2]);
		}
	}
	//Print(indices.size());
	for (unsigned int i = 0; i < indices.size(); i += 2) {
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 1]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 2]);
		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3]);
		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3 + 1]);
		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3 + 2]);
	}

	/*for (unsigned int i = 0; i < index_vertice.size(); i ++) {

		vertices->push_back(temp_vertices[index_vertice[i] - 1] * 3 + 0);
		vertices->push_back(temp_vertices[index_vertice[i] - 1] * 3 + 1);
		vertices->push_back(temp_vertices[index_vertice[i] - 1] * 3 + 2);

		normals->push_back(temp_normals[index_normal[i] - 1] * 3 + 0);
		normals->push_back(temp_normals[index_normal[i] - 1] * 3 + 1);
		normals->push_back(temp_normals[index_normal[i] - 1] * 3 + 2);

	}*/


}