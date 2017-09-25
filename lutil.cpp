#include "lutil.h"
#include "ltexture.h"

//Checkerboard texture
LTexture gCheckerBoardTexture;

//yep, ini canvas yg kita pake
//false = kosong
//true = ada isi
bool plotted[300][140];
//biar gak update testure per frame
bool updt=false;
//lined circle toggle
bool lined=false;
//poly mode
bool polymodet = false;
//started poly
bool polystarted = false;

//leftclick down pos
int presx,presy;
//leftclick up pos
int relx,rely;
//rightclick down pos
int presxr,presyr;
//rightclick up pos
int relxr,relyr;
//dist from down to up
int cdist;

//first pos of poly
int fpolx, fpoly;

//last pos of poly
int lpolx, lpoly;

//fungsi sgn (keknya ada di kalkulus I deh)
int sgn(int a) {
	return (int(0<a)-int(a<0));
}

//cek posisi x ama y ada di canvas g
bool valid(int x, int y) {
	return (x>=0 && x<256 && y>=0 && y<128);
}

//hitung jarak euclid
int dist(int x1, int y1, int x2, int y2) {
	return int(ceil(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))));
}

//yep, kosongin canvas
void resetPlot() {
	system("CLS");
	memset(plotted, false, sizeof(plotted));
	updt=true;
}

//ini dia, Bresenham's Line Algorithm
//kalo mau detilnya, baca wiki ya :v
//ato di sini: http://www.uobabylon.edu.iq/eprints/publication_2_22893_6215.pdf
//kodenya implement langsung dari site di atas :v
void bresenAlg(int x1, int y1, int x2, int y2) {
	int x=x1;
	int y=y1;
	int dx=abs(x2-x1);
	int dy=abs(y2-y1);
	int s1=sgn(x2-x1);
	int s2=sgn(y2-y1);
	bool swp;
	if (dy>dx) {
		std::swap(dx,dy);
		swp=true;
	} else {
		swp=false;
	}
	int e=2*dy-dx;
	int a=2*dy;
	int b=2*dy-2*dx;
	plotted[x][y]=true;
	for(int i=1; i<=dx; i++) {
		if(e<0) {
			if(swp)y+=s2;
			else x+=s1;
			e+=a;
		} else {
			y+=s2;
			x+=s1;
			e+=b;
		}
		plotted[x][y]=true;
	}
}

//karena algo bresenham yg lingkaran sebenernya cuma untuk 1 octal aja
//tapi karena hitungannya sama juga untuk 7 octal yg lain
//jadi langsung 8 8 nya dibuat :v
void drawCirc(int xc, int yc, int x, int y) {
	if(valid(xc+x,yc+y))
		plotted[xc+x][yc+y]=true;
	if(valid(xc-x,yc+y))
		plotted[xc-x][yc+y]=true;
	if(valid(xc+x,yc-y))
		plotted[xc+x][yc-y]=true;
	if(valid(xc-x,yc-y))
		plotted[xc-x][yc-y]=true;
	if(valid(xc+y,yc+x))
		plotted[xc+y][yc+x]=true;
	if(valid(xc-y,yc+x))
		plotted[xc-y][yc+x]=true;
	if(valid(xc+y,yc-x))
		plotted[xc+y][yc-x]=true;
	if(valid(xc-y,yc-x))
		plotted[xc-y][yc-x]=true;
	//updt=true;
}

// yep akhirnya
// Bresenham's Circle Algorithm
// seperti biasa, detil di wiki
// ato di sini: http://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
// kodingannya copas poll :v
void bresenCirc(int xc, int yc, int r) {
	int x = 0, y = r;
	int d = 3 - 2 * r;
	while (y >= x) {

		drawCirc(xc, yc, x, y);
		x++;

		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		} else
			d = d + 4 * x + 6;
		drawCirc(xc, yc, x, y);
	}
}

//polymode continous draw line until right click
void polymode(int x, int y, bool cls) {
	if(cls&&polystarted) {
		bresenAlg(lpolx,lpoly,fpolx,fpoly);
		polystarted=false;
	} else {
		if(!polystarted) {
			fpolx=x;
			fpoly=y;
			lpolx=x;
			lpoly=y;
			polystarted = true;
		} else {
			bresenAlg(lpolx,lpoly,x,y);
			lpolx=x;
			lpoly=y;
		}
	}
}

//initGL ya inisialisasi OpenGL, emangnya ngapain lagi? :v
bool initGL() {
	//Set viewport
	glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

	//Inisialisasi Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0, 1.0, -1.0 );

	//Inisialisasi Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Inisialisasi warna background
	glClearColor( 0.f, 0.f, 0.f, 1.f );

	//udah jelas
	glEnable( GL_TEXTURE_2D );

	//ngeeeng
	GLenum error = glGetError();
	if( error != GL_NO_ERROR ) {
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		return false;
	}

	return true;
}

