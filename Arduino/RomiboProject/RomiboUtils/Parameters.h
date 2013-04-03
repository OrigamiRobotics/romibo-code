// Parameters.h : numeric global parameter interface for interactive debugging and control
// Copyright (c) 2011 Garth Zeglin

// ##BEGIN_LICENSE##
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this file; if not, write to the Free Software Foundation,
// Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
// ##END_LICENSE##
/****************************************************************/

#ifndef __PARAMETERS_H_INCLUDED__
#define __PARAMETERS_H_INCLUDED__

#include <stdio.h>

// Descriptor for a single global parameter.
struct parameter_descriptor_t {
  const char *name;  // Name of the parameter, which must be a global variable.
  int *value;        // Address of the parameter in memory.  Only the default integer type is currently supported.
  // This may eventually expand to include a type field to support multiple data types.
};

// Linked list entry for storing one parameter array in a linked list of arrays.
struct parameter_sublist_t {
  struct parameter_sublist_t *next;
  struct parameter_descriptor_t *table;
  int entries;
};
  
// Define an object representing a set of parameters which can be queried and
// modified interactively.
class Parameters
{
private:

  // The parameter descriptors are stored as a linked list of descriptor
  // arrays.  This is intended to support static allocation of descriptor tables
  // for each individual static object, but still allow the whole set to be
  // extensible and declared separately.  Note that this allows each module to
  // provide a table for its private variables for use in debugging.
  struct parameter_sublist_t *list;  

  // traverse the list of arrays, calling a method for each parameter
  void iterateAll( void (Parameters::*)(struct parameter_descriptor_t *) );

  // A simple iterator structure which can be declared as an automatic variable
  // and used to traverse the set of parameters.  The coordinates always point
  // at the next item to return.
  struct iterator_t {
    struct parameter_sublist_t *list;
    int i;
  };
  // Initialize an iterator to the start of the parameters.
  void initIterator(  struct Parameters::iterator_t * );

  // Return the next parameter descriptor or NULL, and update the iterator to the next item.
  struct parameter_descriptor_t *nextParameter(   struct Parameters::iterator_t * );

  // Find a single named parameter.
  struct parameter_descriptor_t *find( const char *name );

public:
  // Constructor, which takes an initial array as the first list element.
  Parameters( struct parameter_descriptor_t *parameters_table, size_t table_size );

  // Extend the set of parameters.
  void addParameters( struct parameter_descriptor_t *parameters_table, size_t table_size );

  // Return true if the parameter name is valid.
  int isValid( const char *name );

  // Return the value of the parameter matching the name, or 0 if not found.
  int value( const char *name );

  // Set the value of the parameter, silently ignoring invalid names.
  void set( const char *name, int value );

  // Set the value of the first parameter with a partial name match.
  void setFirstMatching( const char *name, int value );

  // Iterate over the table, calling the given function once for each entry for
  // which the name matches the initial portion of the parameter name.  A null
  // name string matches nothing, an empty name string matches everything.
  void forEachMatching( void (*f)(const char *name, int value), const char *name );

};

#endif // __PARAMETERS_H_INCLUDED__
