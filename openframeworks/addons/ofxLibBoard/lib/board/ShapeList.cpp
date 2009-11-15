/* -*- mode: c++ -*- */
/**
 * @file   ShapeList.cpp
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  Definition of the ShapeList and Group classes.
 * 
 * @copyright
 * This source code is part of the Board project, a C++ library whose
 * purpose is to allow simple drawings in EPS, FIG or SVG files.
 * Copyright (C) 2007 Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 * This source code is part of the Board project, a C++ library whose
 * purpose is to allow simple drawings in EPS, FIG or SVG files.
 * Copyright (C) 2007 Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */
#include "ShapeList.h"
#include <algorithm>
#include <typeinfo>
#include "Tools.h"

namespace LibBoard {

  //
  // Definition of the ShapeList methods.
  //

const std::string ShapeList::_name("ListOfShapes");

const std::string &
ShapeList::name() const
{
  return _name;
}

ShapeList::~ShapeList()
{
  free();
}

ShapeList &
ShapeList::clear()
{
  free();
  _shapes.clear();
  _nextDepth = std::numeric_limits<int>::max() - 1;
  return *this;
}

void
ShapeList::free()
{
  std::vector<Shape*>::const_iterator i = _shapes.begin();
  std::vector<Shape*>::const_iterator end = _shapes.end();
  while ( i != end ) {
    delete *i;
    ++i;
  }  
}

ShapeList::ShapeList( const ShapeList & other ) : Shape( other )
{
  if ( ! other._shapes.size() ) return;  
  _shapes.resize( other._shapes.size(), 0 );
  std::vector<Shape*>::iterator t = _shapes.begin();
  std::vector<Shape*>::const_iterator i = other._shapes.begin();
  std::vector<Shape*>::const_iterator end = other._shapes.end();
  while ( i != end ) {
    *t = (*i)->clone();
    ++i; ++t;
  }
}

ShapeList &
ShapeList::operator=( const ShapeList & other )
{
  free();
  if ( ! other._shapes.size() ) return *this;
  _shapes.resize( other._shapes.size(), 0 );
  std::vector<Shape*>::iterator t = _shapes.begin();
  std::vector<Shape*>::const_iterator i = other._shapes.begin();
  std::vector<Shape*>::const_iterator end = other._shapes.end();
  while ( i != end ) {
    *t = (*i)->clone();
    ++i; ++t;
  }
  return *this;
}

ShapeList &
ShapeList::operator<<( const Shape & shape )
{
  if ( typeid( shape ) == typeid( ShapeList ) ) {
    // Insertion on top, respecting the same depth order.    
    const ShapeList & sl = dynamic_cast<const ShapeList &>( shape );
    std::vector<Shape*> shapes = sl._shapes;
    stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
    std::vector<Shape*>::iterator i = shapes.begin();
    std::vector<Shape*>::iterator end = shapes.end();
    while ( i != end ) {
      Shape * s = (*i)->clone();
      s->depth( _nextDepth-- );
      _shapes.push_back( s );
      ++i;
    }
  } else {
    Shape * s = shape.clone();
    if ( s->depth() == -1 )
      s->depth( _nextDepth-- );
    _shapes.push_back( s );
    if ( typeid( shape ) == typeid( Group ) ) {
      _nextDepth = dynamic_cast<const Group&>(shape).minDepth() - 1;
    }
  }
  return *this;
}

void
ShapeList::addShape( const Shape & shape, double scaleFactor )
{
  if ( typeid( shape ) == typeid( ShapeList ) ) {
    // Insertion on top, respecting the same depth order.
    const ShapeList & sl = dynamic_cast<const ShapeList &>( shape );
    std::vector<Shape*> shapes = sl._shapes;
    stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
    std::vector<Shape*>::iterator i = shapes.begin();
    std::vector<Shape*>::iterator end = shapes.end();
    while ( i != end ) {
      Shape * s = (*i)->clone();
      s->depth( _nextDepth-- );
      if ( scaleFactor != 1.0 )
	s->scaleAll( scaleFactor );
      _shapes.push_back( s );
      ++i;
    }
  } else {
    Shape * s = shape.clone();
    if ( s->depth() == -1 )
      s->depth( _nextDepth-- );
    if ( scaleFactor != 1.0 )
      s->scaleAll( scaleFactor );
    _shapes.push_back( s );
    if ( typeid( shape ) == typeid( Group ) ) {
      _nextDepth = dynamic_cast<const Group&>(shape).minDepth() - 1;
    }
  }
}

ShapeList &
ShapeList::dup( unsigned int count )
{
  if ( ! _shapes.size() ) { 
    warning << "dup() called with an empty list of shapes.\n";
    return *this;
  }
  while ( count-- ) {
    (*this) << (*_shapes.back());
  }
  return *this;
}

ShapeList &
ShapeList::operator+=( const Shape & shape )
{
  if ( typeid( shape ) == typeid( ShapeList ) ) {
    const ShapeList & sl = dynamic_cast<const ShapeList &>( shape );
    std::vector<Shape*>::const_iterator i = sl._shapes.begin();
    std::vector<Shape*>::const_iterator end = sl._shapes.end();
    while ( i != end ) {
      Shape * s = (*i)->clone();
      _shapes.push_back( s );
      ++i;
    }
  } else {
    _shapes.push_back( shape.clone() );
  }
  return *this;
}

ShapeList &
ShapeList::insert( const Shape & , int /* depth */ )
{
  warning << "ShapeList::insert() not implemented yet.\n";
  return *this;
}


Point
ShapeList::center() const {
  std::vector<Shape*>::const_iterator i = _shapes.begin();
  std::vector<Shape*>::const_iterator end = _shapes.end();
  float f = 1.0 / _shapes.size();
  Point r(0,0);
  while ( i != end ) {
    r += f * (*i)->center();
    ++i;
  }
  return r;
}

Shape &
ShapeList::rotate( double angle, const Point & center )
{
  std::vector<Shape*>::iterator i = _shapes.begin();
  std::vector<Shape*>::iterator end = _shapes.end();
  while ( i != end ) {
    (*i)->rotate( angle, center );
    ++i;
  }
  return *this;
}

ShapeList
ShapeList::rotated( double angle, const Point & center )
{
  ShapeList r( *this );
  return static_cast<ShapeList&>( r.rotate( angle, center ) );
}

Shape &
ShapeList::rotate( double angle )
{
  return rotate( angle, center() );
}

ShapeList
ShapeList::rotated( double angle )
{
  ShapeList r( *this );
  return static_cast<ShapeList&>( r.rotate( angle, center() ) );
}

Shape &
ShapeList::translate( double dx, double dy )
{
  std::vector<Shape*>::iterator i = _shapes.begin();
  std::vector<Shape*>::iterator end = _shapes.end();
  while ( i != end ) {
    (*i)->translate( dx, dy );
    ++i;
  }
  return *this;  
}

ShapeList
ShapeList::translated( double dx, double dy )
{
  ShapeList r( *this );
  return static_cast<ShapeList&>( r.translate( dx, dy ) );
}

Shape &
ShapeList::scale( double sx, double sy )
{
  Point c = center();
  Point delta;
  std::vector<Shape*>::iterator i = _shapes.begin();
  std::vector<Shape*>::iterator end = _shapes.end();
  while ( i != end ) {
    delta = (*i)->center() - c;
    delta.x *= sx;
    delta.y *= sy;
    (*i)->scale( sx, sy );
    delta = ( c + delta ) - (*i)->center();
    (*i++)->translate( delta.x, delta.y );
  }
  return *this;
}

Shape &
ShapeList::scale( double s )
{
  scale( s, s );
  return *this;
}

ShapeList
ShapeList::scaled( double sx, double sy )
{
  ShapeList r( *this );
  return static_cast<ShapeList&>( r.scale( sx, sy ) );
}

ShapeList
ShapeList::scaled( double s )
{
  return scaled( s, s );
}

void
ShapeList::scaleAll( double s )
{
  std::vector<Shape*>::iterator i = _shapes.begin();
  std::vector<Shape*>::iterator end = _shapes.end();
  while ( i != end ) {
    (*i++)->scaleAll( s );
  }
}

void
ShapeList::flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const
{
  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  stream << "%%% Begin ShapeList\n";
  while ( i != end ) {
    (*i++)->flushPostscript( stream, transform );
  }
  stream << "%%% End ShapeList\n";
}
  
void
ShapeList::flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const
{
  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  while ( i != end ) {
    (*i)->flushFIG( stream, transform, colormap );
    ++i;
  }  
}

void
ShapeList::flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const
{
  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  stream << "<g>\n";
  while ( i != end ) {
    (*i)->flushSVG( stream, transform );
    ++i;
  }  
  stream << "</g>\n";
}

Rect
ShapeList::boundingBox() const
{
  Rect r;
  std::vector< Shape* >::const_iterator i = _shapes.begin();
  std::vector< Shape* >::const_iterator end = _shapes.end();
  if ( i == end ) return r;
  r = (*i)->boundingBox();
  ++i;
  while ( i != end ) { 
    r = r || (*i)->boundingBox();
    ++i;
  }
  return r;
}

int
ShapeList::minDepth() const
{
  int res = std::numeric_limits<int>::max();
  int d;
  ShapeList * sl;
  std::vector< Shape* >::const_iterator i = _shapes.begin();
  std::vector< Shape* >::const_iterator end = _shapes.end();
  while ( i != end ) { 
    sl = dynamic_cast<ShapeList*>( *i );
    if ( sl ) {
      d = sl->minDepth();
    } else {
      d = (*i)->depth();
    }
    if ( d < res ) res = d;
    ++i;
  }
  return res;
}

int
ShapeList::maxDepth() const
{
  int res = std::numeric_limits<int>::max();
  int d;
  ShapeList * sl;
  std::vector< Shape* >::const_iterator i = _shapes.begin();
  std::vector< Shape* >::const_iterator end = _shapes.end();
  while ( i != end ) { 
    sl = dynamic_cast<ShapeList*>( *i );
    if ( sl ) {
      d = sl->maxDepth();
    } else {
      d = (*i)->depth();
    }
    if ( d > res ) res = d;
    ++i;
  }
  return res;
}

void
ShapeList::shiftDepth( int shift )
{
  std::vector< Shape* >::const_iterator i = _shapes.begin();
  std::vector< Shape* >::const_iterator end = _shapes.end();
  while ( i != end ) { 
    (*i++)->shiftDepth( shift );
  }
}

Shape *
ShapeList::clone() const
{
  ShapeList * g = new ShapeList( *this );
  return g;
}

Shape &
ShapeList::last( const unsigned int position )
{
  return last<Shape>( position );
}

Shape &
ShapeList::top()
{
  return last<Shape>(0);
}


