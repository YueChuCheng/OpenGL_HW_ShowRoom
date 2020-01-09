#ifndef CTEXTUREPOOL_H
#define CTEXTUREPOOL_H

#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr;  } while(0) //�R�����V���Ŷ��A�B�]���V�s�Ŷ�
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

#include "../header/Angel.h"

#define	TEXTUREPOOL_MAX 100  //�w�p�̦h��100�i

typedef struct structTextureElement {  //�O���C�i�K�Ϫ��򥻸��
	GLuint  uiTextureID; //texture ID 
	char* TextureName; //texture ���W�� 
	int		iTexWidth;
	int		iTexHeight;
} TextureElement;


//���ѶK�Ϥ�K���޻P�ϥ�
//�ĥ�singleton pattern ���覡�޲z
class CTexturePool
{
	int _iNumTextures;
	TextureElement _Pool[TEXTUREPOOL_MAX];		// �w�]���x�s 100 �i�K��
public:
	static CTexturePool* create();		// �إ� CTexturePool ������
	static CTexturePool* getInstance();	// ���o  CTexturePool ������

	void destroyInstance();		// ������o�귽
	GLuint AddTexture(char* texName);
	GLuint GetTextureID(char* texName) {}; // �S����@ //�i�H�ھڨϥΪ̩ҵ����K�ϦW�١A�h�d�߸ӶK�ϬO�_�w�gŪ�J�A�p�GŪ�J�h�Ǧ^texture ID

protected:
	CTexturePool();			// �קK�Q�~��
	~CTexturePool();
	void initDefault() {};
	static CTexturePool* _pTexturePool;
};

#endif

