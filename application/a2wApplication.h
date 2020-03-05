#pragma once

namespace me
{
    class a2wApplication : public au::com::casaletto::_2020::lib::application
    {

        public:
            a2wApplication( const std::string& title ) ;
            virtual ~a2wApplication() ;

        protected:
            void on_wm_paint     ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect ) ;
            void on_wm_create    ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
            void on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;

            std::wstring _wstr1 ;
            std::wstring _wstr2 ;
            int          _fontsize ;

    } ; // end class

} // end ns

