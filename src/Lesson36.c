/**************************************
*                                     *
*   Jeff Molofee's Basecode Example   *
*          nehe.gamedev.net           *
*                2001                 *
*                                     *
*    All Code / Tutorial Commenting   *
*       by Jeff Molofee ( NeHe )      *
*                                     *
***************************************


==========================================================================
          OpenGL Lesson 36:  Radial Blur & Rendering To A Texture
==========================================================================

	 Authors Name: Dario Corno ( rIo )

	 Modified for LCCWin32 compiler by Yann [ kohai/synrj ] Parmentier 2004/01/31

	 Notes from kohai : I striped down the new NeHe framework and its C++ syntax,
	 					as it wasn't compatible with LCC pure C syntax.
						I used good old NeHe initialization code instead,
						for maximum simplicity and reliability.

***************************************/

 /* **************************************************************
 *  Project: $(project)
 *  Function : Main program
 ***************************************************************
 *  $Author: Jeff Molofee 2000
 *  $Name:  $
 ***************************************************************
 *
 *  Copyright NeHe Production
 *
 ***************************************************************/

/**         Comments manageable by Doxygen
*
*  Modified smoothly by Thierry DECHAIZE
*
*  Paradigm : obtain one source (only one !) compatible for multiple free C Compilers
*    and provide for all users an development environment on Windows (64 bits compatible),
*    the great Code::Blocks manager (version 20.03), and don't use glaux.lib or glaux.dll.
*
*	a) Mingw 32 bits, version officielle gcc 9.2.0 (old !) : downloadable on http://sourceforge.net/projects/mingw/ (official)
*	b) Mingw 64 bits included in new IDE Red Panda Dev-Cpp, version gcc 10.3.0 : donwloadable on http://sourceforge.net/projects/dev-cpp-2020/
*	c) Mingw 64 bits included in package Code::Blocks (version 20.03), version gcc 8.1.0 (very old !) : downloadable on http://sourceforge.net/projects/codeblocks/files/Binaries/20.03/Windows/
*	d) Mingw 32 and 64 bits packagés, version gcc 11.2.0 : downloadable on  https://winlibs.com/ (and CLANG included in, 32 and 64 bits), two kits :
*			- winlibs-i686-posix-dwarf-gcc-12.2.0-llvm-14.0.6-mingw-w64ucrt-10.0.0-r2.7z (32 bits)
*			- winlibs-x86_64-posix-seh-gcc-12.2.0-llvm-14.0.6-mingw-w64ucrt-10.0.0-r2.7z (64 bits)
*	e) Cygwin64, 32 et 64 bits, version gcc 11.3.0 : downloadable on http://www.cygwin.com/install.html (tool for install : setup-x86_64.exe)
*	f) TDM GCC, 32 et 64 bits, version 10.3.0 : downloadable on http://sourceforge.net/projects/TDM-GCC
*	g) MSYS2 environnement MINGW32 and MINGW64, 32 et 64 bits, version de 2022 (msys2-x86_64-20220603.exe), version gcc 12.2.0 : downloadable on https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20220603.exe
*	h) Visual Studio 2022, 32 et 64 bits, community edition for free : downloadable on https://visualstudio.microsoft.com/fr/thank-you-downloading-visual-studio/?sku=Community&rel=17
*	i) Borland C/C++ 32 bits, version 5.5 : downloadable on https://developerinsider.co/download-and-install-borland-c-compiler-on-windows-10/
*	j) Digital Mars Compiler C/C++ 32 bits version 8.57 : downloadable on http://www.digitalmars.com (the more old compiler, the more bugged, dead branch !)
*	k) OpenWatcom C/C++ 32 et 64 bits, version 2.0 : downloadable on http://openwatcom.mirror.fr/ (only 32 bits version run correctly !)
*	l) Lcc and Lcc64, 32 et 64 bits: downloadable http://www.cs.virginia.edu/~lcc-win32/
*	m) PELLES C (only C) , 32 et 64 bits, version 11.0 : downloadable on http://www.smorgasbordet.com/pellesc/
*	o) CLANG, adossé aux environnements MINGW64 et MINGW32, version 14.0.6 (version gcc 12.0.0) : downloadable on https://winlibs.com/
*	p) CLANG, adossé aux environnements Visual Studio 2022 (+ kits Microsoft), version 15.0.1 : downloadable on https://releases.llvm.org/download.html
*	q) CLANG de la version MSYS2, adossé aux environnements MINGW64 et MINGW32, version 15.0.0 (version gcc 12.2.0) : downloadable on https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20220118.exe
*	r) CLANG de la version CYGWIN, adossé aux environnements MINGW64 et MINGW32, version 8.0.0 (very old !) (version gcc 11.3.0) : downloadable http://www.cygwin.com/install.html (tool for install or update : setup-x86_64.exe)
*
*   TDE -> Add resource file and resource header for restitute version + icon OpenGL.ico for fun
*			because versionning is important, same for freeware :-) !
*
*  Date : 2022/02/16
*
* \file            Lesson36.c
* \author          Dario Corno & Jeff Molofee (NeHe) originely, Modified for LCCWin32 compiler by Robert Wishlaw 2002/11/24
* \author          After adapted by Thierry Dechaize to verify paradigm : one source for multiple C Compilers
* \version         2.0.1.0
* \date            16 février 2022
* \brief           Ouvre une fenêtre Windows et dessine une spirale colorée avec l'utilisation de lumière "radial blur".
* \details         Ce programme ne gére que trois événements : le clic sur le bouton "Fermé" de la fenêtre, la sortie par la touche ESC et les flêches de déplacement clavier.
*
*/

