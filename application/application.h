#pragma once

namespace au {
    namespace com {
        namespace casaletto {
            namespace _2020 {
                namespace lib {

class application
{
    protected:
        std::string               _title   ;
        std::vector<std::string>  _args    ;
        std::string               _exePath ;

        static application*       _application ;
        static std::string        _exception   ;

        static LRESULT WINAPI wndProc( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;

        void messageLoop( HWND hwnd ) ;

        virtual void on_wm_paint     ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, HDC hdc, const ::RECT& rect ) ;
        virtual void on_wm_size      ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, const ::RECT& rect ) ;
        virtual void on_wm_close     ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
        virtual void on_wm_destroy   ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
        virtual void on_wm_keydown   ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
        virtual void on_wm_create    ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
        virtual void on_wm_mousewheel( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;
        virtual void on_wm_char      ( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam ) ;

        virtual void on_idle( HWND hwnd, double secondsSinceEpoch ) ; 

    public:
        application( const std::string& title ) ;
        virtual ~application() ;

        virtual void run( int argc, WCHAR** argv, int width, int height ) ;

        virtual std::string getTitle() ;

} ; // end class

} } } } } // end namespace


