//
//  main.cpp
//  Visualization
//
//  Created by Sayan Mandal on 05/03/19.
//  Copyright © 2019 Sayan Mandal. All rights reserved.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void DrawCube( GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength, GLfloat edgeWidth, GLfloat edgeDepth);

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;

int main( void )
{
    GLFWwindow *window;
    
    // Initialize the library
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL );
    
    glfwSetKeyCallback( window, keyCallback );
    glfwSetInputMode( window, GLFW_STICKY_KEYS, 1 );
    
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    glViewport( 0.0f, 0.0f, screenWidth, screenHeight ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment
    
    GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
    GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;
    
    
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Render OpenGL here
        
        glPushMatrix( );
        glTranslatef( halfScreenWidth, halfScreenHeight, -500 );
        glRotatef( rotationX, 1, 0, 0 );
        glRotatef( rotationY, 0, 1, 0 );
        glTranslatef( -halfScreenWidth, -halfScreenHeight, 500 );
        
        DrawCube( halfScreenWidth, halfScreenHeight, -500, 100, 200, 400 );
        
        glPopMatrix();
        
        
        // Swap front and back buffers
        glfwSwapBuffers( window );
        
        // Poll for and process events
        glfwPollEvents( );
    }
    
    glfwTerminate( );
    
    return 0;
}



void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
    //std::cout << key << std::endl;
    
    const GLfloat rotationSpeed = 10;
    
    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if ( action == GLFW_PRESS || action == GLFW_REPEAT )
    {
        switch ( key )
        {
            case GLFW_KEY_UP:
                rotationX -= rotationSpeed;
                break;
            case GLFW_KEY_DOWN:
                rotationX += rotationSpeed;
                break;
            case GLFW_KEY_RIGHT:
                rotationY += rotationSpeed;
                break;
            case GLFW_KEY_LEFT:
                rotationY -= rotationSpeed;
                break;
        }
        
        
    }
}


void DrawCube( GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength, GLfloat edgeWidth, GLfloat edgeDepth)
{
    GLfloat halfSideLength = edgeLength * 0.5f;
    GLfloat halfSideWidth = edgeWidth * 0.5f;
    GLfloat halfSideDepth = edgeDepth * 0.5f;
    
    GLfloat vertices[] =
    {
        // front face
        centerPosX - halfSideWidth, centerPosY + halfSideLength, centerPosZ + halfSideDepth, // top left
        centerPosX + halfSideWidth, centerPosY + halfSideLength, centerPosZ + halfSideDepth, // top right
        centerPosX + halfSideWidth, centerPosY - halfSideLength, centerPosZ + halfSideDepth, // bottom right
        centerPosX - halfSideWidth, centerPosY - halfSideLength, centerPosZ + halfSideDepth, // bottom left
        
        // back face
        centerPosX - halfSideWidth, centerPosY + halfSideLength, centerPosZ - halfSideDepth, // top left
        centerPosX + halfSideWidth, centerPosY + halfSideLength, centerPosZ - halfSideDepth, // top right
        centerPosX + halfSideWidth, centerPosY - halfSideLength, centerPosZ - halfSideDepth, // bottom right
        centerPosX - halfSideWidth, centerPosY - halfSideLength, centerPosZ - halfSideDepth, // bottom left
        
        // left face
        centerPosX - halfSideWidth, centerPosY + halfSideLength, centerPosZ + halfSideDepth, // top left
        centerPosX - halfSideWidth, centerPosY + halfSideLength, centerPosZ - halfSideDepth, // top right
        centerPosX - halfSideWidth, centerPosY - halfSideLength, centerPosZ - halfSideDepth, // bottom right
        centerPosX - halfSideWidth, centerPosY - halfSideLength, centerPosZ + halfSideDepth, // bottom left
        
        // right face
        centerPosX + halfSideWidth, centerPosY + halfSideLength, centerPosZ + halfSideDepth, // top left
        centerPosX + halfSideWidth, centerPosY + halfSideLength, centerPosZ - halfSideDepth, // top right
        centerPosX + halfSideWidth, centerPosY - halfSideLength, centerPosZ - halfSideDepth, // bottom right
        centerPosX + halfSideWidth, centerPosY - halfSideLength, centerPosZ + halfSideDepth, // bottom left
        
        // top face
        centerPosX - halfSideWidth, centerPosY + halfSideLength, centerPosZ + halfSideDepth, // top left
        centerPosX - halfSideWidth, centerPosY + halfSideLength, centerPosZ - halfSideDepth, // top right
        centerPosX + halfSideWidth, centerPosY + halfSideLength, centerPosZ - halfSideDepth, // bottom right
        centerPosX + halfSideWidth, centerPosY + halfSideLength, centerPosZ + halfSideDepth, // bottom left
        
        // top face
        centerPosX - halfSideWidth, centerPosY - halfSideLength, centerPosZ + halfSideDepth, // top left
        centerPosX - halfSideWidth, centerPosY - halfSideLength, centerPosZ - halfSideDepth, // top right
        centerPosX + halfSideWidth, centerPosY - halfSideLength, centerPosZ - halfSideDepth, // bottom right
        centerPosX + halfSideWidth, centerPosY - halfSideLength, centerPosZ + halfSideDepth  // bottom left
    };
    
    GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
    };
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    //glColor3f( colour[0], colour[1], colour[2] );
    //glColor3f(0.5f, 0.0f, 1.0f);
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, vertices );
    
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, g_color_buffer_data);
    
    glDrawArrays( GL_QUADS, 0, 24 );
    
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState(GL_COLOR_ARRAY);
}