#if defined __CYGWIN__ || defined __LCC__
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>		// Header File For Windows
#if defined(__LCC__)
#ifndef WINGDIAPI
#   define WINGDIAPI __stdcall
#endif
#endif

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glut.h>		// Header File For The GLut32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library
#include <math.h>

#if defined __GNUC__ || defined __LCC__ || defined _MSC_VER || defined __TURBOC__
#include <stdbool.h>
#endif

#if defined(__SC__) || defined(__DMC__)
#include <wtypes.h>
// typedef BOOLEAN         bool;
#endif

// A user defined ``bool'' type for compilers that don't yet support one.
//
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x500) || defined(__WATCOMC__)
  #define DONT_HAVE_BOOL_TYPE
#endif

// A ``bool'' type for compilers that don't yet support one.
#if defined(DONT_HAVE_BOOL_TYPE)
  typedef char bool;

  #ifdef true
    #warning Better check include file ``mytypes.h''.
    #undef true
  #endif
  #define true 1

  #ifdef false
    #warning Better check include file ``mytypes.h''.
    #undef false
  #endif
  #define false 0
#endif

#if defined __POCC__ || defined __LCC__ || defined __WATCOMC__  // because "rand" function declared in it ... !!!`
#include <stdlib.h>
#endif

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

BOOL	keys[256];			// Array Used For The Keyboard Routine
BOOL	active=TRUE;		// Window Active Flag Set To TRUE By Default
BOOL	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
int 	width = 640, height = 480;


// User Defined Variables
float		angle;												// Used To Rotate The Helix
float		vertexes[4][3];										// Holds Float Info For 4 Sets Of Vertices
float		normal[3];											// An Array To Store The Normal Data
GLuint		BlurTexture;										// An Unsigned Int To Store The Texture Number
int 		TextureSize = 8 ;									// Size of our RenderToTexture target

static	PIXELFORMATDESCRIPTOR pfd=				    // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		0,						    				// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
    };

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

/**	            This Code Create An Empty Texture
*
* \brief      Fonction EmptyTexture : création d'une texture vide
* \details    Rien
* \return     GLuint        un entier npon signé OpenGL                *
*
*/

GLuint EmptyTexture()											// Create An Empty Texture
{
	GLuint txtnumber;											// Texture ID
	unsigned int* data;											// Stored Data

	// Create Storage Space For Texture Data (128x128x4)
	data = (unsigned int*) malloc((TextureSize * TextureSize)* 4 * sizeof(unsigned int));
	ZeroMemory(data,((TextureSize * TextureSize)* 4 * sizeof(unsigned int)));	// Clear Storage Memory

	glGenTextures(1, &txtnumber);								// Create 1 Texture
	glBindTexture(GL_TEXTURE_2D, txtnumber);					// Bind The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 4, TextureSize, TextureSize, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, data);						// Build Texture Using Information In data
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	free ( data );												// Release data

	return txtnumber;											// Return The Texture ID
}

