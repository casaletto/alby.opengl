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

        on_gl_create( hwnd, _hdc, _hrc ) ;
    }

    void glApplication::on_wm_destroy( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        on_gl_destroy( hwnd, _hdc, _hrc ) ;

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

    void glApplication::on_wm_paint(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect )
    {
        on_idle( hwnd ) ;
    }

    void glApplication::on_idle( HWND hwnd ) 
    {
        ::RECT rect ;
        ::GetClientRect( hwnd, &rect ) ;

        ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) ;

        on_gl_paint( hwnd, _hdc, _hrc, rect, l::hlp::secondsSinceEpoch() ) ;

        ::SwapBuffers( _hdc ) ;
    }

    void glApplication::on_gl_create( HWND hwnd, HDC hdc, HGLRC hglrc )
    {
    }

    void glApplication::on_gl_destroy( HWND hwnd, HDC hdc, HGLRC hglrc ) 
    {
    }

    void glApplication::on_gl_paint( HWND hwnd, HDC hdc, HGLRC hglrc, const::RECT& rect, double secondsSinceEpoch )
    {
    }

} // end ns

