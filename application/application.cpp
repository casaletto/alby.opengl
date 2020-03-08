#include <windows.h>
#include <atlbase.h>
#include <iostream>
#include <string>
#include <exception>
#include <string> 
#include <typeinfo> 
#include <thread>
#include <chrono>
#include <exception>
#include <filesystem>

#include "hlp.h"
#include "application.h"

namespace s = std ;

namespace au {
    namespace com {
        namespace casaletto {
            namespace _2020 {
                namespace lib {
                    
application* application::_application = nullptr ;
s::string    application::_exception ;

application::application( const s::string& title )
{
    hlp::ods( "application" ) ; 

    _title   = title ;
    _exePath = hlp::exePath() ;

    hlp::ods( _exePath.c_str() ) ;
}

application::~application()
{
    hlp::ods( "~application" ) ;
}

s::string application::getTitle()
{
    return _title ;
}

void application::run( int argc, WCHAR** argv, int width, int height )
{
    _application = this ;
    _exception   = "" ;

    hlp::ods( "run() %s %s", typeid(*this).name(), _title.c_str() ) ;

    auto temp = s::vector<s::wstring>{ argv, argv + argc };
    s::for_each( temp.begin(), temp.end(),
        [this] ( auto &x )
        {
            _args.push_back( hlp::w2a(x) ) ;
        }) ;

    int i = 0 ;
    for ( auto arg : _args ) 
        hlp::ods( "arg(%d) %s", i++, arg.c_str() ) ;
    
    auto theTitle = lib::hlp::a2w( _title ) ;

    // register windows class 
    ::WNDCLASS wc ;

    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;
    wc.lpfnWndProc   = wndProc ;
    wc.cbClsExtra    = 0 ;
    wc.cbWndExtra    = 0 ;
    wc.hInstance     = nullptr ;
    wc.hIcon         = nullptr ;
    wc.hCursor       = ::LoadCursor( nullptr, IDC_ARROW ) ;
    wc.hbrBackground = nullptr ; 
    wc.lpszMenuName  = theTitle.c_str() ;
    wc.lpszClassName = theTitle.c_str() ;

    if ( ::RegisterClass( &wc ) == 0 )
        throw s::exception( "::RegisterClass() failed" ) ;

    // createthe window
    auto hwnd = ::CreateWindowEx
    (
        0,
        theTitle.c_str(),
        theTitle.c_str(),
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    ) ;

    if ( hwnd == nullptr )
        throw s::exception( "::CreateWindow() failed" ) ;

    ::ShowWindow( hwnd, SW_NORMAL ) ;
    ::UpdateWindow( hwnd ) ;

    messageLoop( hwnd ) ;

    if ( _exception.length() > 0 )
         throw s::exception( _exception.c_str() ) ;
}

void application::messageLoop( HWND hwnd )
{
    ::MSG  msg  ;

    while ( true ) 
    {
        while ( ::PeekMessage( &msg, nullptr, 0, 0, PM_NOREMOVE ) != 0 ) 
        {
            if ( ::GetMessage( &msg, nullptr, 0, 0 ) == 0 ) return ;

            ::TranslateMessage( &msg ) ;
            ::DispatchMessage ( &msg ) ;
        }

        try
        {
            on_idle( hwnd ) ;
        }
        catch( const s::exception& ex )
        {
            _exception = ex.what() ;
            ::PostMessage( hwnd, WM_CLOSE, 0, 0 );
        }
    }
}

LRESULT CALLBACK application::wndProc( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
    switch( umsg ) 
    {
        case WM_CREATE:
        {
            hlp::ods( "WM_CREATE" ) ;
            try
            {
                _application->on_wm_create(hwnd, umsg, wparam, lparam); 
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
                ::PostMessage( hwnd, WM_CLOSE, 0, 0 ) ;
            }
            return 0 ;
        }

        case WM_ERASEBKGND:
        {
            return 1 ;
        }

        case WM_PAINT: 
        {
            hlp::ods( "WM_PAINT" ) ; 

            ::RECT rect ;
            ::GetClientRect( hwnd, &rect ) ;

            ::PAINTSTRUCT ps ;
            auto hdc = ::BeginPaint( hwnd, &ps ) ;
            try
            {
                _application->on_wm_paint( hwnd, umsg, wparam, lparam, hdc, rect ) ; 
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
                ::PostMessage( hwnd, WM_CLOSE, 0, 0 ) ;
            }
            ::EndPaint( hwnd, &ps ) ;
            return 0 ;
        }

        case WM_SIZE:
        {
            hlp::ods( "WM_SIZE" ) ;

            ::RECT rect ;
            ::GetClientRect( hwnd, &rect ) ;

            try
            {
                _application->on_wm_size( hwnd, umsg, wparam, lparam, rect ) ;
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
                ::PostMessage( hwnd, WM_CLOSE, 0, 0 ) ;
            }
            return 0 ;
        }

        case WM_CLOSE:
        {
            hlp::ods( "WM_CLOSE" ) ;
            try
            {
                _application->on_wm_close( hwnd, umsg, wparam, lparam ) ;
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
            }
            ::DestroyWindow( hwnd ) ;
            return 0 ;
        }

        case WM_DESTROY:
        {
            hlp::ods( "WM_DESTROY" ) ;
            try
            {
                _application->on_wm_destroy( hwnd, umsg, wparam, lparam ) ;
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
            }
            ::PostQuitMessage( 0 ) ;
            return 0 ;
        }

        case WM_KEYDOWN: 
        {
            hlp::ods( "WM_KEYDOWN" ) ;
            try
            {
                _application->on_wm_keydown( hwnd, umsg, wparam, lparam ) ;
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
                ::PostMessage( hwnd, WM_CLOSE, 0, 0 ) ;
            }
            return 0 ;
        }

        case WM_CHAR: 
        {
            hlp::ods( "WM_CHAR" ) ;
            try
            {
                _application->on_wm_char( hwnd, umsg, wparam, lparam ) ;
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
                ::PostMessage( hwnd, WM_CLOSE, 0, 0 ) ;
            }
            return 0 ;
        }

        case WM_MOUSEWHEEL: 
        {
            hlp::ods( "WM_MOUSEWHEEL" ) ;
            try
            {
                _application->on_wm_mousewheel( hwnd, umsg, wparam, lparam ) ;
            }
            catch( const s::exception& ex )
            {
                _exception = ex.what() ;
                ::PostMessage( hwnd, WM_CLOSE, 0, 0 ) ;
            }
            return 0 ;
        }

        default: 
            return ::DefWindowProc( hwnd, umsg, wparam, lparam ) ;
    }

    return 0 ;

} // end wnd proc

void application::on_wm_paint( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect )
{
    auto height = - ::MulDiv( 24, ::GetDeviceCaps( hdc, LOGPIXELSY), 72 ) ;
    auto hfont  = ::CreateFont( height, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, L"Arial" ) ;
    ::SelectObject( hdc, hfont ) ;

    ::SetBkMode( hdc, TRANSPARENT ) ;
    auto hbrush = ::CreateSolidBrush( RGB(0x80, 0x80, 0x80) ) ;
    ::FillRect( hdc, &rect, hbrush ) ;

    auto str = hlp::format( L"%S\n%dx%d", _title, rect.right, rect.bottom ) ;
    
    ::RECT rect2 = rect ;
    ::RECT textrect = rect ;
    ::DrawText( hdc, str.c_str(), -1, &textrect, DT_CALCRECT | DT_WORDBREAK | DT_SINGLELINE ) ;
    rect2.top = rect.bottom/2 - textrect.bottom/2 ;
            
    ::SetTextColor( hdc, RGB( 0x00, 0x00, 0x00 ) ) ;
    ::DrawText( hdc, str.c_str(), -1, &rect2, DT_CENTER | DT_VCENTER ) ;

    ::DeleteObject( hbrush ) ;
    ::DeleteObject( hfont ) ;
}

void application::on_wm_size(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, const ::RECT& rect ) 
{
}

void application::on_wm_close( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
}

void application::on_wm_destroy( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
}

void application::on_wm_keydown( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
}

void application::on_wm_char( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
    if ( wparam == VK_ESCAPE )
        ::PostMessage( hwnd, WM_CLOSE, 0, 0 ) ;
}

void application::on_wm_create( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
}

void application::on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
}

void application::on_idle( HWND hwnd ) 
{
}

} } } } } // end namespace


