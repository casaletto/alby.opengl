#pragma once

namespace glprogram
{
	class glProgram
	{
		public:
			glProgram() ;
			virtual ~glProgram() ;

			virtual void create( const char* vertexGlsl, const char* fragmentGlsl ) ;
			virtual void use() ;

		protected:
			GLuint _program = 0 ;
			GLuint _vao     = 0 ;


	} ; // end class

} // end ns