/**	            This Code Reduces A Normal Vector (3 Coordinates)
*
* \brief      Fonction ReduceToUnit : reduction d'un vecteur normal
* \details    Rien
* \param	  vector		un vecteur (tableau de trois valeurs)       *
* \return     void          un void (aucun retour)                      *
*
*/

void ReduceToUnit(float vector[3])								// Reduces A Normal Vector (3 Coordinates)
{																// To A Unit Normal Vector With A Length Of One.
	float length;												// Holds Unit Length
	// Calculates The Length Of The Vector
	length = (float)sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));

	if(length == 0.0f)											// Prevents Divide By 0 Error By Providing
		length = 1.0f;											// An Acceptable Value For Vectors To Close To 0.

	vector[0] /= length;										// Dividing Each Element By
	vector[1] /= length;										// The Length Results In A
	vector[2] /= length;										// Unit Normal Vector.
}

/**	            This Code Calculates Normal For A Quad Using 3 Points
*
* \brief      Fonction calcNormal : dcalcul la normale à partir de trois points fournis en matrice
* \details    Rien
* \param	  v			    un matrice 3x3 				                *
* \param	  out		    la normale calculée		                    *
* \return     void          un void (aucun retour)                      *
*
*/

void calcNormal(float v[3][3], float out[3])					// Calculates Normal For A Quad Using 3 Points
{
	float v1[3],v2[3];											// Vector 1 (x,y,z) & Vector 2 (x,y,z)
	static const int x = 0;										// Define X Coord
	static const int y = 1;										// Define Y Coord
	static const int z = 2;										// Define Z Coord

	// Finds The Vector Between 2 Points By Subtracting
	// The x,y,z Coordinates From One Point To Another.

	// Calculate The Vector From Point 1 To Point 0
	v1[x] = v[0][x] - v[1][x];									// Vector 1.x=Vertex[0].x-Vertex[1].x
	v1[y] = v[0][y] - v[1][y];									// Vector 1.y=Vertex[0].y-Vertex[1].y
	v1[z] = v[0][z] - v[1][z];									// Vector 1.z=Vertex[0].y-Vertex[1].z
	// Calculate The Vector From Point 2 To Point 1
	v2[x] = v[1][x] - v[2][x];									// Vector 2.x=Vertex[0].x-Vertex[1].x
	v2[y] = v[1][y] - v[2][y];									// Vector 2.y=Vertex[0].y-Vertex[1].y
	v2[z] = v[1][z] - v[2][z];									// Vector 2.z=Vertex[0].z-Vertex[1].z
	// Compute The Cross Product To Give Us A Surface Normal
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];							// Cross Product For Y - Z
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];							// Cross Product For X - Z
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];							// Cross Product For X - Y

	ReduceToUnit(out);											// Normalize The Vectors
}

/**	            This Code Draws A Helix.
*
* \brief      Fonction ProcessHelix : dessine une hélice
* \details    Rien
* \return     void          un void (aucun retour)                                  *
*
*/

