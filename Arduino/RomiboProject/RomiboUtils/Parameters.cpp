// Parameters.cpp : numeric parameter interface for interactive debugging and control
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
#include <stdlib.h>
#include <string.h>
#include "Parameters.h"

/****************************************************************/
// Construct a parameter table object given the first array.
Parameters::Parameters( struct parameter_descriptor_t *parameters_table, size_t table_size )
{
  if (parameters_table != NULL && table_size > 0) {
    list = (struct parameter_sublist_t *) malloc(sizeof(struct parameter_sublist_t));
    list->next = NULL;
    list->table = parameters_table;
    list->entries = table_size / sizeof( struct parameter_descriptor_t );
  } else {
    list = NULL;
  }
}

// Extend the set of parameters.
void Parameters::addParameters( struct parameter_descriptor_t *parameters_table, size_t table_size )
{
  struct parameter_sublist_t *sublist = (struct parameter_sublist_t *) malloc(sizeof(struct parameter_sublist_t));
  sublist->next = list;
  sublist->table = parameters_table;
  sublist->entries = table_size / sizeof( struct parameter_descriptor_t );

  // prepend the new set to the linked list of sets
  list = sublist;
}

/****************************************************************/
// Initialize an iterator to the start of the parameters.
void Parameters::initIterator(  struct Parameters::iterator_t *iter )
{
  iter->list = list;
  iter->i = 0;
}

// Return a parameter descriptor or NULL, and update the iterator to the next item.
struct parameter_descriptor_t *Parameters::nextParameter(   struct Parameters::iterator_t *iter )
{
  // Guard against the case of an empty list or having reached the end of the iteration.
  if (iter->list == NULL || iter->i >= iter->list->entries) return NULL;

  // Save the entry to which the iterator currently points.
  struct parameter_descriptor_t *value = &iter->list->table[iter->i];

  // Advance the iterator to the next element, and if at the array end, to the next sublist.
  if (++iter->i >= iter->list->entries) {
    iter->i = 0;
    iter->list = iter->list->next;
  }
  return value;
}

/****************************************************************/
// Internal method to iterate over all parameters, calling a method callback on each one.
// This is generally less useful than the iterator methods.
void Parameters::iterateAll( void (Parameters::*callback)(struct parameter_descriptor_t *) )
{
  struct Parameters::iterator_t iterator;
  initIterator(&iterator);
  struct parameter_descriptor_t *desc = nextParameter(&iterator);

  while (desc) {
    (this->*callback)( desc );
    desc = nextParameter(&iterator);
  }
}

/****************************************************************/

// Internal function to find a named parameter in an array of descriptors.
// Returns the pointer to the descriptor or NULL.
struct parameter_descriptor_t *Parameters::find( const char *name )
{
  struct Parameters::iterator_t iterator;
  initIterator(&iterator);
  struct parameter_descriptor_t *desc = nextParameter(&iterator);

  while (desc) {
    if ( !strcmp(name, desc->name)) return desc;
    desc = nextParameter(&iterator);
  }
  return NULL;
}
int Parameters::isValid( const char *name )
{
  return (find(name) != NULL);
}

int Parameters::value( const char *name )
{
  struct parameter_descriptor_t *desc = find(name);
  if (desc) return *(desc->value); // dereference the pointer to the parameter in the global variable area
  else return 0;
}

void Parameters::set( const char *name, int value )
{
  struct parameter_descriptor_t *desc = find(name);
  if (desc) *(desc->value) = value;
}

void Parameters::setFirstMatching( const char *name, int value )
{
  if (name != NULL) {
    int namelen = strlen(name);
    struct Parameters::iterator_t iterator;
    initIterator(&iterator);
    struct parameter_descriptor_t *desc = nextParameter(&iterator);
    while (desc) {
      if ( !strncmp(name, desc->name, namelen)) {
	*(desc->value) = value;
	return;
      }
      desc = nextParameter(&iterator);
    }
  }
}

void Parameters::forEachMatching( void (*f)(const char *name, int value), const char *name )
{
  if (name != NULL) {
    int namelen = strlen(name);
    struct Parameters::iterator_t iterator;
    initIterator(&iterator);
    struct parameter_descriptor_t *desc = nextParameter(&iterator);
    while (desc) {
      if ( !strncmp(name, desc->name, namelen)) {
	f( desc->name, *(desc->value));
      }
      desc = nextParameter(&iterator);
    }
  }
}
