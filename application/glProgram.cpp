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
#include "glProgram.h"

namespace s = std ;
namespace l = au::com::casaletto::_2020::lib ;

namespace glprogram
{
    glProgram::glProgram()
    {
    }

    void glProgram::create( const char *vertexGlsl, const char* fragmentGlsl )
    {
        _program = ::glCreateProgram() ; 

        auto vs = ::glCreateShader( GL_VERTEX_SHADER ) ;
        ::glShaderSource ( vs, 1, &vertexGlsl, nullptr ) ;
        ::glCompileShader( vs ) ;
        ::glAttachShader ( _program, vs ) ;

        auto fs = ::glCreateShader( GL_FRAGMENT_SHADER ) ; 
        ::glShaderSource ( fs, 1, &fragmentGlsl, nullptr ) ;
        ::glCompileShader( fs ) ;
        ::glAttachShader ( _program, fs ) ;

        ::glLinkProgram( _program ) ;

        ::glGenVertexArrays( 1, &_vao ) ;
        ::glBindVertexArray( _vao ) ;
    }

    glProgram::~glProgram()
    {
    }

    void glProgram::use()
    {
        ::glUseProgram( _program ) ;
    }

} // end ns