bool loadMedia() {
	//tekstur cuma bisa array linear 2^n
	const int CHECKERBOARD_WIDTH = 512;
	const int CHECKERBOARD_HEIGHT = 256;
	const int CHECKERBOARD_PIXEL_COUNT = CHECKERBOARD_WIDTH * CHECKERBOARD_HEIGHT;
	GLuint checkerBoard[ CHECKERBOARD_PIXEL_COUNT ];

	//yep iterasiin satu-satu
	for( int i = 0; i < CHECKERBOARD_PIXEL_COUNT; ++i ) {
		//ini byte yaaaa
		GLubyte* colors = (GLubyte*)&checkerBoard[ i ];
		//nebak posisi baris ama kolom di canvas
		//btw satu pixel di canvas = 2*2 pixel di texture, soalnya 1*1 ga keliatan :v
		int row = i/1024;
		int col = (i%512)/2;
		//bikin belang-belang, klo ada daerah yg dipake, jadiin item
		if( ((row+col)%2==1) && !plotted[col][row] ) {
			//ini putih
			colors[ 0 ] = 0xFF;
			colors[ 1 ] = 0xFF;
			colors[ 2 ] = 0xFF;
			colors[ 3 ] = 0xFF;
		} else if(!plotted[col][row]) {
			//ini abu-abu
			colors[ 0 ] = 0xEF;
			colors[ 1 ] = 0xEF;
			colors[ 2 ] = 0xEF;
			colors[ 3 ] = 0xFF;
		} else {
			//ini hitam
			colors[ 0 ] = 0x00;
			colors[ 1 ] = 0x00;
			colors[ 2 ] = 0x00;
			colors[ 3 ] = 0xFF;
		}
	}

	//Load texture
	if( !gCheckerBoardTexture.loadTextureFromPixels32( checkerBoard, CHECKERBOARD_WIDTH, CHECKERBOARD_HEIGHT ) ) {
		printf( "Unable to load checkerboard texture!\n" );
		return false;
	}

	return true;
}

void update() {
	//yep updatenya 1x aja, abis update langsung set biar ga update lagi
	if(updt) {
		loadMedia();
		updt=false;
	}
}

void render() {
	//hapus color buffer, kalo nggak katanya error (katanya)
	glClear( GL_COLOR_BUFFER_BIT );

	GLfloat x = 0.0f;
	GLfloat y = 0.0f;

	//render
	gCheckerBoardTexture.render( x, y );

	//ya buffernya ada 2, klo udah gantian
	glutSwapBuffers();
}

//simpelnya FreeGLUT, semua yg ada di window pake callback :v
//ini kalo ada tombol mouse yg ditekan
void pressedMouse(int button, int state, int x, int y) {
	//nah 1*1 pixel di canvas = 4*4 pixel di window
	//1*1 pixel di canvas = 2*2 pixel di texture
	//jadi 1*1 pixel di texture = 2*2 pixel di window
	//gak keder kan :v
	int row=y/4;
	int col=x/4;

	//ya ini biar nggak keluar canvas, tapi caranya jelek sih :v
	row = std::min(127,row);
	col = std::min(255,col);
	row = std::max(0,row);
	col = std::max(0,col);

	//yep, tombol dipencet ada 2 state
	//down klo masih diteken
	//up klo dah selesai diteken
	if(state==GLUT_DOWN) {
		if(button==GLUT_LEFT_BUTTON&& !polymodet) {
			presx = col;
			presy = row;
			printf("line start at x:%d y:%d\n",col,row);
		} else if(button==GLUT_RIGHT_BUTTON&& !polymodet) {
			presxr = col;
			presyr = row;
			printf("circle center at x:%d y:%d\n",col,row);
		} else if(button==GLUT_LEFT_BUTTON&& polymodet) {
			polymode(col,row);
		} else if(button==GLUT_RIGHT_BUTTON&& polymodet) {
			polymode(col,row,true);
		}
	} else if(state==GLUT_UP) {
		if(button==GLUT_LEFT_BUTTON&& !polymodet) {
			relx = col;
			rely = row;
			printf("line end at x:%d y:%d\n",col,row);
			bresenAlg(presx,presy,relx,rely);
			printf("created a line from x1:%d y1:%d to x2:%d y2:%d\n",presx,presy,relx,rely);
		} else if(button==GLUT_RIGHT_BUTTON&& !polymodet) {
			relxr = col;
			relyr = row;
			cdist = dist(presxr,presyr,relxr,relyr);
			printf("circle edge x:%d y:%d\n",col,row);
			bresenCirc(presxr,presyr,cdist);
			if(lined)
				bresenAlg(presxr,presyr,relxr,relyr);
			printf("created a circle from x:%d y:%d with radius of %d\n",presx,presy,cdist);
		}
		//set biar diupdate
		updt=true;
	}
}

//simpan gambar :v
void imgSave(int width,int height) {
	//array byte (1 buat masing2 R, G, B)
	BYTE* pixels = new BYTE[3 * width * height];

	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// konversi ke freeimage format, trus disave
	FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
	FreeImage_Save(FIF_BMP, image, "save.bmp", 0);
	printf("Image Saved!\n");

	//biar hemat memori katanya :v
	FreeImage_Unload(image);
	delete [] pixels;
}

//ini callback buat klo ada tombol keyboard dipencet
void pressedKey(unsigned char key, int x, int y) {
	if (key=='r'||key=='R') {
		resetPlot();
	} else if (key=='q'||key=='Q') {
		glutLeaveMainLoop();
		exit(0);
	} else if (key=='s'||key=='S') {
		imgSave(SCREEN_WIDTH,SCREEN_HEIGHT);
	} else if (key=='t'||key=='T') {
		if(lined) {
			lined=false;
			printf("Circle now without line\n");
		} else {
			lined=true;
			printf("Circle now with line\n");
		}
	} else if (key=='p'||key=='P') {
		if(polymodet) {
			if(polystarted){
				polymode(lpolx,lpoly,true);
				updt=true;
			}
			polymodet=false;
			//printf("Circle now without line\n");
		} else {
			polymodet=true;
			//printf("Circle now with line\n");
		}
	}
}