void ProcessHelix()												// Draws A Helix
{
	GLfloat x;													// Helix x Coordinate
	GLfloat y;													// Helix y Coordinate
	GLfloat z;													// Helix z Coordinate
	GLfloat phi;												// Angle
	GLfloat theta;												// Angle
	GLfloat v,u;												// Angles
	GLfloat r;													// Radius Of Twist
	int twists = 5;												// 5 Twists
	GLfloat glfMaterialColor[4];			                    // The Material Color
	GLfloat specular[4];					                    // Specular Lighting

    glfMaterialColor[0]=0.4f;			                // Set The Material Color
    glfMaterialColor[1]=0.2f;
    glfMaterialColor[2]=0.8f;
    glfMaterialColor[3]=1.0f;
    specular[0]=1.0f;					                // Sets Up Specular Lighting
    specular[1]=1.0f;
    specular[2]=1.0f;
    specular[3]=1.0f;

	glLoadIdentity();											// Reset The Modelview Matrix
	gluLookAt(0, 5, 50, 0, 0, 0, 0, 1, 0);						// Eye Position (0,5,50) Center Of Scene (0,0,0), Up On Y Axis

	glPushMatrix();												// Push The Modelview Matrix

	glTranslatef(0,0,-50);										// Translate 50 Units Into The Screen
	glRotatef(angle/2.0f,1,0,0);								// Rotate By angle/2 On The X-Axis
	glRotatef(angle/3.0f,0,1,0);								// Rotate By angle/3 On The Y-Axis

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,glfMaterialColor);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);

	r=1.5f;														// Radius

	glBegin(GL_QUADS);											// Begin Drawing Quads
	for(phi=0; phi <= 360; phi+=20.0)							// 360 Degrees In Steps Of 20
	{
		for(theta=0; theta<=360*twists; theta+=20.0)			// 360 Degrees * Number Of Twists In Steps Of 20
		{
			v=(phi/180.0f*3.142f);								// Calculate Angle Of First Point	(  0 )
			u=(theta/180.0f*3.142f);							// Calculate Angle Of First Point	(  0 )

			x=(float)(cos(u)*(2.0f+cos(v) ))*r;					// Calculate x Position (1st Point)
			y=(float)(sin(u)*(2.0f+cos(v) ))*r;					// Calculate y Position (1st Point)
			z=(float)((( u-(2.0f*3.142f)) + sin(v) ) * r);		// Calculate z Position (1st Point)

			vertexes[0][0]=x;									// Set x Value Of First Vertex
			vertexes[0][1]=y;									// Set y Value Of First Vertex
			vertexes[0][2]=z;									// Set z Value Of First Vertex

			v=(phi/180.0f*3.142f);								// Calculate Angle Of Second Point	(  0 )
			u=((theta+20)/180.0f*3.142f);						// Calculate Angle Of Second Point	( 20 )

			x=(float)(cos(u)*(2.0f+cos(v) ))*r;					// Calculate x Position (2nd Point)
			y=(float)(sin(u)*(2.0f+cos(v) ))*r;					// Calculate y Position (2nd Point)
			z=(float)((( u-(2.0f*3.142f)) + sin(v) ) * r);		// Calculate z Position (2nd Point)

			vertexes[1][0]=x;									// Set x Value Of Second Vertex
			vertexes[1][1]=y;									// Set y Value Of Second Vertex
			vertexes[1][2]=z;									// Set z Value Of Second Vertex

			v=((phi+20)/180.0f*3.142f);							// Calculate Angle Of Third Point	( 20 )
			u=((theta+20)/180.0f*3.142f);						// Calculate Angle Of Third Point	( 20 )

			x=(float)(cos(u)*(2.0f+cos(v) ))*r;					// Calculate x Position (3rd Point)
			y=(float)(sin(u)*(2.0f+cos(v) ))*r;					// Calculate y Position (3rd Point)
			z=(float)((( u-(2.0f*3.142f)) + sin(v) ) * r);		// Calculate z Position (3rd Point)

			vertexes[2][0]=x;									// Set x Value Of Third Vertex
			vertexes[2][1]=y;									// Set y Value Of Third Vertex
			vertexes[2][2]=z;									// Set z Value Of Third Vertex

			v=((phi+20)/180.0f*3.142f);							// Calculate Angle Of Fourth Point	( 20 )
			u=((theta)/180.0f*3.142f);							// Calculate Angle Of Fourth Point	(  0 )

			x=(float)(cos(u)*(2.0f+cos(v) ))*r;					// Calculate x Position (4th Point)
			y=(float)(sin(u)*(2.0f+cos(v) ))*r;					// Calculate y Position (4th Point)
			z=(float)((( u-(2.0f*3.142f)) + sin(v) ) * r);		// Calculate z Position (4th Point)

			vertexes[3][0]=x;									// Set x Value Of Fourth Vertex
			vertexes[3][1]=y;									// Set y Value Of Fourth Vertex
			vertexes[3][2]=z;									// Set z Value Of Fourth Vertex

			calcNormal(vertexes,normal);						// Calculate The Quad Normal

			glNormal3f(normal[0],normal[1],normal[2]);			// Set The Normal

			// Render The Quad
			glVertex3f(vertexes[0][0],vertexes[0][1],vertexes[0][2]);
			glVertex3f(vertexes[1][0],vertexes[1][1],vertexes[1][2]);
			glVertex3f(vertexes[2][0],vertexes[2][1],vertexes[2][2]);
			glVertex3f(vertexes[3][0],vertexes[3][1],vertexes[3][2]);
		}
	}
	glEnd();													// Done Rendering Quads

	glPopMatrix();												// Pop The Matrix
}

