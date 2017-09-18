#include "LTexture.h"

LTexture::LTexture() {
	//inisialisasi texture ID
	mTextureID = 0;

	//set ukuran
	mTextureWidth = 0;
	mTextureHeight = 0;
}

LTexture::~LTexture() {
	//hapus aja biar aman
	freeTexture();
}

bool LTexture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height ) {
	//hapus klo udah ada
	freeTexture();

	//set ukuran
	mTextureWidth = width;
	mTextureHeight = height;

	//pasang tuh idnya
	glGenTextures( 1, &mTextureID );

	//nah ini di bind (kek bind di valve game :v)
	glBindTexture( GL_TEXTURE_2D, mTextureID );

	//pasang texturenya
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	//yayaya settingan gaje
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	//Unbind (kek valve game lagi :v)
	glBindTexture( GL_TEXTURE_2D, NULL );

	//cek error
	GLenum error = glGetError();
	if( error != GL_NO_ERROR ) {
		printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
		return false;
	}

	return true;
}

void LTexture::freeTexture() {
	//hapus texture
	if( mTextureID != 0 ) {
		glDeleteTextures( 1, &mTextureID );
		mTextureID = 0;
	}

	mTextureWidth = 0;
	mTextureHeight = 0;
}

void LTexture::render( GLfloat x, GLfloat y ) {
	//kalo ada texture
	if( mTextureID != 0 ) {
		//wajib tau
		glLoadIdentity();

		//udah jelas
		glTranslatef( x, y, 0.f );

		//ya ampun
		glBindTexture( GL_TEXTURE_2D, mTextureID );

		//wajib tau klo pake OpenGL
		glBegin( GL_QUADS );
		glTexCoord2f( 0.f, 0.f );
		glVertex2f(           0.f,            0.f );
		glTexCoord2f( 1.f, 0.f );
		glVertex2f( mTextureWidth,            0.f );
		glTexCoord2f( 1.f, 1.f );
		glVertex2f( mTextureWidth, mTextureHeight );
		glTexCoord2f( 0.f, 1.f );
		glVertex2f(           0.f, mTextureHeight );
		glEnd();
	}
}

GLuint LTexture::getTextureID() {
	return mTextureID;
}

GLuint LTexture::textureWidth() {
	return mTextureWidth;
}

GLuint LTexture::textureHeight() {
	return mTextureHeight;
}
