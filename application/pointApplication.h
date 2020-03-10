#pragma once

namespace me
{
    class pointApplication : public glApplication
    {
        public:
            pointApplication( const std::string& title ) ;
            virtual ~pointApplication() ;

        protected:
            virtual void on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
            virtual void on_gl_create    ( HWND hwnd, HDC hdc, HGLRC hglrc ) ;
            virtual void on_gl_destroy   ( HWND hwnd, HDC hdc, HGLRC hglrc ) ;
            virtual void on_gl_paint     ( HWND hwnd, HDC hdc, HGLRC hglrc, const::RECT& rect, double secondsSinceEpoch ) ;

            long _i = 0 ;
            int _pixelSize = 10 ;

            glprogram::glProgram _program ;

            static const char* _fragmentGsls ;
            static const char* _vertexGsls   ;


    } ; // end class

} // end ns

