/* -*- mode: c++ -*- */
/**
 * @file   Tools.h
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Nov 2008
 * 
 * @brief  
 * @copyright
 */
#ifndef _BOARD_TOOLS_H_
#define _BOARD_TOOLS_H_

#include <iostream>

namespace LibBoard {

  class MessageStream {
  public:

    inline MessageStream( std::ostream & out, const char * prefix  );
    
    template<typename T>  inline MessageStream operator<<( const T & v );
    
  private:
    std::ostream & _out;    
    const char * _prefix;
  };
  
  extern MessageStream error;
  extern MessageStream warning;
  extern MessageStream notice;
  
  MessageStream::MessageStream( std::ostream & out, const char * prefix )
    : _out( out ),
      _prefix( prefix )
  {
  }

  template<typename T> 
  MessageStream MessageStream::operator<<( const T & v )
  {
    if ( _prefix )
      _out << _prefix << v;
    else
      _out << v;
    return MessageStream( _out, 0 );
  }
}

#endif /* _SHAPE_H_ */

