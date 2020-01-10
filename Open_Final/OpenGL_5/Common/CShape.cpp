#include "CShape.h"



CShape::CShape() {

	_TRSUpdated = _viewUpdated = _projUpdated = false;
	// ambient �w�]�� 0, diffuse, specular ���C�ⳣ�O�Ǧ� 0.5
	// Ka = 0 �Y�� , kd = 0.8 , ks = 0.2
	_material.ambient = vec4(vec3(0));
	_material.diffuse = vec4(vec3(0.5f));
	_material.specular = vec4(vec3(0.5f));
	_material.ka = 0; _material.kd = 0.8f; _material.ks = 0.2f;
	_material.shininess = 2.0f;

	_iMode = FLAT_SHADING;

	// �w�]�� RGBA �� (0.5,0.5,0.5,1.0) , �ѳo�ӦǶ��C��ӥN�������C��
	_Color[0] = 0.5f; _Color[1] = 0.5f; _Color[2] = 0.5f; _Color[3] = 1.0f;
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		_iLighting[i] = 1; // �w�]�����O�����ө�

	}

	m_iTexLayer = 0;		// �w�]��0�i Diffuse �K��
	_pPoints = nullptr; 	_pNormals = nullptr; 	_pColors = nullptr; 	_pTex = nullptr;

	//light map
#ifdef LIGHTMAP
	_pTex_light = nullptr;
#endif

#ifdef NORMALMAP
	_pTex_normal = nullptr;
	_pTangent = nullptr;
#endif // NORMALMAP




}

//���^�Ŷ�
CShape::~CShape() {
	if (_pPoints != NULL)delete[] _pPoints;
	if (_pNormals != NULL)delete[] _pNormals;
	if (_pColors != NULL)delete[] _pColors;
	if (_pTex != NULL)delete[] _pTex;


#ifdef LIGHTMAP
	if (_pTex_light != NULL) delete _pTex_light;
#endif
#ifdef NORMALMAP
	if (_pTex_normal != NULL) delete _pTex_normal;
	if (_pTangent != NULL) delete[] _pTangent;
#endif



	if (_pVXshader != NULL)delete[]_pVXshader;
	if (_pFShader != NULL)delete[] _pFShader;
}

void CShape::createBufferObject()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);


#ifndef LIGHTMAP 
glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2)/*�s�W�K�Ϯy�Ъ���m*/) * _iNumVtx, NULL, GL_STATIC_DRAW);
//sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx + sizeof(vec4)*m_iNumVtx <- vertices, normal and color
#endif

#ifdef LIGHTMAP 
#ifndef NORMALMAP
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2) * 2) * _iNumVtx, NULL, GL_STATIC_DRAW); // �]�t��i�K��
#endif
#endif

#ifdef NORMALMAP
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2) * 3 + sizeof(vec3)) * _iNumVtx, NULL, GL_STATIC_DRAW); // �]�t�T�i�K�ϻP tangent
#endif // NORMALMAP


	

	// vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * _iNumVtx, _pPoints);
	// vertices' normal
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx, sizeof(vec3) * _iNumVtx, _pNormals);
	// vertcies' Color
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3)) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors);

	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4)) * _iNumVtx, sizeof(vec2)/*�K�Ϯy�Ъ��j�p*/ * _iNumVtx, _pTex/*�K�Ϯy��*/);

#ifdef LIGHTMAP
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2)) * _iNumVtx, sizeof(vec2) * _iNumVtx, _pTex_light); // �ĤG�i�K��
#endif


#ifdef NORMALMAP
// Normal Map's Texture Coordinates
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2) * 2) * _iNumVtx, sizeof(vec2) * _iNumVtx, _pTex_normal); // �ĤT�i�K��
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2) * 3) * _iNumVtx, sizeof(vec3) * _iNumVtx, _pTangent); // �ĤT�i�K��
	// Vertex's Tangent
#endif

}


void CShape::setShader(GLuint shaderHandle) {

	createBufferObject();

	if (shaderHandle == MAX_UNSIGNED_INT)_vbo = InitShader(_pVXshader, _pFShader);
	else _vbo = shaderHandle;
	GLuint vPosition = glGetAttribLocation(_vbo, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(_vbo, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * _iNumVtx));

	GLuint vColorVtx = glGetAttribLocation(_vbo, "vVtxColor");  // vertices' color 
	glEnableVertexAttribArray(vColorVtx);
	glVertexAttribPointer(vColorVtx, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4) + sizeof(vec3)) * _iNumVtx));

	GLuint vDifMapCoord = glGetAttribLocation(_vbo, "vDiffuseMapCoord");  // vertices' texture coordinates //��K�Ϯy�ХΤ��P�W�l�ǤJ
	glEnableVertexAttribArray(vDifMapCoord);
	glVertexAttribPointer(vDifMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4) + sizeof(vec3) + sizeof(vec4)) * _iNumVtx));
	glUniform1i(glGetUniformLocation(_vbo, "diffuMap"), 0);// 0 : glAtiveTexture(GL_TEXTURE0);