  //
  // Definition of the Group methods.
  //

const std::string Group::_name("GroupOfShapes");

const std::string &
Group::name() const
{
  return _name;
}

Group
Group::rotated( double angle, const Point & center )
{
  return static_cast<const Group &>( Group( *this ).rotate( angle, center ) );
}

Group
Group::rotated( double angle )
{
  return static_cast<const Group &>( Group( *this ).rotate( angle ) );
}

Group
Group::translated( double dx, double dy )
{
  return static_cast<const Group &>( Group( *this ).translate( dx, dy ) );
}

Group
Group::scaled( double sx, double sy )
{
  return static_cast<const Group &>( Group( *this ).scale( sx, sy ) );
}

Group
Group::scaled( double s )
{
  return static_cast<const Group &>( Group( *this ).scale( s ) );
}


void
Group::flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const
{
  stream << "%%% Begin Group\n";
  ShapeList::flushPostscript( stream, transform );
  stream << "%%% End Group\n";
}
  
void
Group::flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const
{
  Rect bbox = boundingBox();
  stream << "# Begin group\n";
  stream << "6 "
	 << transform.mapX( bbox.left ) << " "
	 << transform.mapY( bbox.top ) << " "
	 << transform.mapX( bbox.left + bbox.width ) << " "
	 << transform.mapY( bbox.top - bbox.height ) << "\n";
  ShapeList::flushFIG( stream, transform, colormap );
  stream << "-6\n";
  stream << "# End Group\n";
}

void
Group::flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const
{
  stream << "<g>\n";
  ShapeList::flushSVG( stream, transform );
  stream << "</g>\n";
}

Shape *
Group::clone() const
{
  Group * g = new Group( *this );
  return g;
}

Group &
Group::operator=( const Group & other )
{
  ShapeList::operator=( other );
  return *this;
}


} // namespace LibBoard
