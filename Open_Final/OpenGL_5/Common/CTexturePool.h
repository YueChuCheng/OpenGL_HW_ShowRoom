#ifndef CTEXTUREPOOL_H
#define CTEXTUREPOOL_H

#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr;  } while(0) //刪除指向的空間，且設指向零空間
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

#include "../header/Angel.h"

#define	TEXTUREPOOL_MAX 100  //預計最多放100張

typedef struct structTextureElement {  //記錄每張貼圖的基本資料
	GLuint  uiTextureID; //texture ID 
	char* TextureName; //texture 的名稱 
	int		iTexWidth;
	int		iTexHeight;
} TextureElement;


//提供貼圖方便控管與使用
//採用singleton pattern 的方式管理
class CTexturePool
{
	int _iNumTextures;
	TextureElement _Pool[TEXTUREPOOL_MAX];		// 預設能儲存 100 張貼圖
public:
	static CTexturePool* create();		// 建立 CTexturePool 的實體
	static CTexturePool* getInstance();	// 取得  CTexturePool 的實體

	void destroyInstance();		// 釋放取得資源
	GLuint AddTexture(char* texName);
	GLuint GetTextureID(char* texName) {}; // 沒有實作 //可以根據使用者所給的貼圖名稱，去查詢該貼圖是否已經讀入，如果讀入則傳回texture ID

protected:
	CTexturePool();			// 避免被誤用
	~CTexturePool();
	void initDefault() {};
	static CTexturePool* _pTexturePool;
};

#endif

