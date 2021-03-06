#define _CRT_SECURE_NO_WARNINGS 0

#include "header/Angel.h"
#include "SOIL/SOIL.h"

//--------------------------------------------------------------------------------------------
// png_load_SOIL uses SOIL to load png files
//
GLuint png_load_SOIL(const char* file_name, int* width, int* height, bool bMipMap)
{
	GLuint texture;
	GLint iwidth, iheight, ichannel, iformat, row_bytes;
	GLubyte* texArray, * imageData;
	texArray = SOIL_load_image(file_name, &iwidth, &iheight, &ichannel, SOIL_LOAD_AUTO); //儲存空間的起始位置 以及圖片的資料
	row_bytes = iwidth * ichannel;
	imageData = (GLubyte*)malloc(row_bytes * iheight * sizeof(GLubyte));//圖片翻轉後的存放位置

	// 對 texArray 執行垂直鏡射，放到 imageData 中
	for (int i = 0; i < iheight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(imageData + (row_bytes * i), texArray + (iheight - i - 1) * row_bytes, row_bytes);
	}
	switch (ichannel)
	{
	case 3:
		iformat = GL_RGB;
		break;
	case 4:
		iformat = GL_RGBA;
		break;
	default:
		fprintf(stderr, "%s: Unknown libpng with %d channel.\n", file_name, ichannel);
		return 0;
		break;
	}

	glGenTextures(1, &texture); //generate a set of new texture names.
	glBindTexture(GL_TEXTURE_2D, texture);// bind a named texture to a texturing target
	if (bMipMap) { // 開啟 MipMap 功能
		glTexImage2D(GL_TEXTURE_2D, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData); //define image as a texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//重複貼圖
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else { // 沒有使用 MipMap 功能
		glTexImage2D(GL_TEXTURE_2D, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //重複貼圖
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //GL_LINEAR
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //GL_LINEAR

	}
	glBindTexture(GL_TEXTURE_2D, 0); //取消目前所有byte，避免參數綁到其他texture object上
	*width = iwidth; *height = iheight; //存入圖片長、高

	// clean up
	free(texArray);
	free(imageData);
	return texture;
}
//--------------------------------------------------------------------------------------------

GLuint CubeMap_load_SOIL()
{
	GLuint texture;
	GLint iwidth, iheight, ichannel, iformat, row_bytes;
	GLubyte* texArray, * imageData;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	// 預設讀取 Sunny_NX Sunny_PX Sunny_NY Sunny_PY Sunny_NZ Sunny_PZ 六張影像, 都是 png
	// 每一張影像大小都相同, 所以 imageData 取得一次就行
	texArray = SOIL_load_image("texture/SkyBox/Skybox_PX.png", &iwidth, &iheight, &ichannel, SOIL_LOAD_AUTO);
	row_bytes = iwidth * ichannel;
	imageData = (GLubyte*)malloc(row_bytes * iheight * sizeof(GLubyte));
	// 對 texArray 執行垂直鏡射，放到 imageData 中
	for (int i = 0; i < iheight; i++) memcpy(imageData + (row_bytes * i), texArray + (iheight - i - 1) * row_bytes, row_bytes);
	iformat = GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData);
	free(texArray); // 釋放

	texArray = SOIL_load_image("texture/SkyBox/Skybox_NX.png", &iwidth, &iheight, &ichannel, SOIL_LOAD_AUTO);
	for (int i = 0; i < iheight; i++) memcpy(imageData + (row_bytes * i), texArray + (iheight - i - 1) * row_bytes, row_bytes);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData);
	free(texArray); // 釋放

	// 因為配合圖檔的垂直鏡射 所以 PY 與 NY 必須對調
	texArray = SOIL_load_image("texture/SkyBox/Skybox_NY.png", &iwidth, &iheight, &ichannel, SOIL_LOAD_AUTO);
	for (int i = 0; i < iheight; i++) memcpy(imageData + (row_bytes * i), texArray + (iheight - i - 1) * row_bytes, row_bytes);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData);
	free(texArray); // 釋放

	texArray = SOIL_load_image("texture/SkyBox/Skybox_PY.png", &iwidth, &iheight, &ichannel, SOIL_LOAD_AUTO);
	for (int i = 0; i < iheight; i++) memcpy(imageData + (row_bytes * i), texArray + (iheight - i - 1) * row_bytes, row_bytes);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData);
	free(texArray); // 釋放

	texArray = SOIL_load_image("texture/SkyBox/Skybox_PZ.png", &iwidth, &iheight, &ichannel, SOIL_LOAD_AUTO);
	for (int i = 0; i < iheight; i++) memcpy(imageData + (row_bytes * i), texArray + (iheight - i - 1) * row_bytes, row_bytes);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData);
	free(texArray); // 釋放

	texArray = SOIL_load_image("texture/SkyBox/Skybox_NZ.png", &iwidth, &iheight, &ichannel, SOIL_LOAD_AUTO);
	for (int i = 0; i < iheight; i++) memcpy(imageData + (row_bytes * i), texArray + (iheight - i - 1) * row_bytes, row_bytes);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, iformat, iwidth, iheight, 0, iformat, GL_UNSIGNED_BYTE, imageData);
	free(texArray); // 釋放

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //重複貼圖
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	free(imageData);
	return texture;
}

//--------------------------------------------------------------------------------------------