#ifdef LIGHTMAP
	GLuint vLightMapCoord = glGetAttribLocation(_vbo, "vLightMapCoord");  // Light maps' texture coordinates�A �����s�W�� Shader �� //��K�Ϯy�ХΤ��P�W�l�ǤJ
	glEnableVertexAttribArray(vLightMapCoord);
	glVertexAttribPointer(vLightMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2)) * _iNumVtx));
	glUniform1i(glGetUniformLocation(_vbo, "lightMap"), 1); // 1 : glAtiveTexture(GL_TEXTURE1);
#endif // LIGHTMAP

#ifdef NORMALMAP
	// For Normal Map
	GLuint vNormalMapCoord = glGetAttribLocation(_vbo, "vNormalMapCoord");  // Light maps' texture coordinates�A �����s�W�� Shader ��
	glEnableVertexAttribArray(vNormalMapCoord);
	glVertexAttribPointer(vNormalMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2) * 2) * _iNumVtx));
	glUniform1i(glGetUniformLocation(_vbo, "normalMap"), 2);

	GLuint vTangentVec = glGetAttribLocation(_vbo, "vTangentV");  // vertices' color 
	glEnableVertexAttribArray(vTangentVec);
	glVertexAttribPointer(vTangentVec, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2) * 3) * _iNumVtx));

