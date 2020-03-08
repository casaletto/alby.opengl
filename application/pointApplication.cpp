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
#include "pointApplication.h"

namespace s = std ;
namespace l = au::com::casaletto::_2020::lib ;

namespace me
{
    pointApplication::pointApplication( const s::string& title )
        : glApplication( title )
    {
        l::hlp::ods( "pointApplication" ) ;
    }

    pointApplication::~pointApplication()
    {
        l::hlp::ods( "~pointApplication" ) ;
    }

    void pointApplication::on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        auto delta = GET_WHEEL_DELTA_WPARAM( wparam ) ;

        if ( delta > 0 )
             _pixelSize++ ;
        else
             _pixelSize-- ;

        if ( _pixelSize < 1 )
             _pixelSize = 1 ;
    }

    void pointApplication::on_gl_create( HWND hwnd, HDC hdc, HGLRC hglrc )
    {
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

//ALBY to do : maske progranm class, and then base class
        _program = ::glCreateProgram() ; 
        auto fs = ::glCreateShader( GL_FRAGMENT_SHADER ) ;
        ::glShaderSource( fs, 1, &fs_source, NULL ) ;
        ::glCompileShader( fs );

        auto vs = ::glCreateShader( GL_VERTEX_SHADER ) ;
        ::glShaderSource( vs, 1, &vs_source, NULL ) ;
        ::glCompileShader(vs ) ;

        ::glAttachShader( _program, vs ) ;
        ::glAttachShader( _program, fs ) ;

        ::glLinkProgram( _program ) ;

        ::glGenVertexArrays( 1, &_vao ) ; 
        ::glBindVertexArray( _vao ) ;
    }

    void pointApplication::on_gl_destroy( HWND hwnd, HDC hdc, HGLRC hglrc ) 
    {
        ::glDeleteVertexArrays( 1, &_vao ) ;
        ::glDeleteProgram( _program ) ;
    }

    void pointApplication::on_gl_paint( HWND hwnd, HDC hdc, HGLRC hglrc, const::RECT& rect, double secondsSinceEpoch )
    {
        _i++ ;

        auto rad = s::fmod( (double) _i / 200.0, M_PI * 2.0 ) ;
        auto r = s::sin( rad ) * 0.5 + 0.5 ;
        auto g = s::cos( rad ) * 0.5 + 0.5 ;

        float color[] = 
        { 
            (float) r, 
            0.0f, 
            1.0f 
        } ;
        
        ::glClearBufferfv( GL_COLOR, 0, color ) ;
        ::glUseProgram( _program ) ;

        ::glPointSize( (float) _pixelSize ) ;
        ::glDrawArrays( GL_POINTS, 0, 1 ) ;
    }

} // end ns


