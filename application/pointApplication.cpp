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
#include "glProgram.h"
#include "pointApplication.h"

namespace s = std;
namespace l = au::com::casaletto::_2020::lib;
namespace g = glprogram ;

//TODO
//
// move point around x y z
// point colour and size
// xyz axes, dump info
// rotate axes , camera, perspective  
// rotect xyz, zoom in and out
//

namespace me
{
    const char* pointApplication::_vertexGsls = R"(
#version 460 core                             
                                              
void main(void)                               
{                                             
    gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 ) ;   
} 
)";

    const char* pointApplication::_fragmentGsls = R"(
#version 460 core                             
                                              
out vec4 color;                               
                                              
void main(void)                               
{                                             
    color = vec4( 0.0, 0.8, 1.0, 1.0 ) ;         
}                                             
)";

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
        _program.create( _vertexGsls, _fragmentGsls ) ;
    }

    void pointApplication::on_gl_destroy( HWND hwnd, HDC hdc, HGLRC hglrc ) 
    {
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
        
        _program.use() ;

        ::glPointSize( (float) _pixelSize ) ;
        ::glDrawArrays( GL_POINTS, 0, 1 ) ;
    }

} // end ns


