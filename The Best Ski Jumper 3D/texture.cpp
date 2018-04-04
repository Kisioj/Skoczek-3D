#include "texture.h"
#include <IL/IL.h>
#include <IL/ILU.h>
#include <iostream>


texture::texture()
{
    mTextureID = 0;
    mTextureWidth = 0;
    mTextureHeight = 0;
}

texture::~texture()
{
    freeTexture();
}



bool texture::loadTextureFromFile( std::string path )
{
    bool textureLoaded = false;
    ILuint imgID = 0;
    ilGenImages( 1, &imgID );
    ilBindImage( imgID );

	ILboolean success= ilLoadImage( path.c_str() );
	iluFlipImage();

    if( success == IL_TRUE )
    {
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if( success == IL_TRUE )
        {
            textureLoaded = loadTextureFromPixels32( (GLuint*)ilGetData(), (GLuint)ilGetInteger( IL_IMAGE_WIDTH ), (GLuint)ilGetInteger( IL_IMAGE_HEIGHT ) );
        }
        ilDeleteImages( 1, &imgID );
    }

    if( !textureLoaded )
    {
        printf( "Unable to load %s\n", path.c_str() );
    }


    return textureLoaded;
}

bool texture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height )
{
    freeTexture();

    mTextureWidth = width;
    mTextureHeight = height;

    glGenTextures( 1, &mTextureID );

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glBindTexture( GL_TEXTURE_2D, NULL );

    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }
    return true;
}

void texture::freeTexture()
{
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
}

void texture::render( GLfloat x, GLfloat y )
{
    if( mTextureID != 0 )
    {
        glLoadIdentity();
        glTranslatef( x, y, 0.f );
        glBindTexture( GL_TEXTURE_2D, mTextureID );

        glBegin( GL_QUADS );
            glTexCoord2i( 0, 0 ); glVertex2i(           0,				   0);
            glTexCoord2i( 1, 0 ); glVertex2i( mTextureWidth,			   0);
            glTexCoord2i( 1, 1 ); glVertex2i( mTextureWidth, mTextureHeight );
            glTexCoord2i( 0, 1 ); glVertex2i(           0, mTextureHeight );
        glEnd();
    }
}

GLuint texture::getTextureID()
{
    return mTextureID;
}

GLuint texture::textureWidth()
{
    return mTextureWidth;
}

GLuint texture::textureHeight()
{
    return mTextureHeight;
}


