#include "CSolidSphere.h"

CSolidSphere::CSolidSphere(const GLfloat fRadius, const int iSlices, const  int iStacks)
{
	GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat)iStacks;
	GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat)iSlices;
	GLfloat ds = 1.0f / (GLfloat)iSlices;
	GLfloat dt = 1.0f / (GLfloat)iStacks;
	GLfloat t = 1.0f;
	GLfloat s = 0.0f;
	GLint i, j;     // Looping variables  
	int idx = 0; // 儲存 vertex 順序的索引值

	m_fRadius = fRadius;
	m_iSlices = iSlices;
	m_iStacks = iStacks;
	_iNumVtx = iStacks * (2 * (iSlices + 1));

	_pPoints = NULL; _pNormals = NULL; _pTex = NULL;

	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];
	_pTex = new vec2[_iNumVtx];

#ifdef LIGHTMAP
	_pTex_light = new vec2[_iNumVtx];  // 產生 light map 所需的貼圖座標
#endif
#ifdef NORMALMAP
	_pTex_normal = new vec2[_iNumVtx];	// 產生 normal map 所需的貼圖座標
	_pTangent = new vec3[_iNumVtx];
#endif

	for (i = 0; i < iStacks; i++) {
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));

		// Many sources of OpenGL sphere drawing code uses a triangle fan  
		// for the caps of the sphere. This however introduces texturing   
		// artifacts at the poles on some OpenGL implementations  
		s = 0.0f;
		for (j = 0; j <= iSlices; j++) {
			GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));

			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;

			_pPoints[idx].x = x * m_fRadius;
			_pPoints[idx].y = y * m_fRadius;
			_pPoints[idx].z = z * m_fRadius;
			_pPoints[idx].w = 1;

			_pNormals[idx].x = x;
			_pNormals[idx].y = y;
			_pNormals[idx].z = z;

			_pTex[idx].x = s;
			_pTex[idx].y = t; // 設定貼圖座標
			idx++;

			x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;

			_pPoints[idx].x = x * m_fRadius;
			_pPoints[idx].y = y * m_fRadius;
			_pPoints[idx].z = z * m_fRadius;
			_pPoints[idx].w = 1;

			_pNormals[idx].x = x;
			_pNormals[idx].y = y;
			_pNormals[idx].z = z;

			_pTex[idx].x = s;
			_pTex[idx].y = t - dt; // 設定貼圖座標
			idx++;
			s += ds;
		}
		t -= dt;
	}

	// 預設將所有的面都設定成灰色
	for (int i = 0; i < _iNumVtx; i++) _pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f);

#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsPerVtxLighting.glsl", "fsPerVtxLighting.glsl");
#else
	setShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif 

	// Create and initialize a buffer object ，將此部分的設定移入 SetShader 中
	// CreateBufferObject();

	// 設定材質
	setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	setKaKdKsShini(0, 0.8f, 0.2f, 1);
}

// 回家自己寫
void CSolidSphere::RenderWithFlatShading(const LightSource& lights)
{


}

void CSolidSphere::RenderWithGouraudShading(const LightSource& lights)
{
	// Method 1 : 對每一個 Vertex 都計算顏色
	for (int i = 0; i < m_iStacks; i++) {
		for (int j = 0; j < 2 * (m_iSlices + 1); j++) {
			_pColors[i * 2 * (m_iSlices + 1) + j] = phongReflectionModel(_pPoints[i * 2 * (m_iSlices + 1) + j], _pNormals[i * 2 * (m_iSlices + 1) + j], lights);
		}
	}

	// Method 2 : 重疊的 Vertex 使用前一次計算的顏色
	 //先計算第一個 Stack 的顏色
	//for( int j = 0 ; j < 2*(m_iSlices+1) ; j++ ) {
	//	m_pColors[j] = PhongReflectionModel(m_pPoints[j], m_pNormals[j], vLightPos, vLightI);
	//}
	//// 後續 Stack 的 vertex 顏色，編號偶數(含 0) 使用前一個 stack 編號+1的 顏色
	//// 編號奇數就必須計算顏色
	//// 每一個 Slices 最後兩個 vertex 於開頭前兩個 vertex 重疊，所以使用該兩個 vertex 的顏色
	//for (int i = 1; i < m_iStacks; i++ ) {  
	//	for( int j = 0 ; j < 2*(m_iSlices+1) - 2 ; j++ ) {
	//		if( j%2 ) m_pColors[i*2*(m_iSlices+1)+j] = PhongReflectionModel(m_pPoints[i*2*(m_iSlices+1)+j], m_pNormals[i*2*(m_iSlices+1)+j], vLightPos, vLightI);
	//		else m_pColors[i*2*(m_iSlices+1)+j] =  m_pColors[(i-1)*2*(m_iSlices+1)+j+1];		
	//	}
	//	m_pColors[(i+1)*2*(m_iSlices+1)-2] = m_pColors[i*2*(m_iSlices+1)];
	//	m_pColors[(i+1)*2*(m_iSlices+1)-1] = m_pColors[i*2*(m_iSlices+1)+1];
	//}

	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _iNumVtx + sizeof(vec3) * _iNumVtx, sizeof(vec4) * _iNumVtx, _pColors); // vertcies' Color
}


void CSolidSphere::update(float dt, const LightSource& lights)
{
/*#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x, m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y, m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z, m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
#endif
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING)  RenderWithGouraudShading(lights);
	else RenderWithGouraudShading(lights);

#else // Lighting With GPU
	if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	_vLightInView = _mxView * lights.position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	m_AmbientProduct = m_Material.ka * m_Material.ambient * lights.ambient;
	m_AmbientProduct.w = m_Material.ambient.w;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse * lights.diffuse;
	m_DiffuseProduct.w = m_Material.diffuse.w;
	m_SpecularProduct = m_Material.ks * m_Material.specular * lights.specular;
	m_SpecularProduct.w = m_Material.specular.w;
#endif*/
}

void CSolidSphere::update(float dt)
{
	/*if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
		m_bViewUpdated = m_bTRSUpdated = false;
	}*/
}

void CSolidSphere::draw()
{
	drawingSetShader();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Change to wireframe mode
	for (int i = 0; i < m_iStacks; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * (2 * (m_iSlices + 1)), 2 * (m_iSlices + 1));
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Return to solid mode
}

void CSolidSphere::drawW()
{
	drawingWithoutSetShader();
	for (int i = 0; i < m_iStacks; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * (2 * (m_iSlices + 1)), 2 * (m_iSlices + 1));
	}
}

CSolidSphere::~CSolidSphere()
{

}

void CSolidSphere::update(float dt, const LightSource* Lights)
{
	if (_viewUpdated || _TRSUpdated) {
		_mxMVFinal = _mxView * _mxTRS;
		_viewUpdated = _TRSUpdated = false;
	}
	for (int i = 0; i < LIGHT_NUM_MAX; i++)
	{
		_vLightInView[i] = _mxView * Lights[i].position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_ambientProduct[i] = _material.ka * _material.ambient * Lights[i].ambient;
		_diffuseProduct[i] = _material.kd * _material.diffuse * Lights[i].diffuse;
		_specularProduct[i] = _material.ks * _material.specular * Lights[i].specular;
		_cutoff[i] = Lights[i].spotCutoff;

	}
}
