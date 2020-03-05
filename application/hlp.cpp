#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <windows.h>
#include <atlbase.h>
#include <iostream>
#include <string>
#include <exception>
#include <string> 
#include <typeinfo> 
#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <exception>
#include <filesystem>
#include <codecvt>
#include <vector>
#include <cstdarg>
#include <chrono>
#include <ctime>

#include "hlp.h"

namespace s = std ;

namespace au {
    namespace com {
        namespace casaletto {
            namespace _2020 {
                namespace lib {

double hlp::secondsSinceEpoch() 
{
    auto duration = s::chrono::steady_clock::now().time_since_epoch() ; 

    return duration.count() / 1.0e06 ;
}

void hlp::sleep( long ms )
{
    s::this_thread::sleep_for( s::chrono::milliseconds( ms ) ) ; 
}

s::wstring hlp::readFileUtf16( const s::filesystem::path& filename )
{
    return readFileUtf16( filename.string() ) ;
}

s::wstring hlp::readFileUtf16( const s::string& filename )
{
    s::wifstream f( filename, s::ios::binary ) ;

    auto codecvt = new s::codecvt_utf16<WCHAR, 0x10ffff, s::consume_header>() ;
    
    auto locale  = f.imbue( s::locale( f.getloc(), codecvt ) ) ;

    s::wstringstream buffer ;
    
    buffer << f.rdbuf() ;

    auto wstr = buffer.str() ;

    return wstr ;
}

s::string hlp::readFileUtf8( const s::filesystem::path& filename )
{
    return readFileUtf8( filename.string() ) ;
}

s::string hlp::readFileUtf8( const s::string& filename )
{
    s::ifstream f( filename ) ;

    s::stringstream buffer ;
    
    buffer << f.rdbuf() ;

    auto str = buffer.str() ;

    return str ;
}

s::string hlp::exePath()
{
    wchar_t* p = nullptr ;
    _get_wpgmptr( &p ) ;

    return w2a( p ) ;
}

void hlp::ods( const s::string& str ) 
{
    auto wstr = a2w( str ) ;

    ods( wstr ) ;
}

void hlp::ods( const s::wstring& wstr ) 
{
    ::OutputDebugString( wstr.c_str() ) ;
}

void hlp::ods( const char* fmt, ... )
{
    va_list vl ;
    va_start( vl, fmt ) ; 

    auto size = ::_vscprintf( fmt, vl ) ;        
    size++ ;

    s::string str( size, 0 ) ;

    ::vsnprintf_s( str.data(), size, size, fmt, vl ) ;

    va_end( vl ) ;

    if ( str.size() > 0 ) 
         str.resize( str.size() - 1 ) ;

    auto wstr = a2w( str ) ;
    ::OutputDebugString( wstr.c_str() ) ;
}

void hlp::ods( const WCHAR* fmt, ...)
{
    va_list vl ;
    va_start( vl, fmt ) ; 

    auto size = ::_vscwprintf( fmt, vl ) ;        
    size++ ;

    s::wstring str( size, 0 ) ;

    ::_vsnwprintf_s( str.data(), size, size, fmt, vl ) ;

    va_end( vl ) ;

    if ( str.size() > 0 ) 
         str.resize( str.size() - 1 ) ;

    ::OutputDebugString( str.c_str() ) ;
}

s::string hlp::format( const char* fmt, ... )
{
    va_list vl ;
    va_start( vl, fmt ) ; 

    auto size = ::_vscprintf( fmt, vl ) ;        
    size++ ;

    s::string str( size, 0 ) ;

    ::vsnprintf_s( str.data(), size, size, fmt, vl ) ;

    va_end( vl ) ;

    if ( str.size() > 0 ) 
         str.resize( str.size() - 1 ) ;

    return str ;
}

s::wstring hlp::format( const WCHAR* fmt, ... )
{
    va_list vl ;
    va_start( vl, fmt ) ; 

    auto size = ::_vscwprintf( fmt, vl ) ;        
    size++ ;

    s::wstring str( size, 0 ) ;

    ::_vsnwprintf_s( str.data(), size, size, fmt, vl ) ;

    va_end( vl ) ;

    if ( str.size() > 0 ) 
         str.resize( str.size() - 1 ) ;

    return str ;
}

s::wstring hlp::a2w( const s::string& utf8 ) // char utf8 to wchar utf16
{
    s::wstring utf16 ;

    if ( utf8.length() == 0 ) return utf16 ;

    auto utf8length = utf8.length() ;

    auto utf16length = ::MultiByteToWideChar(
                            CP_UTF8, 
                            0,        
                            utf8.c_str(),  
                            (int) utf8length,   
                            nullptr,      
                            0 ) ;

    utf16.resize( utf16length ) ;

    ::MultiByteToWideChar(
        CP_UTF8, 
        0, 
        utf8.c_str(),  
        (int) utf8length,   
        (LPWSTR) utf16.c_str(),    
        utf16length ) ;

    return utf16 ;
}
    
s::string hlp::w2a( const s::wstring& utf16 ) // wchar utf16 to char utf8
{
    s::string utf8 ;

    if ( utf16.length() == 0 ) return utf8 ;

    auto utf16length = utf16.length()  ;

    auto utf8length = ::WideCharToMultiByte(
                            CP_UTF8,               
                            0, 
                            utf16.c_str(),       
                            (int) utf16length,        
                            nullptr,            
                            0,                  
                            nullptr, 
                            nullptr ) ;

    if ( utf8length == 0 ) return utf8 ;

    utf8.resize( utf8length ) ;

    ::WideCharToMultiByte(
        CP_UTF8,
        0, 
        utf16.c_str(),   
        (int) utf16length,            
        (LPSTR) utf8.c_str(),
        utf8length,     
        nullptr, 
        nullptr ) ;

    return utf8 ;
}

} } } } } // end namespace