/**	            This Code Set Up An Ortho View.
*
* \brief      Fonction ViewOrtho : positionne une vue orthogonale
* \details    Rien
* \return     void          un void (aucun retour)                                  *
*
*/

void ViewOrtho()												// Set Up An Ortho View
{
	glMatrixMode(GL_PROJECTION);								// Select Projection
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
	glOrtho( 0, 640 , 480 , 0, -1, 1 );							// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);									// Select Modelview Matrix
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
}

/**	            This Code Set Up A Perspective View.
*
* \brief      Fonction ViewPerspective : positionne une vue perspective
* \details    Rien
* \return     void          un void (aucun retour)                                  *
*
*/

void ViewPerspective()											// Set Up A Perspective View
{
	glMatrixMode( GL_PROJECTION );								// Select Projection
	glPopMatrix();												// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );								// Select Modelview
	glPopMatrix();												// Pop The Matrix
}

/**	            This Code Renders To A Texture.
*
* \brief      Fonction RenderToTexture : rend une texture
* \details    Rien
* \return     void          un void (aucun retour)                                  *
*
*/

void RenderToTexture()											// Renders To A Texture
{
	glViewport(0,0,TextureSize,TextureSize);									// Set Our Viewport (Match Texture Size)

	ProcessHelix();												// Render The Helix

	glBindTexture(GL_TEXTURE_2D,BlurTexture);					// Bind To The Blur Texture

	// Copy Our ViewPort To The Blur Texture (From 0,0 To 128,128... No Border)
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, 0, TextureSize, TextureSize, 0);

	glClearColor(0.0f, 0.0f, 0.5f, 0.5);						// Set The Clear Color To Medium Blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And Depth Buffer

	glViewport(0 , 0,640 ,480);									// Set Viewport (0,0 to 640x480)
}

/**	            This Code Draw The Blurred Image.
*
* \brief      Fonction DrawBlur : dessine l'image floue
* \details    En entrée les deux nouvelles dimensions de la fenêtre
* \param	  times			un entier : le nombre de fois 				            *
* \param	  inc		    un entier : l'incrément du "blur"			            *
* \return     void          un void (aucun retour)                                  *
*
*/

void DrawBlur(int times, float inc)								// Draw The Blurred Image
{
	float spost = 0.0f;											// Starting Texture Coordinate Offset
	float alphainc = 0.3f / times;								// Fade Speed For Alpha Blending
	float alpha = 0.1f;											// Starting Alpha Value
    int num = 0;

	// Disable AutoTexture Coordinates
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_2D);									// Enable 2D Texture Mapping
	glDisable(GL_DEPTH_TEST);									// Disable Depth Testing
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);							// Set Blending Mode
	//glBlendFunc(GL_ONE,GL_ONE);							// Set Blending Mode
	glEnable(GL_BLEND);											// Enable Blending
	glBindTexture(GL_TEXTURE_2D,BlurTexture);					// Bind To The Blur Texture
	ViewOrtho();												// Switch To An Ortho View

	alphainc = alpha / times;									// alphainc=0.2f / Times To Render Blur

	glBegin(GL_QUADS);											// Begin Drawing Quads
		for (num = 0;num < times;num++)						// Number Of Times To Render Blur
		{
			glColor4f(1.0f, 1.0f, 1.0f, alpha);					// Set The Alpha Value (Starts At 0.2)
			glTexCoord2f(0+spost,1-spost);						// Texture Coordinate	( 0, 1 )
			glVertex2f(0,0);									// First Vertex		(   0,   0 )

			glTexCoord2f(0+spost,0+spost);						// Texture Coordinate	( 0, 0 )
			glVertex2f(0,480);									// Second Vertex	(   0, 480 )

			glTexCoord2f(1-spost,0+spost);						// Texture Coordinate	( 1, 0 )
			glVertex2f(640,480);								// Third Vertex		( 640, 480 )

			glTexCoord2f(1-spost,1-spost);						// Texture Coordinate	( 1, 1 )
			glVertex2f(640,0);									// Fourth Vertex	( 640,   0 )

			spost += inc;										// Gradually Increase spost (Zooming Closer To Texture Center)
			alpha = alpha - alphainc;							// Gradually Decrease alpha (Gradually Fading Image Out)
		}
	glEnd();													// Done Drawing Quads

	ViewPerspective();											// Switch To A Perspective View

	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glDisable(GL_TEXTURE_2D);									// Disable 2D Texture Mapping
	glDisable(GL_BLEND);										// Disable Blending
	glBindTexture(GL_TEXTURE_2D,0);								// Unbind The Blur Texture
}