#endif

	_uiModelView = glGetUniformLocation(_vbo, "ModelView");
	// m_mxMVFinal , m_mxModelView �ŧi�ɴN�O���x�}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	_uiProjection = glGetUniformLocation(_vbo, "Projection");
	// m_mxProjection �ŧi�ɴN�O���x�}
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);

	_uiColor = glGetUniformLocation(_vbo, "vObjectColor");
	glUniform4fv(_uiColor, 1, _Color);


	//lighting with GPU

	_uiShininess = glGetUniformLocation(_vbo, "fShininess");
	glUniform1f(_uiShininess, _material.shininess);

	
	_uiLightInView[0] = glGetUniformLocation(_vbo, "LightInView[0]");
	glUniform4fv(_uiLightInView[0], 1, _vLightInView[0]);

	_uiLightInView[1] = glGetUniformLocation(_vbo, "LightInView[1]");
	glUniform4fv(_uiLightInView[1], 1, _vLightInView[1]);

	_uiLightInView[2] = glGetUniformLocation(_vbo, "LightInView[2]");
	glUniform4fv(_uiLightInView[2], 1, _vLightInView[2]);

	_uiLightInView[3] = glGetUniformLocation(_vbo, "LightInView[3]");
	glUniform4fv(_uiLightInView[3], 1, _vLightInView[3]);



	_uiLightNUM = glGetUniformLocation(_vbo, "LightNUM");
	glUniform1i(_uiLightNUM, _vLightNUM);


		
	
	_uiAmbient[0] = glGetUniformLocation(_vbo, "AmbientProduct[0]");
	glUniform4fv(_uiAmbient[0], 1, _ambientProduct[0]);

	_uiAmbient[1] = glGetUniformLocation(_vbo, "AmbientProduct[1]");
	glUniform4fv(_uiAmbient[1], 1, _ambientProduct[1]);

	_uiAmbient[2] = glGetUniformLocation(_vbo, "AmbientProduct[2]");
	glUniform4fv(_uiAmbient[2], 1, _ambientProduct[2]);

	_uiAmbient[3] = glGetUniformLocation(_vbo, "AmbientProduct[3]");
	glUniform4fv(_uiAmbient[3], 1, _ambientProduct[3]);
		

	_uiDiffuse[0] = glGetUniformLocation(_vbo, "DiffuseProduct[0]");
	glUniform4fv(_uiDiffuse[0], 1, _diffuseProduct[0]);
		
	_uiDiffuse[1] = glGetUniformLocation(_vbo, "DiffuseProduct[1]");
	glUniform4fv(_uiDiffuse[1], 1, _diffuseProduct[1]);

	_uiDiffuse[2] = glGetUniformLocation(_vbo, "DiffuseProduct[2]");
	glUniform4fv(_uiDiffuse[2], 1, _diffuseProduct[2]);

	_uiDiffuse[3] = glGetUniformLocation(_vbo, "DiffuseProduct[3]");
	glUniform4fv(_uiDiffuse[3], 1, _diffuseProduct[3]);
		
	_uiSpecular[0] = glGetUniformLocation(_vbo, "SpecularProduct[0]");
	glUniform4fv(_uiSpecular[0], 1, _specularProduct[0]);

	_uiSpecular[1] = glGetUniformLocation(_vbo, "SpecularProduct[1]");
	glUniform4fv(_uiSpecular[1], 1, _specularProduct[1]);
		
	_uiSpecular[2] = glGetUniformLocation(_vbo, "SpecularProduct[2]");
	glUniform4fv(_uiSpecular[2], 1, _specularProduct[2]);

	_uiSpecular[3] = glGetUniformLocation(_vbo, "SpecularProduct[3]");
	glUniform4fv(_uiSpecular[3], 1, _specularProduct[3]);
		
	_uiLighting[0] = glGetUniformLocation(_vbo, "iLighting[0]");
	glUniform1i(_uiLighting[0], _iLighting[0]);

	_uiLighting[1] = glGetUniformLocation(_vbo, "iLighting[1]");
	glUniform1i(_uiLighting[1], _iLighting[1]);
		
	_uiLighting[2] = glGetUniformLocation(_vbo, "iLighting[2]");
	glUniform1i(_uiLighting[2], _iLighting[2]);

	_uiLighting[3] = glGetUniformLocation(_vbo, "iLighting[3]");
	glUniform1i(_uiLighting[3], _iLighting[3]);
	
	_uicutoff[0] = glGetUniformLocation(_vbo, "Cutoff[0]");
	glUniform1f(_uicutoff[0], _cutoff[0]);

	_uicutoff[1] = glGetUniformLocation(_vbo, "Cutoff[1]");
	glUniform1f(_uicutoff[1], _cutoff[1]);
		
	_uicutoff[2] = glGetUniformLocation(_vbo, "Cutoff[2]");
	glUniform1f(_uicutoff[2], _cutoff[2]);

	_uicutoff[3] = glGetUniformLocation(_vbo, "Cutoff[3]");
	glUniform1f(_uicutoff[3], _cutoff[3]);
		
	m_uiTexLayer = glGetUniformLocation(_vbo, "iTexLayer");
	glUniform1i(m_uiTexLayer, m_iTexLayer);	// �K�Ϫ��ӼơA�w�]�� 1�A�����ǤJ pixel shader


	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void CShape::drawingSetShader() {

	glUseProgram(_vbo);
	glBindVertexArray(_vao);
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);
	if (_viewUpdated || _TRSUpdated) { // Model View �������x�}���e�����
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);
	
	if (_projUpdated) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_projUpdated = false;
	}

	glUniform4fv(_uiColor, 1, _Color);
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		glUniform4fv(_uiLightInView[i], 1, _vLightInView[i]);
		glUniform4fv(_uiAmbient[i], 1, _ambientProduct[i]);
		glUniform4fv(_uiDiffuse[i], 1, _diffuseProduct[i]);
		glUniform4fv(_uiSpecular[i], 1, _specularProduct[i]);
		glUniform1i(_uiLighting[i], _iLighting[i]);
		glUniform1f(_uicutoff[i], _cutoff[i]);
	}
	
	
	glUniform1i(m_uiTexLayer, m_iTexLayer);
	glUniform1f(_uiShininess, _material.shininess);
	

}

// ���B�w�]�ϥΫe�@�Ӵyø����ҨϥΪ� Shader
void CShape::drawingWithoutSetShader()
{
	glBindVertexArray(_vao);
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	if (_viewUpdated || _TRSUpdated) { // Model View �������x�}���e�����
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	if (_projUpdated) {
		glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);
		_projUpdated = false;
	}

	glUniform4fv(_uiColor, 1, _Color);
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		glUniform4fv(_uiLightInView[i], 1, _vLightInView[i]);
		glUniform4fv(_uiAmbient[i], 1, _ambientProduct[i]);
		glUniform4fv(_uiDiffuse[i], 1, _diffuseProduct[i]);
		glUniform4fv(_uiSpecular[i], 1, _specularProduct[i]);
		glUniform1i(_uiLighting[i], _iLighting[i]);
		glUniform1f(_uicutoff[i], _cutoff[i]);
	}
	
	
	glUniform1i(m_uiTexLayer, m_iTexLayer);
	glUniform1f(_uiShininess, _material.shininess);
	
}


