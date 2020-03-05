#include <windows.h>
#include <atlbase.h>
#include <iostream>
#include <string>
#include <exception>
#include <string> 
#include <exception>
#include <filesystem>

#include "hlp.h"
#include "application.h"
#include "a2wApplication.h"

namespace s = std ;
namespace l = au::com::casaletto::_2020::lib ;

namespace me
{
    a2wApplication::a2wApplication( const s::string& title ) 
        : l::application( title )
    {
        l::hlp::ods( "a2wApplication" ) ;

        _fontsize = 10 ;
    }

    a2wApplication::~a2wApplication()
    {
        l::hlp::ods( "~a2wApplication" ) ;
    }

    void a2wApplication::on_wm_paint( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect )
    {
        l::hlp::ods( "font size %d", _fontsize ) ;

        auto text = l::hlp::format( L"%s\n\n%s", _wstr1.c_str(), _wstr2.c_str() ) ;

        l::hlp::ods( "text length %d", text.length() ) ;

        auto height = - ::MulDiv( _fontsize, ::GetDeviceCaps( hdc, LOGPIXELSY ), 72 ) ;
        auto hfont  = ::CreateFont( height, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, L"Tahoma" ) ;
        ::SelectObject( hdc, hfont ) ;

        ::SetBkMode( hdc, TRANSPARENT ) ;
        auto hbrush = ::CreateSolidBrush( RGB(0x20, 0x00, 0x00) ) ;
        ::FillRect( hdc, &rect, hbrush ) ;

        ::SetTextColor( hdc, RGB( 0x80, 0xFF, 0xFF ) ) ;
        ::DrawText( hdc, text.c_str(), -1, (LPRECT) &rect, DT_LEFT | DT_TOP | DT_WORDBREAK ) ;

        ::DeleteObject( hbrush ) ;
        ::DeleteObject( hfont ) ;
    }

    void a2wApplication::on_wm_create( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        auto exeDir = s::filesystem::path( _exePath ).remove_filename() ;

        auto file = exeDir / "../.." / "doc/utf8 test file.txt" ;
        auto str = l::hlp::readFileUtf8( file ) ;
        _wstr1 = l::hlp::a2w( str ) ;

        l::hlp::ods( "file %s %d", file.string().c_str(), _wstr1.length() ) ;

        file = exeDir / "../.." / "doc/utf16 test file.txt" ;
        _wstr2 = l::hlp::readFileUtf16( file ) ;

        l::hlp::ods( "file %s %d", file.string().c_str(), _wstr2.length() ) ;
    }

    void a2wApplication::on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        auto delta = GET_WHEEL_DELTA_WPARAM( wparam ) ;

        if ( delta > 0 )
            _fontsize++ ;
        else
            _fontsize-- ;

        if ( _fontsize < 4 )
             throw s::exception( "font too small girl" ) ;

        ::InvalidateRect( hwnd, nullptr, true ) ;
    }

} // end ns


