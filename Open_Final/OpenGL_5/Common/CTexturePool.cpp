#include "CTexturePool.h"
#include "../png_loader.h"

CTexturePool* CTexturePool::_pTexturePool = nullptr;

void CTexturePool::destroyInstance()
{
	if (_iNumTextures != 0) { // 這邊很重要!!，釋放所有貼圖的名稱與貼圖物件
		for (int i = 0; i < _iNumTextures; i++) {
			glDeleteTextures(1, &_Pool[i].uiTextureID); //移除所有貼圖
			delete _Pool[i].TextureName;
		}

	}
	CC_SAFE_DELETE(_pTexturePool);
}


CTexturePool::CTexturePool() {
	//變數初始化
	_iNumTextures = 0;
	for (int i = 0; i < TEXTUREPOOL_MAX; i++) {
		_Pool[i].TextureName = NULL;
		_Pool[i].uiTextureID = -1;
		_Pool[i].iTexHeight = _Pool[i].iTexWidth = 0;
	}
}

CTexturePool::~CTexturePool()
{

}

CTexturePool* CTexturePool::create()
{
	if (_pTexturePool == nullptr) {
		_pTexturePool = new (std::nothrow) CTexturePool;
	}
	return _pTexturePool;
}

CTexturePool* CTexturePool::getInstance()
{
	if (_pTexturePool == nullptr) {
		_pTexturePool = new (std::nothrow) CTexturePool;
	}
	return _pTexturePool;
}

GLuint CTexturePool::AddTexture(char* texName)
{
	// 先將 texName 複製一份到 _pool 中
	int len = strlen(texName);
	_Pool[_iNumTextures].TextureName = new char[len + 1]; //取得貼圖的名稱與儲存空間
	memcpy(_Pool[_iNumTextures].TextureName, texName, len); //複製字串內容
	_Pool[_iNumTextures].TextureName[len] = '\0';//於字串尾部加入結尾字元
	_Pool[_iNumTextures].uiTextureID = png_load_SOIL(texName, &(_Pool[_iNumTextures].iTexWidth), &(_Pool[_iNumTextures].iTexHeight), true);
	_iNumTextures++; //指向下一個存放空間
	return(_Pool[_iNumTextures - 1].uiTextureID); //回傳空間位址
}