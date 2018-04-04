#pragma once

#include <GL/glut.h>
#include <stdio.h>
#include <string>

class texture
{
    public:
        texture();
        ~texture();
        bool loadTextureFromFile( std::string path );
        bool loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height );
        void freeTexture();
        void render( GLfloat x, GLfloat y );
        GLuint getTextureID();
        GLuint textureWidth();
        GLuint textureHeight();

    private:
        GLuint mTextureID;

        GLuint mTextureWidth;
        GLuint mTextureHeight;
};