/**	            This Code Resize the main window.
*
* \brief      Fonction ReSizeGLScene : redimensionnement de la fenêtre Windows gérant OpenGL
* \details    En entrée les deux nouvelles dimensions de la fenêtre
* \param	  width			Width of the GL Window Or Fullscreen Mode				*
* \param	  height		Height of the GL Window Or Fullscreen Mode			    *
* \return     GLvoid        un void OpenGL (aucun retour)                           *
*
*/

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{



	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(50, (float)width/(float)height, 5,  2000); // Set Our Perspective

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

BOOL Initialize ()					// Any GL Init Code & User Initialization Goes Here
{

	// Start Of User Initialization
	GLfloat angle		= 0.0f;									// Set Starting Angle To Zero

// #ifndef _MSC_VER

	GLfloat global_ambient[4] = {0.2f, 0.2f,  0.2f, 1.0f};		// Set Ambient Lighting To Fairly Dark Light (No Color)
	GLfloat light0pos[4] =      {0.0f, 5.0f, 10.0f, 1.0f};		// Set The Light Position
	GLfloat light0ambient[4] =  {0.2f, 0.2f,  0.2f, 1.0f};		// More Ambient Light
	GLfloat light0diffuse[4] =  {0.3f, 0.3f,  0.3f, 1.0f};		// Set The Diffuse Light A Bit Brighter
	GLfloat light0specular[4] = {0.8f, 0.8f,  0.8f, 1.0f};		// Fairly Bright Specular Lighting

	GLfloat lmodel_ambient[4] = {0.2f,0.2f,0.2f,1.0f};			// And More Ambient Light

// #else
/*
    GLfloat global_ambient[4];
    GLfloat light0pos[4];
    GLfloat light0ambient[4];
    GLfloat light0diffuse[4];
    GLfloat light0specular[4];
    GLfloat lmodel_ambient[4];

	global_ambient[0]=0.2f; 		            // Set Ambient Lighting To Fairly Dark Light (No Color)
    global_ambient[1]=0.2f;
	global_ambient[2]=0.2f;
	global_ambient[3]=1.0f;
	light0pos[0]= 0.0f; 	                    // Set The Light Position
	light0pos[1]= 5.0f;
	light0pos[2]= 10.0f;
    light0pos[3]= 1.0f;
	light0ambient[0]=0.2f;		                // More Ambient Light
	light0ambient[1]=0.2f;
	light0ambient[2]=0.2f;
    light0ambient[3]=1.0f;
	light0diffuse[0]=0.3f;		                // Set The Diffuse Light A Bit Brighter
	light0diffuse[1]=0.3f;
	light0diffuse[2]=0.3f;
    light0diffuse[3]=1.0f;
	light0specular[0]=0.8f;		                // Fairly Bright Specular Lighting
	light0specular[1]=0.8f;
	light0specular[2]=0.8f;
    light0specular[3]= 1.0f;
	lmodel_ambient[0]= 0.2f;			        // And More Ambient Light
	lmodel_ambient[1]= 0.2f;
	lmodel_ambient[2]= 0.2f;
    lmodel_ambient[3]= 1.0f;
*/

// #endif // _MSC_VER

	BlurTexture = EmptyTexture();								// Create Our Empty Texture

	glViewport(0 , 0,width ,height);	// Set Up A Viewport
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glLoadIdentity();											// Reset The Projection Matrix
	gluPerspective(50, (float)width/(float)height, 5,  2000); // Set Our Perspective
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix

	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);		// Set The Ambient Light Model

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);		// Set The Global Ambient Light Model
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);				// Set The Lights Position
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);			// Set The Ambient Light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);			// Set The Diffuse Light
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);			// Set Up Specular Lighting
	glEnable(GL_LIGHTING);										// Enable Lighting
	glEnable(GL_LIGHT0);										// Enable Light0

	glShadeModel(GL_SMOOTH);									// Select Smooth Shading

	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5);						// Set The Clear Color To Black

	return TRUE;												// Return TRUE (Initialization Successful)
}

