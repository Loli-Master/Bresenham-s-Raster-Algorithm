#include "LUtil.h"

void runMainLoop( int val );

int main( int argc, char* args[] ) {
	//inisialisasi FreeGLUT
	glutInit( &argc, args );

	//set versi openGL ke 2.1
	glutInitContextVersion( 2, 1 );

	glutInitDisplayMode( GLUT_DOUBLE );
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutCreateWindow( "OpenGL" );

	//ngeeeng
	if( !initGL() ) {
		printf( "Unable to initialize graphics library!\n" );
		return 1;
	}

	//Load media
	if( !loadMedia() ) {
		printf( "Unable to load media!\n" );
		return 2;
	}

	//Set beberapa fungsi callback
	glutDisplayFunc( render );
	glutMouseFunc(pressedMouse);
	glutKeyboardFunc(pressedKey);
	resetPlot();

	//Set timer buat 60FPS (ini bukan game sih :v)
	glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );

	//mulai GLUT main loop
	glutMainLoop();

	return 0;
}

void runMainLoop( int val ) {
	//ngeeng
	update();
	render();

	//timer lagi...
	glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}