//�ƻsvxShader�BfsShader�W��
void CShape::setShaderName(const char vxShader[], const char fsShader[])
{
	int len;
	len = strlen(vxShader);
	_pVXshader = new char[len + 1];
	memcpy(_pVXshader,vxShader,len+1);

	len = strlen(fsShader);
	_pFShader = new char[len + 1];
	memcpy(_pFShader, fsShader, len + 1);

}

void CShape::SetTiling(float uTiling, float vTiling)  // �� U�b �P V�b �i����K���Y��
{
	for (int i = 0; i < _iNumVtx; i++) {
		_pTex[i].x *= uTiling; _pTex[i].y *= vTiling;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4)) * _iNumVtx, sizeof(vec2) * _iNumVtx, _pTex); // vertcies' Color
}

void CShape::setViewMatrix(mat4& mat) {
	_mxView = mat;
	_viewUpdated = true;
}

void CShape::setTRSMatrix(mat4& mat) {
	_mxTRS = mat;
	_TRSUpdated = true;
}

void CShape::setProjectionMatrix(mat4& mat) {
	_mxProjection = mat;
	_projUpdated = true;

}

void CShape::setColor(vec4 vColor) {
	_Color[0] = vColor.x;
	_Color[1] = vColor.y;
	_Color[2] = vColor.z;
	_Color[3] = vColor.w;
//	glUniform4fv(_uiColor, 1, _Color);
}

void CShape::SetTextureLayer(int texlayer)
{
	m_iTexLayer = texlayer;
}

void CShape::setMaterials(color4 ambient, color4 diffuse, color4 specular) {

	_material.ambient = ambient;
	_material.diffuse = diffuse;
	_material.specular = specular;

}

void CShape::setKaKdKsShini(float ka, float kd, float ks, float shininess) // ka kd ks shininess
{
	_material.ka = ka;
	_material.kd = kd;
	_material.ks = ks;
	_material.shininess = shininess;
}


vec4 CShape::phongReflectionModel(vec4 vPoint, vec3 vNormal, vec4 vLightPos, color4 vLightI) {

	vec3 vN;
	vec4 vColor;
	vec4 vPosInView , vLightInView;

	// 1.�N�I��������Y�y�Шt��
	vPosInView = _mxMVFinal * vPoint;

	// 2.�N���� Normal ������Y�y�Шt�ΡA���ন���V�q ���[��S�[�@��
	//#ifdef GENERAL_CASE 
	//	//	  m_mxITMV �w�g�b�I�s SetModelViewMatrix(mat4 &mat) �P SetTRSMatrix(mat4 &mat) �N�P�ɭp��
	//	//    ���B�N���ΦA���s�p��
	//	vN = m_mxITMV * vNormal;
	//#else
	//vN = m_mxMV3X3Final * vNormal;
	//#endif

	//vN = normalize(vN);

	// 3. �p�� Ambient color :  Ia = Ka * Material.ambient * La
	vColor = _material.ka * _material.ambient * vLightI;

	// 4. �p�� Light �b���Y�y�Шt�Ϊ���m
//    ���B���] Light ����m�w�g�b�@�ɮy�Ъ������m
	vLightInView = _mxView * vLightPos;

	vec3 vLight; // �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	vLight.x = vLightInView.x - vPosInView.x;
	vLight.y = vLightInView.y - vPosInView.y;
	vLight.z = vLightInView.z - vPosInView.z;
	vLight = normalize(vLight); // normalize light vector

	// 5. �p�� L dot N
	GLfloat fLdotN = vLight.x * vN.x + vLight.y * vN.y + vLight.z * vN.z; //�O���P���~�k�V�q���n
	if (fLdotN > 0) { // ���I�Q�����Ө�~�ݭn�p��
		// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
		vColor += _material.kd * _material.diffuse * vLightI * fLdotN;

		// Specular color
		// Method 1: Phone Model
		//   �p�� View Vector
		vec3 vView;
		vView.x = 0 - vPosInView.x;  // �ثe�w�g�H���Y�y�Ь���¦, �ҥH View ����m�N�O (0,0,0)
		vView.y = 0 - vPosInView.y;
		vView.z = 0 - vPosInView.z;
		vView = normalize(vView);

		//	 �p�� Light �� �Ϯg�� vRefL
		vec3 vRefL = 2.0f * (fLdotN)*vN - vLight;

		vRefL = normalize(vRefL);
		//   �p��  vReflectedL dot View
		GLfloat RdotV = vRefL.x * vView.x + vRefL.y * vView.y + vRefL.z * vView.z;

		// Specular Color : Is = Ks * Ls * (R dot V)^Shininess;
		if (RdotV > 0) vColor += _material.ks * _material.specular * vLightI * powf(RdotV, _material.shininess);
	}

	vColor.w = 1; // Alpha ��]�w�� 1�A�w�]���O���z������
	// Method 2: Modified Phone Model 

	// �N�C���x�s��  m_Colors ���A�]�� Quad �O��Ӧ@�������T�����Һc��, �ҥH�]�w��ӤT���������ۦP���C��
	// �]�N�O�]�w�Ҧ������I���O�o���C��
	return vColor;


}


