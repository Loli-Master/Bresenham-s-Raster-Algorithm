#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "LOpenGL.h"

class LTexture {
  public:
	LTexture();

	~LTexture();

	bool loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height );

	void freeTexture();

	void render( GLfloat x, GLfloat y );

	GLuint getTextureID();

	GLuint textureWidth();

	GLuint textureHeight();

  private:
	//Texture name
	GLuint mTextureID;

	//Texture dimensions
	GLuint mTextureWidth;
	GLuint mTextureHeight;
};

#endif
