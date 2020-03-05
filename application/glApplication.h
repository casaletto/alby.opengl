#pragma once

namespace me
{
    class glApplication : public au::com::casaletto::_2020::lib::application
    {
        public:
            glApplication( const std::string& title ) ;
            virtual ~glApplication() ;

        protected:
            void on_wm_paint     ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect ) ;
            void on_wm_create    ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
            void on_wm_destroy   ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
            void on_wm_size      ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, const ::RECT& rect ) ;
            void on_idle         ( HWND hwnd, double secondsSinceEpoch ) ;
            void on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;

            HDC   _hdc = nullptr ;
            HGLRC _hrc = nullptr ;

            GLuint _program = 0 ;
            GLuint _vao     = 0 ;

            long long _i         = 0  ;
            int       _pixelSize = 10 ;

    } ; // end class

} // end ns