void Deinitialize (void)										// Any User DeInitialization Goes Here
{
	glDeleteTextures(1,&BlurTexture);							// Delete The Blur Texture
}

/**	            This Code draw the scene OpenGL in the main window.
*
* \brief      Fonction DrawGLScene : Dessin sous OpenGL dans la fenêtre Windows.
* \details    Rien
* \param	  GLvoid		    un void OpenGL (aucun paramètre)
* \return     void        	    un void (aucun retour)
*
*/

void DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.5);						// Set The Clear Color To Black
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();											// Reset The View
	RenderToTexture();											// Render To A Texture
	ProcessHelix();												// Draw Our Helix
	DrawBlur(20,0.055f);											// Draw The Blur Effect
	glFlush ();													// Flush The GL Rendering Pipeline

}

/**	            This Code destroy all resources of this program.
*
* \brief      Fonction KillGLWindow : Destruction de toutes les ressources du programme.
* \details    Rien
* \param	  GLvoid			un void OpenGL.
* \return     GLvoid        	un void OpenGL.
*
*/

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{


	Deinitialize();										// Get rid of allocated texture

	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/**	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *  \brief          Creation of our OpenGL Window
 *  \param 			title			- Title To Appear At The Top Of The Window				*
 *	\param 			width			- Width Of The GL Window Or Fullscreen Mode				*
 *	\param 			height			- Height Of The GL Window Or Fullscreen Mode			*
 *	\param          bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	\param          fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*
 *  \return         BOOL            - un entier booleen (OK ou non).
 */

BOOL CreateGLWindow(char* title, int width, int height, int bits, BOOL fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

    pfd.cColorBits = bits;

    if (!(hDC=GetDC(hWnd)))                         // Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!Initialize())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

/**         Comments manageable by Doxygen
*
* \brief      Fonction CALLBACK de traitement des messages Windows
* \details    Traitement de la boucle infinie des messages Windows
* \param      hWnd         L'header de la fenêtre principale.
* \param      uMsg         Un entier non signé.
* \param      wParam       Les paramétres en entrée.
* \param      lParam       Autres paramétres en entrée.
* \return     LRESULT 	   Un LRESULT donnant le status du traitement du message.
*
*/

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

/**	This Code is mandatory to create windows application (function WinMain)					*
 *  \brief          Creation of our application on Windows System (not console application) *
 *  \param 			hInstance		- Header de l'instance Windows				            *
 *	\param 			hPrevInstance	- Header de l'instance précédente de Windows 			*
 *	\param 			lpCmdLine		- Un pointeur sur la ligne de commande 			        *
 *	\param          nCmdShow		- Un indicateur d'état			                        *
 *  \return         int             - un entier booleen (OK ou non).                        *
 *	                                                                                        *
 */

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// BOOL Variable To Exit Loop
	float	t,t0;

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("[NeHe Radial] _ by rIO / Spinning Kids",width,height,32,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}


	t0 = GetTickCount();

	while(!done)									// Loop That Runs While done=FALSE

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{

					t = GetTickCount();

					angle += (float)(t-t0) * 0.5f;			// Update angle Based On The Clock
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)

					t0 = t;
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's OpenGL Framework",width,height,32,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}


	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
