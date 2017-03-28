/**
 * @file feature_container_matches_sorted_objects.cc
 *
 * @brief
 *
 * Class used to describe feature_container_matches_sorted_objects 
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/world.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/feature_container_matches_sorted_objects.h"
#include "h2sl/object.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Matches_Sorted_Objects class constructor
 */
Feature_Container_Matches_Sorted_Objects::
Feature_Container_Matches_Sorted_Objects( const bool& invert,
                                        const int& number,
                                        const string& sortingKey ) : Feature( invert ) {
  insert_prop< int >( _int_properties, "number", number );
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );
}

Feature_Container_Matches_Sorted_Objects::
Feature_Container_Matches_Sorted_Objects( xmlNodePtr root ) : Feature() {
  insert_prop< int >( _int_properties, "number", 0 );
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  from_xml( root );
}

/**
 * Feature_Container_Matches_Sorted_Objects class copy constructor
 */
Feature_Container_Matches_Sorted_Objects::
Feature_Container_Matches_Sorted_Objects( const Feature_Container_Matches_Sorted_Objects& other ) : Feature( other ) {

}

/**
 * Feature_Container_Matches_Sorted_Objects class destructor
 */
Feature_Container_Matches_Sorted_Objects::
~Feature_Container_Matches_Sorted_Objects() {

}

/** 
 * Feature_Container_Matches_Sorted_Objects class assignment operator
 */
Feature_Container_Matches_Sorted_Objects&
Feature_Container_Matches_Sorted_Objects::
operator=( const Feature_Container_Matches_Sorted_Objects& other ){
  _invert = other._invert;
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Container_Matches_Sorted_Objects::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}


/**
 * returns the value of the feature
 */
bool
Feature_Container_Matches_Sorted_Objects::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if( container != NULL ){
    map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
    if( it_sorted_objects_map == world->sorted_objects().end() ){
      cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
    }
    assert( it_sorted_objects_map != world->sorted_objects().end() );
    if( !container->container().empty() ){
      map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( dynamic_cast< const Object* >( container->container().front() )->type() );
      assert( it_sorted_objects != it_sorted_objects_map->second.end() );
      if( number() < ( int )( it_sorted_objects->second.size() ) ){
        bool all_objects_match = true;
        for( unsigned int i = 0; i < container->container().size(); i++ ){
          Object * object = dynamic_cast< Object* >( container->container()[ i ] );
          bool found_object_match = false;
          if( object != NULL ){
            for( unsigned int j = 0; j < number(); j++ ){
              if( object->name() == it_sorted_objects->second[ j ]->name() ){
                found_object_match = true;
              }
            }
          }
          all_objects_match = all_objects_match && found_object_match;
        }
        if( all_objects_match ){
          return !_invert;
        } else {
          return _invert; 
        }
      }
    }
  }
  return false;
}

/** 
 * imports the Feature_Container_Matches_Sorted_Objects class from an XML file
 */
void
Feature_Container_Matches_Sorted_Objects::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_matches_sorted_objects" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
      xmlFreeDoc( doc );
    }
  }
  return;
}


/** 
 * imports the Feature_Container_Matches_Sorted_Objects class from an XML node pointer
 */
void
Feature_Container_Matches_Sorted_Objects::
from_xml( xmlNodePtr root ){
  _invert = false;
  number() = 0;
  sorting_key() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "number", "sorting_key" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, int > number_prop = has_prop< int >( root, "number" );
    if( number_prop.first ) {
      number() = number_prop.second;
    }

    pair< bool, std::string > sorting_key_prop = has_prop< std::string >( root, "sorting_key" );
    if( sorting_key_prop.first ) {
      sorting_key() = sorting_key_prop.second;
    }
  }
}

/**
 * exports the Feature_Container_Matches_Sorted_Objects class to an XML file
 */
void
Feature_Container_Matches_Sorted_Objects::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

/**
 * exports the Feature_Container_Matches_Sorted_Objects class to an XML node pointer
 */
void
Feature_Container_Matches_Sorted_Objects::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_matches_sorted_objects" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "number" ), ( const xmlChar* )( to_std_string( number() ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Container_Matches_Sorted_Objects class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Matches_Sorted_Objects& other ){
    out << "Feature_Container_Matches_Sorted_Objects:( invert:(" << other.invert() << ") number:(" << other.number() << ") sorting_key:(" << other.sorting_key() << "))";
    return out;
  }
}

