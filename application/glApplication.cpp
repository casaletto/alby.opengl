#include <windows.h>
#include "GL/gl3w.h"

#include <atlbase.h>
#include <iostream>
#include <string>
#include <exception>
#include <string> 
#include <exception>
#include <filesystem>
#include <math.h>

#include "hlp.h"
#include "application.h"
#include "glApplication.h"

namespace s = std ;
namespace l = au::com::casaletto::_2020::lib ;

namespace me
{
    glApplication::glApplication( const s::string& title )
        : l::application( title )
    {
        l::hlp::ods( "glApplication" ) ;
    }

    glApplication::~glApplication()
    {
        l::hlp::ods( "~glApplication" ) ;
    }

    void glApplication::on_wm_paint( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect )
    {
        // call on_gl_render
    }

    void glApplication::on_wm_create( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        _hdc = ::GetDC( hwnd ) ;

        ::PIXELFORMATDESCRIPTOR pfd = 
        {
            sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd  
            1,                              // version number  

            PFD_DRAW_TO_WINDOW |            // support window  
            PFD_SUPPORT_OPENGL |            // support OpenGL  
            PFD_DOUBLEBUFFER,               // double buffered  

            PFD_TYPE_RGBA,                  // RGBA type  
            32,                             // 32-bit color depth  

            0, 0, 0, 0, 0, 0,               // color bits ignored  
            0,                              // no alpha buffer  
            0,                              // shift bit ignored  
            0,                              // no accumulation buffer  
            0, 0, 0, 0,                     // accum bits ignored  

            32,                             // 32-bit z-buffer  

            0,                              // no stencil buffer  
            0,                              // no auxiliary buffer  
            PFD_MAIN_PLANE,                 // main layer  
            0,                              // reserved  
            0, 0, 0                         // layer masks ignored  
        } ;

        auto pixelFormat = ::ChoosePixelFormat( _hdc, &pfd ) ;
        if ( pixelFormat == 0 )
             throw s::exception( "::ChoosePixelFormat() failed." ) ;

        auto b = ::SetPixelFormat( _hdc, pixelFormat, &pfd ) ;
        if ( ! b )
            throw s::exception( "::SetPixelFormat() failed." ) ;

        _hrc = ::wglCreateContext( _hdc ) ;
        if ( _hrc == nullptr )
            throw s::exception( "::wglCreateContext() failed." ) ;

        b = ::wglMakeCurrent( _hdc, _hrc ) ;
        if ( ! b )
            throw s::exception( "::SetPixelFormat() failed." ) ;

        if ( gl3wInit() != 0 ) 
             throw s::exception("::gl3wInit() failed.");

        l::hlp::ods( "OpenGL version: %s",  glGetString( GL_VERSION  ) ) ;
        l::hlp::ods( "OpenGL shader: %s ",  glGetString( GL_SHADING_LANGUAGE_VERSION ) ) ;
        l::hlp::ods( "OpenGL vendor: %s",   glGetString( GL_VENDOR   ) ) ;
        l::hlp::ods( "OpenGL renderer: %s", glGetString( GL_RENDERER ) ) ;


        //-----------------------------------
        // call on_gl_create

        // fix this
        auto vs_source = R"(
#version 460 core                             
                                              
void main(void)                               
{                                             
    gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 ) ;   
} 
)" ;

        auto fs_source = 
R"(
#version 460 core                             
                                              
out vec4 color;                               
                                              
void main(void)                               
{                                             
    color = vec4( 0.0, 0.8, 1.0, 1.0 ) ;         
}                                             
)" ;

        _program = ::glCreateProgram() ; 
        auto fs = ::glCreateShader( GL_FRAGMENT_SHADER);
        ::glShaderSource(fs, 1, &fs_source, NULL);
        ::glCompileShader(fs);

        auto vs = ::glCreateShader(GL_VERTEX_SHADER);
        ::glShaderSource(vs, 1, &vs_source, NULL);
        ::glCompileShader(vs);

        ::glAttachShader(_program, vs);
        ::glAttachShader(_program, fs);

        ::glLinkProgram(_program);

        ::glGenVertexArrays(1, &_vao); 
        ::glBindVertexArray(_vao);
    }

    void glApplication::on_wm_destroy( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        // call on_gl_destroy
        ::glDeleteVertexArrays(1, &_vao);
        ::glDeleteProgram(_program);


        //------------------------------------------------

        if ( _hdc != nullptr ) 
             ::wglMakeCurrent( _hdc, nullptr ) ;

        if ( _hrc != nullptr ) 
             ::wglDeleteContext( _hrc ) ;

        if ( _hdc != nullptr )
             ::ReleaseDC( hwnd, _hdc ) ;

        _hdc = nullptr ;
        _hrc = nullptr ;
    }

    void glApplication::on_wm_size( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, const ::RECT& rect )
    {
        ::glViewport( 0, 0, rect.right, rect.bottom ) ;
    }

    //ALBy change to on_gl_render
    // have on idle sleep ms
    void glApplication::on_idle( HWND hwnd, double secondsSinceEpoch )
    {
        ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) ;

        _i++;

        auto rad = s::fmod( (double) _i / 200.0, M_PI * 2.0 ) ;
        auto r = s::sin( rad ) * 0.5 + 0.5 ;
        auto g = s::cos( rad ) * 0.5 + 0.5 ;


        //GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
        GLfloat color[] = 
        { 
            (float) r, //(float)sin( _i / 10.0 ) * 0.5f + 0.5f, 
            (float) g, // 0.0, //(float)cos( _i ) * 0.5f + 0.5f, 
            0.0f, 
            1.0f 
        } ;
        
        ::glClearBufferfv( GL_COLOR, 0, color ) ;
        ::glUseProgram( _program ) ;

        ::glPointSize( (float) _pixelSize ) ;
        ::glDrawArrays( GL_POINTS, 0, 1 ) ;

        ::SwapBuffers( _hdc ) ; 

        //l::hlp::sleep( 100 ) ;
    }

    void glApplication::on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        auto delta = GET_WHEEL_DELTA_WPARAM( wparam ) ;

        if ( delta > 0 )
             _pixelSize++ ;
        else
             _pixelSize-- ;

        if ( _pixelSize < 1 )
             _pixelSize = 1 ;
    }

} // end ns


