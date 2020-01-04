#include "C2DBTN.h"

C2DBTN::C2DBTN()
{
	_pTex = NULL;//防止空間收回時沒空間可收的問題。

	_iNumVtx = POINT_BTN_NUM;
	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];

	_pPoints[0] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	_pPoints[1] = vec4(0.5f, -0.5f, 0.0f, 1.0f);
	_pPoints[2] = vec4(0.5f, 0.5f, 0.0f, 1.0f);
	_pPoints[3] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	_pPoints[4] = vec4(0.5f, 0.5f, 0.0f, 1.0f);
	_pPoints[5] = vec4(-0.5f, 0.5f, 0.0f, 1.0f);
	

	_pNormals[0] = vec3(0, 0, 1.0f);
	_pNormals[1] = vec3(0, 0, 1.0f);
	_pNormals[2] = vec3(0, 0, 1.0f);
	_pNormals[3] = vec3(0, 0, 1.0f);
	_pNormals[4] = vec3(0, 0, 1.0f);
	_pNormals[5] = vec3(0, 0, 1.0f);

	setShaderName("vsVtxNormal.glsl" , "fsVtxNormal.glsl");

	createBufferObject();

	// 預設為 -1.0f, 由 Normal 決定顏色
	_pColors[0] = -1.0f; _pColors[1] = -1.0f; _pColors[2] = -1.0f; _pColors[3] = 1.0f;
	
	// Sprite 的左下與右上角的 local 座標 , button 的範圍
	_originalLL = vec4(-0.4f, -0.4f, 0.0f, 1.0f);
	_originalTR = vec4(0.4f, 0.4f, 0.0f, 1.0f);

	
	_bPushed = false;


}

C2DBTN::~C2DBTN()
{
	
}
void C2DBTN::update(float dt){

}


void C2DBTN::setTRSMatrix(mat4 &mat){

	_mxTRS = mat;
	_TRSUpdated = true;
	_LL = _mxTRS * _originalLL;
	_TR = _mxTRS * _originalTR;

}


bool C2DBTN::onTouch(const vec2 &tp){
	if (_LL.x <= tp.x && tp.x <= _TR.x && _LL.y <= tp.y && tp.y <= _TR.y) {

		if (!_bPushed) {
			setColor(vec4(0.5, 0.5, 0.5, 1.0));
			_bPushed = true;
		}
		else
		{
			_bPushed = false;
			setColor(_defaultColor);
		}
		return true; //回傳有點到按鈕

	}
	else return false; //回傳沒有點到按鈕

}


void C2DBTN::setDefaultColor(vec4 vColor){
	_defaultColor = vColor;
	setColor(_defaultColor);
}



void C2DBTN::draw(){
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_BTN_NUM);
	
}


void C2DBTN::drawW(){
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, POINT_BTN_NUM);
}



