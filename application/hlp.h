#pragma once

namespace au {
    namespace com {
        namespace casaletto {
            namespace _2020 {
                namespace lib {

class hlp
{
    public:
        static void   sleep( long ms ) ;
        static double secondsSinceEpoch() ; 

        static std::string  format( const char*  fmt, ... ) ; 
        static std::wstring format( const WCHAR* fmt, ... ) ;

        static std::wstring a2w( const std::string&  utf8  ) ;
        static std::string  w2a( const std::wstring& utf16 ) ;

        static std::wstring readFileUtf16( const std::string& filename ) ;
        static std::string  readFileUtf8 ( const std::string& filename ) ;

        static std::wstring readFileUtf16( const std::filesystem::path& filename ) ;
        static std::string  readFileUtf8 ( const std::filesystem::path& filename ) ;
                
        static std::string exePath() ;

        static void        ods( const std::string&  str ) ;
        static void        ods( const std::wstring& str ) ;
        static void        ods( const char*  fmt, ... ) ;
        static void        ods( const WCHAR* fmt, ... ) ;


    protected:


} ; // end class

} } } } } // end namespace


