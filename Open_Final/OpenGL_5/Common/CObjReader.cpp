#include "CObjReader.h"
// Example 4 開始
// 面同 Example 3 朝上(Y軸)
// 每一個 Vertex 增加 Normal ，改成繼曾自 CShape，一併處理相關的設定需求


CObjReader::CObjReader(char* objfile)
{
	FILE* pfile;
	char pLineHead[50];
	int face[3][3];					//讀取用
	int ivec, inormal, iuv, ifaces;	//點、法向量、貼圖、面數紀錄
	ifaces = ivec = inormal = iuv = 0;

	if ((pfile = fopen(objfile, "r")) == NULL) {
		printf("obj file can't open."); system("pause");
	}
	while (!feof(pfile)) { //是否到檔案尾									//--------第一次讀檔，紀錄欲開空間大小--------//
		fscanf(pfile, "%s", pLineHead);						//讀取字串
		if (strcmp(pLineHead, "v") == 0) ivec++;			//讀取point
		else if (strcmp(pLineHead, "vn") == 0) inormal++;	//讀取normal
		else if (strcmp(pLineHead, "vt") == 0) iuv++;		//讀取texture
		else if (strcmp(pLineHead, "f") == 0) ifaces++;		//讀取face
	}

	_iNumVtx = ifaces * 3;				//點數
	_pPoints = NULL; _pNormals = NULL;
	_pTex = NULL; _pTex_light = NULL; _pTex_normal = NULL;

	_pPoints = new vec4[_iNumVtx];	//使用點
	_pNormals = new vec3[_iNumVtx];	//Normal
	_pColors = new vec4[_iNumVtx];	//顏色
	_pTex = new vec2[_iNumVtx];		//diffuse map
#ifdef LIGHT_MAP
	_pTex_light = new vec2[_iNumVtx];		// 產生 light map 所需的貼圖座標
#endif
#ifdef NORMAL_MAP
	_pTex_normal = new vec2[_iNumVtx];		// 產生 normal map 所需的貼圖座標
	_pTangent = new vec3[_iNumVtx];	// 儲存 Tangent Space Normal Map 的 Tangent vector
#endif

	_vec4Points = new vec4[ivec];		//資料點 (vec4)
	_vec3Points_n = new vec3[inormal];	//法向量 (vec3)
	_vec3Points_uv = new vec3[iuv];		//貼圖點 (vec2)

	int pCount, vCount, nCount, uvCount;
	pCount = vCount = nCount = uvCount = 0;
	rewind(pfile);	//重新指到檔案頭

	while (!feof(pfile)) { //是否到檔案尾									//--------第二次讀檔，資料紀錄--------//
		fscanf(pfile, "%s", pLineHead);			 //讀取字串
		if (strcmp(pLineHead, "v") == 0) {		 //讀取vertex
			fscanf(pfile, "%f %f %f", &_vec4Points[vCount].x, &_vec4Points[vCount].y, &_vec4Points[vCount].z); //讀取3點
			_vec4Points[vCount].w = 1;
			vCount++;
		}
		else if (strcmp(pLineHead, "vn") == 0) { //讀取normal
			fscanf(pfile, "%f %f %f", &_vec3Points_n[nCount].x, &_vec3Points_n[nCount].y, &_vec3Points_n[nCount].z); //讀取3點
			nCount++;
		}
		else if (strcmp(pLineHead, "vt") == 0) { //讀取texture
			fscanf(pfile, "%f %f %f", &_vec3Points_uv[uvCount].x, &_vec3Points_uv[uvCount].y, &_vec3Points_uv[uvCount].z); //讀取3點
			uvCount++;
		}
		else if (strcmp(pLineHead, "f") == 0) {	//讀取face
			fscanf(pfile, "%d/%d/%d %d/%d/%d %d/%d/%d", &face[0][0], &face[0][1], &face[0][2],
				&face[1][0], &face[1][1], &face[1][2],
				&face[2][0], &face[2][1], &face[2][2]); //頂點v/貼圖vt(uv)/法線vn
			for (int i = 0; i < 3; i++) {
				_pPoints[pCount + i] = _vec4Points[face[i][0] - 1];		// 讀取頂點
				_pNormals[pCount + i] = _vec3Points_n[face[i][2] - 1];		// 讀取Normal
				_pTex[pCount + i].x = _vec3Points_uv[face[i][1] - 1].x;	// Texture x
				_pTex[pCount + i].y = _vec3Points_uv[face[i][1] - 1].y;	// Texture y
			}
			pCount += 3;
		}
	}
	fclose(pfile); //關閉檔案


	//-----------------------Multitexturing--------------------------
	for (int i = 0; i < _iNumVtx; i++) {
#ifdef LIGHT_MAP
		_pTex_light[i] = _pTex[i];  // 產生 light map 所需的貼圖座標
#endif
#ifdef NORMAL_MAP
		_pTex_normal[i] = _pTex[i];;	// 產生 normal map 所需的貼圖座標
#endif
	}
#ifdef NORMAL_MAP
	// 計算 tangent vector
	for (int i = 0; i < _iNumVtx; i += 3) { // 三個 vertex 一組
		float dU1 = _pTex_normal[i + 1].x - _pTex_normal[i].x;
		float dV1 = _pTex_normal[i + 1].y - _pTex_normal[i].y;
		float dU2 = _pTex_normal[i + 2].x - _pTex_normal[i].x;
		float dV2 = _pTex_normal[i + 2].y - _pTex_normal[i].y;
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
	for (int i = 0; i < _iNumVtx; i++)
		_pTangent[i] = normalize(_pTangent[i]);
#endif


	// Set shader's name
	setShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");

	// Create and initialize a buffer object 
	//CreateBufferObject();

	// 預設將所有的面都設定成灰色
	for (int i = 0; i < _iNumVtx; i++) _pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f);

	// 設定材質
	setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));	//gray
	setKaKdKsShini(0, 0.8f, 0.2f, 1);
}

