#include"CSmoothQuad.h"

CSmoothQuad::~CSmoothQuad()
{
	delete[] _pSquares;
	delete[] _pfSquaresT;
}

CSmoothQuad::CSmoothQuad(int iSize, float fYPos)
{
	_iGridSize = iSize;
	
	_pSquares = new CQuad[_iGridSize * _iGridSize];
	_pfSquaresT = new float[_iGridSize * _iGridSize * 3];
	_fYPos = fYPos;



}

//設定每一格的渲染方式
void CSmoothQuad::setShadingMode(int iMode) {
	int idx = 0;
	for (int i = 0; i < _iGridSize; i++)
	{
		for (int j = 0; j < _iGridSize; j++)
		{
			_pSquares[idx].setShadingMode(iMode);
			idx++;
		}
	}

}


void CSmoothQuad::setShader() {

	mat4 mxT;
	int idx = 0;
	vec4 vT, vColor;
	for (int i = 0; i < _iGridSize; i++)
	{
		for (int j = 0; j < _iGridSize; j++)
		{
			_pfSquaresT[idx * 3 + 2] = -_iGridSize / 2 + i + 0.5f;
			_pfSquaresT[idx * 3 + 0] = -_iGridSize / 2 + j + 0.5f;
			_pfSquaresT[idx * 3 + 1] = _fYPos;
			
			if (i + j == 0) { // 第一個， 需要指定 Shader
				_pSquares[idx].setShader();
				_shaderHandle = _pSquares[idx].getShaderHandle();
				
			}
			else { // 其他的 
				
				_pSquares[idx].setShader(_shaderHandle);
				
			}
			mxT = Translate(_pfSquaresT[idx * 3 + 0], _pfSquaresT[idx * 3 + 1], _pfSquaresT[idx * 3 + 2]);
			_pSquares[idx].setTRSMatrix(mxT);
			idx++;
		}

	}


}


void CSmoothQuad::setProjectionMatrix(mat4& mat) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setProjectionMatrix(mat);
	}
}
void CSmoothQuad::setTRSMatrix(mat4& mat) {
	mat4 mxT;
	// 必須加上每一個方格本身的位移
	for (int i = 0; i < _iGridSize* _iGridSize; i++)
	{
		mxT = Translate(_pfSquaresT[i * 3 + 0], _pfSquaresT[i * 3 + 1], _pfSquaresT[i * 3 + 2]);
		
		_pSquares[i].setTRSMatrix(mat * mxT);
	}
}

void CSmoothQuad::draw() {
	
	_pSquares[0].draw();
	for (int i = 1; i < _iGridSize * _iGridSize; i++) {
		
		_pSquares[i].drawW();
		
	}
}

void CSmoothQuad::setViewMatrix(mat4 &mat) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setViewMatrix(mat);
	}
}

void CSmoothQuad::update(float dt, point4 vLightPos, color4 vLightI) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].update(dt, vLightPos, vLightI);
	}
}
void CSmoothQuad::update(float dt, const LightSource& Lights) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].update(dt, Lights);
	}
}


void CSmoothQuad::update(float dt, const LightSource* Lights) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].update(dt, Lights);
	}

}

void CSmoothQuad::setMaterials(color4 ambient, color4 diffuse, color4 specular) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setMaterials(ambient, diffuse, specular);
	}
}
void CSmoothQuad::setKaKdKsShini(float ka, float kd, float ks, float shininess) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setKaKdKsShini(ka, kd, ks, shininess);
	}
}


//設定法向量的值
void CSmoothQuad::setNormal(vec3 vNormal) {
	for (int i = 0; i < _iGridSize * _iGridSize; i++) {
		_pSquares[i].setNormal(vNormal);
		
	}
}