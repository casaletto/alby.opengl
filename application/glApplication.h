#pragma once

namespace me
{
    class glApplication : public au::com::casaletto::_2020::lib::application
    {
        public:
            glApplication( const std::string& title ) ;
            virtual ~glApplication() ;

        protected:
            virtual void on_wm_create    ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
            virtual void on_wm_destroy   ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
            virtual void on_wm_size      ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, const ::RECT& rect ) ;
            virtual void on_idle         ( HWND hwnd ) ;
            //virtual void on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
            virtual void on_wm_paint     ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect ) ;

            virtual void on_gl_create ( HWND hwnd, HDC hdc, HGLRC hglrc ) ;
            virtual void on_gl_destroy( HWND hwnd, HDC hdc, HGLRC hglrc ) ;
            virtual void on_gl_paint  ( HWND hwnd, HDC hdc, HGLRC hglrc, const::RECT& rect, double secondsSinceEpoch ) ;

            HDC   _hdc = nullptr ;
            HGLRC _hrc = nullptr ;

    } ; // end class

} // end ns

