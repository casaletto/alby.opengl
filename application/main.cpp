#include <windows.h>
#include "GL/gl3w.h"

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

#include "hlp.h"
#include "application.h"
#include "a2wApplication.h"
#include "glApplication.h"
#include "pointApplication.h"


namespace s = std ;
namespace l = au::com::casaletto::_2020::lib ;

s::shared_ptr<l::application> getApplication( int argc, WCHAR** argv )  ;

int wmain( int argc, WCHAR** argv )
{
    int rc = 0 ;
    s::cout << "START\n" ;

    try
    {
        auto app = getApplication( argc, argv ) ; 

        app->run( argc, argv, 800, 500 ) ;
    }
    catch( const std::exception& ex )
    {
        rc = 1 ;
        s::cout << "EXCEPTION\n" << ex.what() << "\n" ;
    }

    s::cout << l::hlp::format( "FINISH [%d]\n", rc ) ;
    return rc ;

} // end main

s::shared_ptr<l::application> getApplication( int argc, WCHAR** argv )
{
    s::vector< s::shared_ptr<l::application> > list ;

    list.push_back( s::make_shared<l::application>      ( "application" ) ) ;
    list.push_back( s::make_shared<me::a2wApplication>  ( "a2w"         ) ) ;
    list.push_back( s::make_shared<me::glApplication>   ( "gl"          ) ) ;
    list.push_back( s::make_shared<me::pointApplication>( "point"       ) ) ;

    if ( argc < 2 ) return list[0] ;

    auto appname = l::hlp::w2a( argv[1] ) ;

    auto it = std::find_if( 
        list.begin(), 
        list.end(), 
        [ appname ]( auto &x )  
        { 
            return appname == x->getTitle() ; 
        } ) ; 

    if ( it == list.end() ) return list[0] ;

    return *it ;
}


