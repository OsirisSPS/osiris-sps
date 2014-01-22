// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "buffer.h"
#include "buffer.pypp.hpp"

namespace bp = boost::python;

static boost::python::object assign_3b39bc6fc6d0358a2fa6595935083bd6( ::osiris::Buffer & inst, void const * data, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.assign(data, size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static void clear_7dd54dc4edb48ef9b0a9718a3583ea51( ::osiris::Buffer & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.clear();
    __pythreadSaver.restore();
}

static boost::python::object compare_504d1fa06b30842370ada8e78546e462( ::osiris::Buffer const & inst, void const * data, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.compare(data, size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object empty_b4060f3683fcc964ff020dabce1a44d3( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.empty();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object eraseOffset_73b7b695fc905ede8640a2907af7c9aa( ::osiris::Buffer & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.eraseOffset();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object find_b5fe5cd0f310ac5871271b83bb44e10b( ::osiris::Buffer const & inst, ::osiris::byte const * value, ::osiris::uint32 length, ::osiris::byte const * pos=(nullptr) ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::byte const * result = inst.find(value, length, pos);
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::return_opaque_pointer > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::byte const * >( result ) );
}

static boost::python::object getAllocEndPosition_abba92b7dec361c742848e61943f40ff( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::byte * result = inst.getAllocEndPosition();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::return_opaque_pointer > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::byte * >( result ) );
}

static boost::python::object getAllocSize_3b853470f83a830cbc2c394280750917( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.getAllocSize();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object getAvailable_a1a306523075a3ccd8e17e0c28e33bea( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.getAvailable();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object getData_e3922238d6a6a9f7b298be3e3039b5bd( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::byte * result = inst.getData();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::return_opaque_pointer > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::byte * >( result ) );
}

static boost::python::object getEndPosition_7c12e10fde18fd83c4e8833e7d406006( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::byte * result = inst.getEndPosition();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::return_opaque_pointer > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::byte * >( result ) );
}

static boost::python::object getGrowSize_f70ce21f6e8d65846d495a5ca66f59ad( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.getGrowSize();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object getOffset_a3f939a4bc621273cfb1a3fa93c6f1e3( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.getOffset();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object getPosition_93f5cb952e85f85ba81844e72b9f4709( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::byte * result = inst.getPosition();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::return_opaque_pointer > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::byte * >( result ) );
}

static boost::python::object getSize_a513cff3aa8bff6baf4f7caca4295a85( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.getSize();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object isReadable_73152373fa3bfacf6e9ee406579118d3( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.isReadable();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object peek_73c8e1cf7ea99ccac1d593de5e2d4977( ::osiris::Buffer const & inst, void * data, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.peek(data, size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object put_5c61aba90d302c7e98950e4fa7c16175( ::osiris::Buffer & inst, void const * data, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.put(data, size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object read_362eaf768b599acaa11b1e8f2c7ce44f( ::osiris::Buffer const & inst, void * data, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.read(data, size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object seekToBegin_34f028343a51764372eed0eff222d0cd( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.seekToBegin();
    __pythreadSaver.restore();
    return boost::python::object(result);
}

static boost::python::object seekToEnd_fd54aab1cfd3d0dc08fb0b3570160057( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.seekToEnd();
    __pythreadSaver.restore();
    return boost::python::object(result);
}

static boost::python::object toBase64_0ba5df3ebde0725463ad5f8cf03a5a7e( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::std::string result = inst.toBase64();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object toHex_a8d36de9ba543845ce0f00bb089d0fa0( ::osiris::Buffer const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::std::string result = inst.toHex();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object write_27479837d133dec89eab9e0b0bb9c4d7( ::osiris::Buffer & inst, void const * data, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.write(data, size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object erase_22f022def07fd519cd1215acac51e863( ::osiris::Buffer & inst, ::osiris::uint32 pos, ::osiris::uint32 count=::osiris::uint32(::osiris::Buffer::npos) ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.erase(pos, count);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object fromBase64_b7b6076c75ae663526526a8a18a40679( ::osiris::Buffer & inst, ::std::string const & str ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.fromBase64(str);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object fromHex_615bfeea84b2c02e78ff03ec205d774c( ::osiris::Buffer & inst, ::std::string const & str ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.fromHex(str);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object getAt_ae30572d73a3a4f5d67b14a39d2c6cef( ::osiris::Buffer const & inst, ::osiris::uint32 position ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::byte result = inst.getAt(position);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object grow_49e74e98c405b4e11a9a5b5893cef4e5( ::osiris::Buffer & inst, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.grow(size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object load_6f09a333b243f905ac067fd3b8ebf1d2( ::osiris::Buffer & inst, ::osiris::String const & filename ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.load(filename);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object load_aee6543d4e383d5046e90b5536861d95( ::osiris::Buffer & inst, ::boost::shared_ptr< const osiris::IStream > stream ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.load(stream);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object readBuffer_a69484f435b76cacc0f6ef0c448d0bac( ::osiris::Buffer const & inst, ::osiris::Buffer & buff ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.readBuffer(buff);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object readCount_bd8fd95a7b2737524e413e990f448991( ::osiris::Buffer const & inst, ::osiris::uint32 & count ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.readCount(count);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object readString_95568b3565c35dd03081a2d22234ccb1( ::osiris::Buffer const & inst, ::osiris::String & str ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.readString(str);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object reserve_f13e01c5179128dcb3fc9af7163a2afc( ::osiris::Buffer & inst, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.reserve(size);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object save_0f519d25eac96e9822c3f5cb7fecb8b4( ::osiris::Buffer const & inst, ::osiris::String const & filename ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.save(filename);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object save_f01626cf509b880a126f73add69c1e64( ::osiris::Buffer const & inst, ::boost::shared_ptr< osiris::IStream > stream ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.save(stream);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object seekAt_59519873defd8a0aeedfeab821b4fa53( ::osiris::Buffer const & inst, ::osiris::byte * position ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.seekAt(position);
    __pythreadSaver.restore();
    return boost::python::object(result);
}

static boost::python::object seekAt_211e37fab155184f728f63f808eccf67( ::osiris::Buffer const & inst, ::osiris::uint32 position ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.seekAt(position);
    __pythreadSaver.restore();
    return boost::python::object(result);
}

static boost::python::object seekOffset_148c3170794cc36d6ec2939f667c7eb2( ::osiris::Buffer const & inst, ::osiris::uint32 offset ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.seekOffset(offset);
    __pythreadSaver.restore();
    return boost::python::object(result);
}

static void setGrowSize_570127840a440752c44e5223ed1fc60d( ::osiris::Buffer & inst, ::osiris::uint32 size ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.setGrowSize(size);
    __pythreadSaver.restore();
}

static boost::python::object writeBuffer_cc0d0d14cdc91f54da1c63bcb907ece3( ::osiris::Buffer & inst, ::osiris::Buffer const & buff ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.writeBuffer(buff);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object writeCount_fab84c045632889ae96f17de308c0880( ::osiris::Buffer & inst, ::osiris::uint32 count ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.writeCount(count);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object writeString_81cc16ffcc81be891f14950ff4bb8f55( ::osiris::Buffer & inst, ::osiris::String const & str ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.writeString(str);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object writeString_4a13c7d4e67c5130fe0e7b515cebc8d0( ::osiris::Buffer & inst, ::osiris::uchar const * str ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.writeString(str);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

void register_Buffer_class(){

    { //::osiris::Buffer
        typedef ::boost::python::class_< ::osiris::Buffer > Buffer_exposer_t;
        Buffer_exposer_t Buffer_exposer = Buffer_exposer_t( "Buffer", ::boost::python::init< ::boost::python::optional< ::osiris::uint32 > >(( ::boost::python::arg("growSize")=(::osiris::uint32)(1024) )) );
        ::boost::python::scope Buffer_scope( Buffer_exposer );
        ::boost::python::scope().attr("npos") = (int)::osiris::Buffer::npos;
        ::boost::python::implicitly_convertible< ::osiris::uint32, ::osiris::Buffer >();
        Buffer_exposer.def( ::boost::python::init< ::osiris::Buffer const & >(( ::boost::python::arg("second") )) );
        { //::osiris::Buffer::assign
        
            typedef boost::python::object ( *assign_function_type )( ::osiris::Buffer &,void const *,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "assign"
                , assign_function_type( &assign_3b39bc6fc6d0358a2fa6595935083bd6 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("data"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::clear
        
            typedef void ( *clear_function_type )( ::osiris::Buffer & );
            
            Buffer_exposer.def( 
                "clear"
                , clear_function_type( &clear_7dd54dc4edb48ef9b0a9718a3583ea51 ) );
        
        }
        { //::osiris::Buffer::compare
        
            typedef boost::python::object ( *compare_function_type )( ::osiris::Buffer const &,void const *,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "compare"
                , compare_function_type( &compare_504d1fa06b30842370ada8e78546e462 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("data"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::empty
        
            typedef boost::python::object ( *empty_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "empty"
                , empty_function_type( &empty_b4060f3683fcc964ff020dabce1a44d3 ) );
        
        }
        { //::osiris::Buffer::eraseOffset
        
            typedef boost::python::object ( *eraseOffset_function_type )( ::osiris::Buffer & );
            
            Buffer_exposer.def( 
                "eraseOffset"
                , eraseOffset_function_type( &eraseOffset_73b7b695fc905ede8640a2907af7c9aa ) );
        
        }
        { //::osiris::Buffer::find
        
            typedef boost::python::object ( *find_function_type )( ::osiris::Buffer const &,::osiris::byte const *,::osiris::uint32,::osiris::byte const * );
            
            Buffer_exposer.def( 
                "find"
                , find_function_type( &find_b5fe5cd0f310ac5871271b83bb44e10b )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("value"), ::boost::python::arg("length"), ::boost::python::arg("pos")=(nullptr) ) );
        
        }
        { //::osiris::Buffer::getAllocEndPosition
        
            typedef boost::python::object ( *getAllocEndPosition_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getAllocEndPosition"
                , getAllocEndPosition_function_type( &getAllocEndPosition_abba92b7dec361c742848e61943f40ff ) );
        
        }
        { //::osiris::Buffer::getAllocSize
        
            typedef boost::python::object ( *getAllocSize_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getAllocSize"
                , getAllocSize_function_type( &getAllocSize_3b853470f83a830cbc2c394280750917 ) );
        
        }
        { //::osiris::Buffer::getAvailable
        
            typedef boost::python::object ( *getAvailable_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getAvailable"
                , getAvailable_function_type( &getAvailable_a1a306523075a3ccd8e17e0c28e33bea ) );
        
        }
        { //::osiris::Buffer::getData
        
            typedef boost::python::object ( *getData_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getData"
                , getData_function_type( &getData_e3922238d6a6a9f7b298be3e3039b5bd ) );
        
        }
        { //::osiris::Buffer::getEndPosition
        
            typedef boost::python::object ( *getEndPosition_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getEndPosition"
                , getEndPosition_function_type( &getEndPosition_7c12e10fde18fd83c4e8833e7d406006 ) );
        
        }
        { //::osiris::Buffer::getGrowSize
        
            typedef boost::python::object ( *getGrowSize_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getGrowSize"
                , getGrowSize_function_type( &getGrowSize_f70ce21f6e8d65846d495a5ca66f59ad ) );
        
        }
        { //::osiris::Buffer::getOffset
        
            typedef boost::python::object ( *getOffset_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getOffset"
                , getOffset_function_type( &getOffset_a3f939a4bc621273cfb1a3fa93c6f1e3 ) );
        
        }
        { //::osiris::Buffer::getPosition
        
            typedef boost::python::object ( *getPosition_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getPosition"
                , getPosition_function_type( &getPosition_93f5cb952e85f85ba81844e72b9f4709 ) );
        
        }
        { //::osiris::Buffer::getSize
        
            typedef boost::python::object ( *getSize_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "getSize"
                , getSize_function_type( &getSize_a513cff3aa8bff6baf4f7caca4295a85 ) );
        
        }
        { //::osiris::Buffer::isReadable
        
            typedef boost::python::object ( *isReadable_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "isReadable"
                , isReadable_function_type( &isReadable_73152373fa3bfacf6e9ee406579118d3 ) );
        
        }
        { //::osiris::Buffer::peek
        
            typedef boost::python::object ( *peek_function_type )( ::osiris::Buffer const &,void *,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "peek"
                , peek_function_type( &peek_73c8e1cf7ea99ccac1d593de5e2d4977 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("data"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::put
        
            typedef boost::python::object ( *put_function_type )( ::osiris::Buffer &,void const *,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "put"
                , put_function_type( &put_5c61aba90d302c7e98950e4fa7c16175 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("data"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::read
        
            typedef boost::python::object ( *read_function_type )( ::osiris::Buffer const &,void *,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "read"
                , read_function_type( &read_362eaf768b599acaa11b1e8f2c7ce44f )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("data"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::seekToBegin
        
            typedef boost::python::object ( *seekToBegin_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "seekToBegin"
                , seekToBegin_function_type( &seekToBegin_34f028343a51764372eed0eff222d0cd ) );
        
        }
        { //::osiris::Buffer::seekToEnd
        
            typedef boost::python::object ( *seekToEnd_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "seekToEnd"
                , seekToEnd_function_type( &seekToEnd_fd54aab1cfd3d0dc08fb0b3570160057 ) );
        
        }
        { //::osiris::Buffer::toBase64
        
            typedef boost::python::object ( *toBase64_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "toBase64"
                , toBase64_function_type( &toBase64_0ba5df3ebde0725463ad5f8cf03a5a7e ) );
        
        }
        { //::osiris::Buffer::toHex
        
            typedef boost::python::object ( *toHex_function_type )( ::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "toHex"
                , toHex_function_type( &toHex_a8d36de9ba543845ce0f00bb089d0fa0 ) );
        
        }
        { //::osiris::Buffer::write
        
            typedef boost::python::object ( *write_function_type )( ::osiris::Buffer &,void const *,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "write"
                , write_function_type( &write_27479837d133dec89eab9e0b0bb9c4d7 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("data"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::erase
        
            typedef boost::python::object ( *erase_function_type )( ::osiris::Buffer &,::osiris::uint32,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "erase"
                , erase_function_type( &erase_22f022def07fd519cd1215acac51e863 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("pos"), ::boost::python::arg("count")=(::osiris::uint32)(::osiris::uint32(::osiris::Buffer::npos)) ) );
        
        }
        { //::osiris::Buffer::fromBase64
        
            typedef boost::python::object ( *fromBase64_function_type )( ::osiris::Buffer &,::std::string const & );
            
            Buffer_exposer.def( 
                "fromBase64"
                , fromBase64_function_type( &fromBase64_b7b6076c75ae663526526a8a18a40679 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("str") ) );
        
        }
        { //::osiris::Buffer::fromHex
        
            typedef boost::python::object ( *fromHex_function_type )( ::osiris::Buffer &,::std::string const & );
            
            Buffer_exposer.def( 
                "fromHex"
                , fromHex_function_type( &fromHex_615bfeea84b2c02e78ff03ec205d774c )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("str") ) );
        
        }
        { //::osiris::Buffer::getAt
        
            typedef boost::python::object ( *getAt_function_type )( ::osiris::Buffer const &,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "getAt"
                , getAt_function_type( &getAt_ae30572d73a3a4f5d67b14a39d2c6cef )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("position") ) );
        
        }
        { //::osiris::Buffer::grow
        
            typedef boost::python::object ( *grow_function_type )( ::osiris::Buffer &,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "grow"
                , grow_function_type( &grow_49e74e98c405b4e11a9a5b5893cef4e5 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::load
        
            typedef boost::python::object ( *load_function_type )( ::osiris::Buffer &,::osiris::String const & );
            
            Buffer_exposer.def( 
                "load"
                , load_function_type( &load_6f09a333b243f905ac067fd3b8ebf1d2 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("filename") ) );
        
        }
        { //::osiris::Buffer::load
        
            typedef boost::python::object ( *load_function_type )( ::osiris::Buffer &,::boost::shared_ptr<const osiris::IStream> );
            
            Buffer_exposer.def( 
                "load"
                , load_function_type( &load_aee6543d4e383d5046e90b5536861d95 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("stream") ) );
        
        }
        { //::osiris::Buffer::operator=
        
            typedef ::osiris::Buffer & ( ::osiris::Buffer::*assign_function_type )( ::osiris::Buffer const & ) ;
            
            Buffer_exposer.def( 
                "assign"
                , assign_function_type( &::osiris::Buffer::operator= )
                , ( ::boost::python::arg("second") )
                , bp::return_self< >() );
        
        }
        Buffer_exposer.def( ::boost::python::self == ::boost::python::self );
        { //::osiris::Buffer::readBuffer
        
            typedef boost::python::object ( *readBuffer_function_type )( ::osiris::Buffer const &,::osiris::Buffer & );
            
            Buffer_exposer.def( 
                "readBuffer"
                , readBuffer_function_type( &readBuffer_a69484f435b76cacc0f6ef0c448d0bac )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("buff") ) );
        
        }
        { //::osiris::Buffer::readCount
        
            typedef boost::python::object ( *readCount_function_type )( ::osiris::Buffer const &,::osiris::uint32 & );
            
            Buffer_exposer.def( 
                "readCount"
                , readCount_function_type( &readCount_bd8fd95a7b2737524e413e990f448991 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("count") ) );
        
        }
        { //::osiris::Buffer::readString
        
            typedef boost::python::object ( *readString_function_type )( ::osiris::Buffer const &,::osiris::String & );
            
            Buffer_exposer.def( 
                "readString"
                , readString_function_type( &readString_95568b3565c35dd03081a2d22234ccb1 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("str") ) );
        
        }
        { //::osiris::Buffer::reserve
        
            typedef boost::python::object ( *reserve_function_type )( ::osiris::Buffer &,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "reserve"
                , reserve_function_type( &reserve_f13e01c5179128dcb3fc9af7163a2afc )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::save
        
            typedef boost::python::object ( *save_function_type )( ::osiris::Buffer const &,::osiris::String const & );
            
            Buffer_exposer.def( 
                "save"
                , save_function_type( &save_0f519d25eac96e9822c3f5cb7fecb8b4 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("filename") ) );
        
        }
        { //::osiris::Buffer::save
        
            typedef boost::python::object ( *save_function_type )( ::osiris::Buffer const &,::boost::shared_ptr<osiris::IStream> );
            
            Buffer_exposer.def( 
                "save"
                , save_function_type( &save_f01626cf509b880a126f73add69c1e64 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("stream") ) );
        
        }
        { //::osiris::Buffer::seekAt
        
            typedef boost::python::object ( *seekAt_function_type )( ::osiris::Buffer const &,::osiris::byte * );
            
            Buffer_exposer.def( 
                "seekAt"
                , seekAt_function_type( &seekAt_59519873defd8a0aeedfeab821b4fa53 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("position") ) );
        
        }
        { //::osiris::Buffer::seekAt
        
            typedef boost::python::object ( *seekAt_function_type )( ::osiris::Buffer const &,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "seekAt"
                , seekAt_function_type( &seekAt_211e37fab155184f728f63f808eccf67 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("position") ) );
        
        }
        { //::osiris::Buffer::seekOffset
        
            typedef boost::python::object ( *seekOffset_function_type )( ::osiris::Buffer const &,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "seekOffset"
                , seekOffset_function_type( &seekOffset_148c3170794cc36d6ec2939f667c7eb2 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("offset") ) );
        
        }
        { //::osiris::Buffer::setGrowSize
        
            typedef void ( *setGrowSize_function_type )( ::osiris::Buffer &,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "setGrowSize"
                , setGrowSize_function_type( &setGrowSize_570127840a440752c44e5223ed1fc60d )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("size") ) );
        
        }
        { //::osiris::Buffer::writeBuffer
        
            typedef boost::python::object ( *writeBuffer_function_type )( ::osiris::Buffer &,::osiris::Buffer const & );
            
            Buffer_exposer.def( 
                "writeBuffer"
                , writeBuffer_function_type( &writeBuffer_cc0d0d14cdc91f54da1c63bcb907ece3 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("buff") ) );
        
        }
        { //::osiris::Buffer::writeCount
        
            typedef boost::python::object ( *writeCount_function_type )( ::osiris::Buffer &,::osiris::uint32 );
            
            Buffer_exposer.def( 
                "writeCount"
                , writeCount_function_type( &writeCount_fab84c045632889ae96f17de308c0880 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("count") ) );
        
        }
        { //::osiris::Buffer::writeString
        
            typedef boost::python::object ( *writeString_function_type )( ::osiris::Buffer &,::osiris::String const & );
            
            Buffer_exposer.def( 
                "writeString"
                , writeString_function_type( &writeString_81cc16ffcc81be891f14950ff4bb8f55 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("str") ) );
        
        }
        { //::osiris::Buffer::writeString
        
            typedef boost::python::object ( *writeString_function_type )( ::osiris::Buffer &,::osiris::uchar const * );
            
            Buffer_exposer.def( 
                "writeString"
                , writeString_function_type( &writeString_4a13c7d4e67c5130fe0e7b515cebc8d0 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("str") ) );
        
        }
        Buffer_exposer.def_readonly( "EMPTY", ::osiris::Buffer::EMPTY );
        { //property "growSize"[fget=::osiris::Buffer::getGrowSize, fset=::osiris::Buffer::setGrowSize]
        
            typedef ::osiris::uint32 ( ::osiris::Buffer::*fget )(  ) const;
            typedef void ( ::osiris::Buffer::*fset )( ::osiris::uint32 ) ;
            
            Buffer_exposer.add_property( 
                "growSize"
                , fget( &::osiris::Buffer::getGrowSize )
                , fset( &::osiris::Buffer::setGrowSize )
                , "get\\set property, built on top of \"osiris::uint32 osiris::Buffer::getGrowSize() const [member function]\" and \"void osiris::Buffer::setGrowSize(osiris::uint32 size) [member function]\"" );
        
        }
        { //property "data"[fget=::osiris::Buffer::getData]
        
            typedef ::osiris::byte * ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "data"
                , ::boost::python::make_function( 
                      fget( &::osiris::Buffer::getData )
                    , bp::return_value_policy< bp::return_opaque_pointer >() ) 
                , "get property, built on top of \"osiris::byte * osiris::Buffer::getData() const [member function]\"" );
        
        }
        { //property "position"[fget=::osiris::Buffer::getPosition]
        
            typedef ::osiris::byte * ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "position"
                , ::boost::python::make_function( 
                      fget( &::osiris::Buffer::getPosition )
                    , bp::return_value_policy< bp::return_opaque_pointer >() ) 
                , "get property, built on top of \"osiris::byte * osiris::Buffer::getPosition() const [member function]\"" );
        
        }
        { //property "endPosition"[fget=::osiris::Buffer::getEndPosition]
        
            typedef ::osiris::byte * ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "endPosition"
                , ::boost::python::make_function( 
                      fget( &::osiris::Buffer::getEndPosition )
                    , bp::return_value_policy< bp::return_opaque_pointer >() ) 
                , "get property, built on top of \"osiris::byte * osiris::Buffer::getEndPosition() const [member function]\"" );
        
        }
        { //property "allocEndPosition"[fget=::osiris::Buffer::getAllocEndPosition]
        
            typedef ::osiris::byte * ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "allocEndPosition"
                , ::boost::python::make_function( 
                      fget( &::osiris::Buffer::getAllocEndPosition )
                    , bp::return_value_policy< bp::return_opaque_pointer >() ) 
                , "get property, built on top of \"osiris::byte * osiris::Buffer::getAllocEndPosition() const [member function]\"" );
        
        }
        { //property "available"[fget=::osiris::Buffer::getAvailable]
        
            typedef ::osiris::uint32 ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "available"
                , fget( &::osiris::Buffer::getAvailable )
                , "get property, built on top of \"osiris::uint32 osiris::Buffer::getAvailable() const [member function]\"" );
        
        }
        { //property "offset"[fget=::osiris::Buffer::getOffset]
        
            typedef ::osiris::uint32 ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "offset"
                , fget( &::osiris::Buffer::getOffset )
                , "get property, built on top of \"osiris::uint32 osiris::Buffer::getOffset() const [member function]\"" );
        
        }
        { //property "size"[fget=::osiris::Buffer::getSize]
        
            typedef ::osiris::uint32 ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "size"
                , fget( &::osiris::Buffer::getSize )
                , "get property, built on top of \"osiris::uint32 osiris::Buffer::getSize() const [member function]\"" );
        
        }
        { //property "allocSize"[fget=::osiris::Buffer::getAllocSize]
        
            typedef ::osiris::uint32 ( ::osiris::Buffer::*fget )(  ) const;
            
            Buffer_exposer.add_property( 
                "allocSize"
                , fget( &::osiris::Buffer::getAllocSize )
                , "get property, built on top of \"osiris::uint32 osiris::Buffer::getAllocSize() const [member function]\"" );
        
        }
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::Buffer > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::Buffer >, boost::shared_ptr< ::osiris::Object > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::Buffer >, boost::shared_ptr< ::boost::equality_comparable< osiris::Buffer, osiris::Buffer, boost::detail::empty_base< osiris::Buffer >, boost::detail::false_t > > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::Buffer >, boost::shared_ptr< ::boost::equality_comparable1< osiris::Buffer, boost::detail::empty_base< osiris::Buffer > > > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::Buffer >, boost::shared_ptr< ::boost::detail::empty_base< osiris::Buffer > > >();
    }

}
