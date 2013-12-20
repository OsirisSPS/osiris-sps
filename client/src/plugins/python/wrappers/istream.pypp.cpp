// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "istream.h"
#include "file.h"
#include "textfile.h"
#include "istream.pypp.hpp"

namespace bp = boost::python;

struct IStream_wrapper : ::osiris::IStream, ::osiris::PythonWrapper< ::osiris::IStream > {

    IStream_wrapper( )
    : ::osiris::IStream( )
      , ::osiris::PythonWrapper< ::osiris::IStream >(){
        // nullptr constructor
    
    }

    static boost::python::object empty( ::osiris::IStream const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.empty();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object available( ::osiris::IStream const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::uint64 result = inst.available();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object seekToBegin( ::osiris::IStream & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.seekToBegin();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object seekToEnd( ::osiris::IStream & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.seekToEnd();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual ::osiris::uint32 peek( void * v, ::osiris::uint32 size ) const  {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_peek = this->get_override( "peek" ) )
            return func_peek( v, size );
        else{
            __pystate.leave();
            return this->::osiris::IStream::peek( v, size );
        }
    }
    
    ::osiris::uint32 default_peek( void * v, ::osiris::uint32 size ) const  {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::IStream::peek( v, size );
    }

    virtual bool is_open(  ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_is_open = this->get_override( "is_open" );
        return func_is_open(  );
    }

    virtual bool close(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_close = this->get_override( "close" );
        return func_close(  );
    }

    virtual ::osiris::uint32 read( void * v, ::osiris::uint32 size ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_read = this->get_override( "read" );
        return func_read( v, size );
    }

    virtual ::osiris::uint32 write( void const * v, ::osiris::uint32 size ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_write = this->get_override( "write" );
        return func_write( v, size );
    }

    virtual bool seek( ::osiris::uint64 offset, ::osiris::SeekPosition from ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_seek = this->get_override( "seek" );
        return func_seek( offset, from );
    }

    virtual ::osiris::uint64 position(  ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_position = this->get_override( "position" );
        return func_position(  );
    }

    virtual ::osiris::uint64 size(  ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_size = this->get_override( "size" );
        return func_size(  );
    }

    virtual bool flush(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_flush = this->get_override( "flush" );
        return func_flush(  );
    }

    virtual bool eof(  ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_eof = this->get_override( "eof" );
        return func_eof(  );
    }

};

void register_IStream_class(){

    { //::osiris::IStream
        typedef ::boost::python::class_< IStream_wrapper, ::boost::noncopyable > IStream_exposer_t;
        IStream_exposer_t IStream_exposer = IStream_exposer_t( "IStream", ::boost::python::init< >() );
        ::boost::python::scope IStream_scope( IStream_exposer );
        { //::osiris::IStream::empty
        
            typedef boost::python::object ( *empty_function_type )( ::osiris::IStream const & );
            
            IStream_exposer.def( 
                "empty"
                , empty_function_type( &IStream_wrapper::empty ) );
        
        }
        { //::osiris::IStream::available
        
            typedef boost::python::object ( *available_function_type )( ::osiris::IStream const & );
            
            IStream_exposer.def( 
                "available"
                , available_function_type( &IStream_wrapper::available ) );
        
        }
        { //::osiris::IStream::seekToBegin
        
            typedef boost::python::object ( *seekToBegin_function_type )( ::osiris::IStream & );
            
            IStream_exposer.def( 
                "seekToBegin"
                , seekToBegin_function_type( &IStream_wrapper::seekToBegin ) );
        
        }
        { //::osiris::IStream::seekToEnd
        
            typedef boost::python::object ( *seekToEnd_function_type )( ::osiris::IStream & );
            
            IStream_exposer.def( 
                "seekToEnd"
                , seekToEnd_function_type( &IStream_wrapper::seekToEnd ) );
        
        }
        { //::osiris::IStream::peek
        
            typedef ::osiris::uint32 ( ::osiris::IStream::*peek_function_type )( void *,::osiris::uint32 ) const;
            typedef ::osiris::uint32 ( IStream_wrapper::*default_peek_function_type )( void *,::osiris::uint32 ) const;
            
            IStream_exposer.def( 
                "peek"
                , peek_function_type(&::osiris::IStream::peek)
                , default_peek_function_type(&IStream_wrapper::default_peek)
                , ( ::boost::python::arg("v"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::IStream::is_open
        
            typedef bool ( ::osiris::IStream::*is_open_function_type )(  ) const;
            
            IStream_exposer.def( 
                "is_open"
                , ( is_open_function_type(&::osiris::IStream::is_open) ) );
        
        }
        { //::osiris::IStream::close
        
            typedef bool ( ::osiris::IStream::*close_function_type )(  ) ;
            
            IStream_exposer.def( 
                "close"
                , ( close_function_type(&::osiris::IStream::close) ) );
        
        }
        { //::osiris::IStream::read
        
            typedef ::osiris::uint32 ( ::osiris::IStream::*read_function_type )( void *,::osiris::uint32 ) const;
            
            IStream_exposer.def( 
                "read"
                , ( read_function_type(&::osiris::IStream::read) )
                , ( ::boost::python::arg("v"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::IStream::write
        
            typedef ::osiris::uint32 ( ::osiris::IStream::*write_function_type )( void const *,::osiris::uint32 ) ;
            
            IStream_exposer.def( 
                "write"
                , ( write_function_type(&::osiris::IStream::write) )
                , ( ::boost::python::arg("v"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::IStream::seek
        
            typedef bool ( ::osiris::IStream::*seek_function_type )( ::osiris::uint64,::osiris::SeekPosition ) const;
            
            IStream_exposer.def( 
                "seek"
                , ( seek_function_type(&::osiris::IStream::seek) )
                , ( ::boost::python::arg("offset"), ::boost::python::arg("from") ) );
        
        }
        { //::osiris::IStream::position
        
            typedef ::osiris::uint64 ( ::osiris::IStream::*position_function_type )(  ) const;
            
            IStream_exposer.def( 
                "position"
                , ( position_function_type(&::osiris::IStream::position) ) );
        
        }
        { //::osiris::IStream::size
        
            typedef ::osiris::uint64 ( ::osiris::IStream::*size_function_type )(  ) const;
            
            IStream_exposer.def( 
                "size"
                , ( size_function_type(&::osiris::IStream::size) ) );
        
        }
        { //::osiris::IStream::flush
        
            typedef bool ( ::osiris::IStream::*flush_function_type )(  ) ;
            
            IStream_exposer.def( 
                "flush"
                , ( flush_function_type(&::osiris::IStream::flush) ) );
        
        }
        { //::osiris::IStream::eof
        
            typedef bool ( ::osiris::IStream::*eof_function_type )(  ) const;
            
            IStream_exposer.def( 
                "eof"
                , ( eof_function_type(&::osiris::IStream::eof) ) );
        
        }
        IStream_exposer.def_readonly( "npos", ::osiris::IStream::npos );
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::IStream > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::IStream >, boost::shared_ptr< ::boost::noncopyable_::noncopyable > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::IStream >, boost::shared_ptr< ::osiris::Object > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::File >, boost::shared_ptr< ::osiris::IStream > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::TextFile >, boost::shared_ptr< ::osiris::IStream > >();
    }

}