CObjReader::~CObjReader()
{
	//歸還空間
	if (_vec4Points != NULL) delete[] _vec4Points;
	if (_vec3Points_n != NULL) delete[] _vec3Points_n;
	if (_vec3Points_uv != NULL) delete[] _vec3Points_uv;
}

void CObjReader::draw()
{
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
}

void CObjReader::drawW()
{
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
}

//----------------------FLAT SHADING--------------------------
void CObjReader::RenderWithFlatShading(const LightSource& lights)
{
	// 以每一個面的三個頂點計算其重心，以該重心作為顏色計算的點頂
	// 根據 Phong lighting model 計算相對應的顏色，並將顏色儲存到此三個點頂
	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal

	vec4 vCentroidP;
	for (int i = 0; i < _iNumVtx; i += 3) {
		// 計算三角形的重心
		vCentroidP = (_pPoints[i] + _pPoints[i + 1] + _pPoints[i + 2]) / 3.0f;
		_pColors[i] = _pColors[i + 1] = _pColors[i + 2] = phongReflectionModel(vCentroidP, _pNormals[i], lights);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}

//----------------------GROUND SHADING--------------------------
void CObjReader::RenderWithGouraudShading(const LightSource& Lights)
{
	for (int i = 0; i < _iNumVtx; i++) {
		_pColors[i] = phongReflectionModel(_pPoints[i], _pNormals[i], Lights);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}

//----------------------UPDATE--------------------------

void CObjReader::update(float dt, const LightSource* Lights) {

	if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	/*vec3 vLightDir = _mxMVFinal * Lights[1].spotDirection;

	// 5. 計算照明的點是否落在 spotCutoff 之內
	GLfloat fLdotLDir = -(vLight.x * vLightDir.x + vLight.y * vLightDir.y + vLight.z * vLightDir.z);
	*/
	for (int i = 0; i < LIGHT_NUM_MAX; i++)
	{
		_vLightInView[i] = _mxView * Lights[i].position;		// 將 Light 轉換到鏡頭座標再傳入
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
		_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse;
		_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
		_cutoff[i] = Lights[i].spotCutoff;
		_vSpotTarget[i] = _mxView * Lights[i].spotTarget;

	}


}


mat4& CObjReader::GetTRSMatrix()
{
	return(_mxTRS);
}