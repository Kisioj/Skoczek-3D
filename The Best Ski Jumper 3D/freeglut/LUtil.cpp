#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"


//Non-power-of-two texture
LTexture gNon2NTexture;

//Sprite texture
//LTexture gArrowTexture;

//Sprite area
//LFRect gArrowClips[ 4 ];

bool initGL()
{
	//Set the viewport
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    //Initialize DevIL
    ilInit();
	iluInit();
    ilClearColour( 255, 255, 255, 000 );

    //Check for error
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }

    return true;
}

void update()
{

}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Render OpenGL texture
	cout << gNon2NTexture.imageWidth() << "," << gNon2NTexture.imageHeight() << endl;
	cout << gNon2NTexture.textureWidth() << "," << gNon2NTexture.textureHeight() << endl;

    gNon2NTexture.render( ( SCREEN_WIDTH - gNon2NTexture.imageWidth() ) / 2.f, ( SCREEN_HEIGHT - gNon2NTexture.imageHeight() ) / 2.f );

    //Update screen
    glutSwapBuffers();
}


bool loadMedia()
{
    //Load texture
    if( !gNon2NTexture.loadTextureFromFile( "opengl.png" ) )
    {
        printf( "Unable to load non-power-of-two texture!\n" );
        return false;
    }

    return true;
}