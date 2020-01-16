#include "CObjReader.h"
// Example 4 �}�l
// ���P Example 3 �¤W(Y�b)
// �C�@�� Vertex �W�[ Normal �A�令�~���� CShape�A�@�ֳB�z�������]�w�ݨD


CObjReader::CObjReader(char* objfile)
{
	FILE* pfile;
	char pLineHead[50];
	int face[3][3];					//Ū����
	int ivec, inormal, iuv, ifaces;	//�I�B�k�V�q�B�K�ϡB���Ƭ���
	ifaces = ivec = inormal = iuv = 0;

	if ((pfile = fopen(objfile, "r")) == NULL) {
		printf("obj file can't open."); system("pause");
	}
	while (!feof(pfile)) { //�O�_���ɮק�									//--------�Ĥ@��Ū�ɡA�������}�Ŷ��j�p--------//
		fscanf(pfile, "%s", pLineHead);						//Ū���r��
		if (strcmp(pLineHead, "v") == 0) ivec++;			//Ū��point
		else if (strcmp(pLineHead, "vn") == 0) inormal++;	//Ū��normal
		else if (strcmp(pLineHead, "vt") == 0) iuv++;		//Ū��texture
		else if (strcmp(pLineHead, "f") == 0) ifaces++;		//Ū��face
	}

	_iNumVtx = ifaces * 3;				//�I��
	_pPoints = NULL; _pNormals = NULL;
	_pTex = NULL; _pTex_light = NULL; _pTex_normal = NULL;

	_pPoints = new vec4[_iNumVtx];	//�ϥ��I
	_pNormals = new vec3[_iNumVtx];	//Normal
	_pColors = new vec4[_iNumVtx];	//�C��
	_pTex = new vec2[_iNumVtx];		//diffuse map
#ifdef LIGHT_MAP
	_pTex_light = new vec2[_iNumVtx];		// ���� light map �һݪ��K�Ϯy��
#endif
#ifdef NORMAL_MAP
	_pTex_normal = new vec2[_iNumVtx];		// ���� normal map �һݪ��K�Ϯy��
	_pTangent = new vec3[_iNumVtx];	// �x�s Tangent Space Normal Map �� Tangent vector
#endif

	_vec4Points = new vec4[ivec];		//����I (vec4)
	_vec3Points_n = new vec3[inormal];	//�k�V�q (vec3)
	_vec3Points_uv = new vec3[iuv];		//�K���I (vec2)

	int pCount, vCount, nCount, uvCount;
	pCount = vCount = nCount = uvCount = 0;
	rewind(pfile);	//���s�����ɮ��Y

	while (!feof(pfile)) { //�O�_���ɮק�									//--------�ĤG��Ū�ɡA��Ƭ���--------//
		fscanf(pfile, "%s", pLineHead);			 //Ū���r��
		if (strcmp(pLineHead, "v") == 0) {		 //Ū��vertex
			fscanf(pfile, "%f %f %f", &_vec4Points[vCount].x, &_vec4Points[vCount].y, &_vec4Points[vCount].z); //Ū��3�I
			_vec4Points[vCount].w = 1;
			vCount++;
		}
		else if (strcmp(pLineHead, "vn") == 0) { //Ū��normal
			fscanf(pfile, "%f %f %f", &_vec3Points_n[nCount].x, &_vec3Points_n[nCount].y, &_vec3Points_n[nCount].z); //Ū��3�I
			nCount++;
		}
		else if (strcmp(pLineHead, "vt") == 0) { //Ū��texture
			fscanf(pfile, "%f %f %f", &_vec3Points_uv[uvCount].x, &_vec3Points_uv[uvCount].y, &_vec3Points_uv[uvCount].z); //Ū��3�I
			uvCount++;
		}
		else if (strcmp(pLineHead, "f") == 0) {	//Ū��face
			fscanf(pfile, "%d/%d/%d %d/%d/%d %d/%d/%d", &face[0][0], &face[0][1], &face[0][2],
				&face[1][0], &face[1][1], &face[1][2],
				&face[2][0], &face[2][1], &face[2][2]); //���Iv/�K��vt(uv)/�k�uvn
			for (int i = 0; i < 3; i++) {
				_pPoints[pCount + i] = _vec4Points[face[i][0] - 1];		// Ū�����I
				_pNormals[pCount + i] = _vec3Points_n[face[i][2] - 1];		// Ū��Normal
				_pTex[pCount + i].x = _vec3Points_uv[face[i][1] - 1].x;	// Texture x
				_pTex[pCount + i].y = _vec3Points_uv[face[i][1] - 1].y;	// Texture y
			}
			pCount += 3;
		}
	}
	fclose(pfile); //�����ɮ�


	//-----------------------Multitexturing--------------------------
	for (int i = 0; i < _iNumVtx; i++) {
#ifdef LIGHT_MAP
		_pTex_light[i] = _pTex[i];  // ���� light map �һݪ��K�Ϯy��
#endif
#ifdef NORMAL_MAP
		_pTex_normal[i] = _pTex[i];;	// ���� normal map �һݪ��K�Ϯy��
#endif
	}
#ifdef NORMAL_MAP
	// �p�� tangent vector
	for (int i = 0; i < _iNumVtx; i += 3) { // �T�� vertex �@��
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

	// �w�]�N�Ҧ��������]�w���Ǧ�
	for (int i = 0; i < _iNumVtx; i++) _pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f);

	// �]�w����
	setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));	//gray
	setKaKdKsShini(0, 0.8f, 0.2f, 1);
}

CObjReader::~CObjReader()
{
	//�k�٪Ŷ�
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
	// �H�C�@�ӭ����T�ӳ��I�p��䭫�ߡA�H�ӭ��ߧ@���C��p�⪺�I��
	// �ھ� Phong lighting model �p��۹������C��A�ñN�C���x�s�즹�T���I��
	// �]���C�@�ӥ��������I�� Normal ���ۦP�A�ҥH���B�èS���p�⦹�T�ӳ��I������ Normal

	vec4 vCentroidP;
	for (int i = 0; i < _iNumVtx; i += 3) {
		// �p��T���Ϊ�����
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
		_vSpotTarget[i] = _mxView * Lights[i].spotTarget;

	}


}


mat4& CObjReader::GetTRSMatrix()
{
	return(_mxTRS);
}