vec4 CShape::phongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource& Lights)
{
	vec3 vN, vLDir;		// �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	vec4 vColor;	// �ӳ��I���C��
	vec4 vPosInView, vLightInView;


	// 1. �N�I��������Y�y�Шt�ΡA�]�����I�|�ھ� TRS Matrix ���ʡA�ҥH�����H m_mxMVFinal �p��b�@�ɮy�Ъ������m
	vPosInView = _mxMVFinal * vPoint;

	// 2. �N���� Normal ������Y�y�Шt�ΡA���ন���V�q
	//    �u������ rotation �|���� Normal ����V�A���W���� Final ModelView Matrix�A�P���ഫ�����Y�y��
	//    m_mxMV3X3Final �b Update �B���� m_mxMVFinal �����W�� 3X3 ����, ���ݭn�첾�q
#ifdef GENERAL_CASE
	//	  m_mxITMV �w�g�b�I�s SetModelViewMatrix(mat4 &mat) �P SetTRSMatrix(mat4 &mat) �N�P�ɭp��
	//    ���B�N���ΦA���s�p��
	vN = m_mxITMV * vNormal;
#else
	vN = _mxMV3x3Final * vNormal;
#endif

	vN = normalize(vN);

	// 3. �p�� Ambient color :  Ia = Ka * Material.ambient * La
	vColor = _material.ka * _material.ambient * Lights.diffuse;

	// 4. �p�� Light �b���Y�y�Шt�Ϊ���m
	//    ���B���] Light ����m�w�g�b�@�ɮy�Ъ������m
	vLightInView = _mxView * Lights.position;

	vec3 vLight; // �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	vLight.x = vLightInView.x - vPosInView.x;
	vLight.y = vLightInView.y - vPosInView.y;
	vLight.z = vLightInView.z - vPosInView.z;
	vLight = normalize(vLight); // normalize light vector

	vec3 vLightDir = _mxMV3x3Final * Lights.spotDirection;

	// 5. �p��ө����I�O�_���b spotCutoff ����
	GLfloat fLdotLDir = -(vLight.x * vLightDir.x + vLight.y * vLightDir.y + vLight.z * vLightDir.z); //�h�@�t���]����V�q�����P��V�A�]���䧨���j��90��

	if (fLdotLDir >= Lights.spotCosCutoff) //�ݦ��spotlight �d��
	{
		// 5. �p�� L dot N
		GLfloat fLdotN = vLight.x * vN.x + vLight.y * vN.y + vLight.z * vN.z;
		color4 fLightI = Lights.diffuse * powf(fLdotLDir, Lights.spotExponent);

		// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
		vColor += _material.kd * _material.diffuse * fLightI * fLdotN;

		// Specular color
		// Method 1: Phone Model
		//   �p�� View Vector
		vec3 vView;
		vView.x = 0 - vPosInView.x;  // �ثe�w�g�H���Y�y�Ь���¦, �ҥH View ����m�N�O (0,0,0)
		vView.y = 0 - vPosInView.y;
		vView.z = 0 - vPosInView.z;
		vView = normalize(vView);

		//	 �p�� Light �� �Ϯg�� vRefL
		vec3 vRefL = 2.0f * (fLdotN)*vN - vLight;

		vRefL = normalize(vRefL);
		//   �p��  vReflectedL dot View
		GLfloat RdotV = vRefL.x * vView.x + vRefL.y * vView.y + vRefL.z * vView.z;

		// Specular Color : Is = Ks * Ls * (R dot V)^Shininess;
		if (RdotV > 0) vColor += _material.ks * _material.specular * fLightI * powf(RdotV, _material.shininess);
	}

	vColor.w = 1; // Alpha ��]�w�� 1�A�w�]���O���z������
	// Method 2: Modified Phone Model 

	// �N�C���x�s��  m_Colors ���A�]�� Quad �O��Ӧ@�������T�����Һc��, �ҥH�]�w��ӤT���������ۦP���C��
	// �]�N�O�]�w�Ҧ������I���O�o���C��
	return vColor;
}