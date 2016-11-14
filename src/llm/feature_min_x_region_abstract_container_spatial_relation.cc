/**
 * @file feature_min_x_region_abstract_container_spatial_relation.cc
 *
 * @brief
 *
 * Class used to describe a feature of the region_abstract_container number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/spatial_relation.h"
#include "h2sl/feature_min_x_region_abstract_container_spatial_relation.h"
#include "h2sl/region_abstract_container.h"

using namespace std;
using namespace h2sl;

/*bool
min_x_region_abstract_container_sort( Object* a,
        Object* b ){
  return a->transform().position().x() < b->transform().position().x();
}*/

bool
min_x_region_abstract_container_sort (Grounding* a, Grounding* b) {
    if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
        return ( ((dynamic_cast< const Object* >(a))->transform().position().x()) <
                ((dynamic_cast< const Object* >(b))->transform().position().x()) );
    } else {
        cout << "Cannot cast container entity into Object" << endl;
        return false;
    }
}

/**
 * Feature_Min_X_Region_Abstract_Container_Spatial_Relation class constructor
 */
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
Feature_Min_X_Region_Abstract_Container_Spatial_Relation( const bool& invert,
                                        const std::string& relation_type ) : Feature( invert ),
                                                                          _relation_type( relation_type ) {

}

/**
 * Feature_Min_X_Region_Abstract_Container_Spatial_Relation class copy constructor
 */
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
Feature_Min_X_Region_Abstract_Container_Spatial_Relation( const Feature_Min_X_Region_Abstract_Container_Spatial_Relation& other ) : Feature( other ),
                                                              _relation_type( other._relation_type ) {

}

/**
 * Feature_Min_X_Region_Abstract_Container_Spatial_Relation class destructor
 */
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
~Feature_Min_X_Region_Abstract_Container_Spatial_Relation() {

}

/** 
 * Feature_Min_X_Region_Abstract_Container_Spatial_Relation class assignment operator
 */
Feature_Min_X_Region_Abstract_Container_Spatial_Relation&
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
operator=( const Feature_Min_X_Region_Abstract_Container_Spatial_Relation& other ){
  _invert = other._invert;
  _relation_type = other._relation_type;
  return (*this);
}


/**
 * returns the value of the feature
 */
bool
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world) {
    return value( cv, grounding, children, phrase, world, NULL );
    
}

/**
 * returns the value of the feature
 */
bool
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Object* object_grounding = dynamic_cast< const Object* >( grounding );
  if( object_grounding != NULL ){
    Region_Abstract_Container* region_abstract_container_child = NULL;
    Spatial_Relation* spatial_relation_child = NULL;
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if( dynamic_cast< Region_Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
          region_abstract_container_child = static_cast< Region_Abstract_Container* >( children[ i ].second[ j ] );
        } else if ( dynamic_cast< Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
          spatial_relation_child = static_cast< Spatial_Relation* >( children[ i ].second[ j ] );
        }
      } 
    } 
    
    if( region_abstract_container_child != NULL ){
      if( spatial_relation_child != NULL ){
        if( spatial_relation_child->relation_type() == _relation_type ){
          map< string, vector< Object* > >::const_iterator it;
          it  = world->min_x_sorted_objects().find( region_abstract_container_child->abstract_container().type() );
          if (it != world->min_x_sorted_objects().end() ) {
            for (unsigned int i = 0; i < it->second.size(); i++ ) {
              if ( *object_grounding == *(it->second[ i ]) ) {
                if( i < world->numeric_map()[ region_abstract_container_child->abstract_container().number() ] ){
                  return !_invert;
                } else {
                  return _invert;
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

    

    
/*  if( region_abstract_container_child != NULL ){
    if( spatial_relation_child != NULL ){
      if( spatial_relation_child->type() == _relation_type ){
        if( object_grounding != NULL ){
          vector< Object* > objects;
          for( unsigned int i = 0; i < world->nsf_nri_mvli_objects().size(); i++ ){
            if( world->nsf_nri_mvli_objects()[ i ]->type() == region_abstract_container_child->type() ){
              objects.push_back( world->nsf_nri_mvli_objects()[ i ] );
            }
          } 
          sort( objects.begin(), objects.end(), min_x_region_abstract_container_sort );
          for( unsigned int i = 0; i < objects.size(); i++ ){
            if( *object_grounding == *objects[ i ] ){
              if( i < region_abstract_container_child->abstract_container().num() ){
                return !_invert;
              } else {
                return _invert;
              }
            }
          }
        }
      }
    }
  }
  return false;
}*/

/** 
 * imports the Feature_Min_X_Region_Abstract_Container_Spatial_Relation class from an XML file
 */
void
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_min_x_region_abstract_container_spatial_relation" ) ) == 0 ){
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
 * imports the Feature_Min_X_Region_Abstract_Container_Spatial_Relation class from an XML node pointer
 */
void
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
from_xml( xmlNodePtr root ){
  _invert = false;
  _relation_type = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "relation_type" ) );
    if( tmp != NULL){
      string relation_type_string = ( const char* )( tmp );
       _relation_type = relation_type_string;
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Min_X_Region_Abstract_Container_Spatial_Relation class to an XML file
 */
void
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
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
 * exports the Feature_Min_X_Region_Abstract_Container_Spatial_Relation class to an XML node pointer
 */
void
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_min_x_region_abstract_container_spatial_relation" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "relation_type" ), ( const xmlChar* )( _relation_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
relation_type( void ){
  return _relation_type;
}

const std::string&
Feature_Min_X_Region_Abstract_Container_Spatial_Relation::
relation_type( void )const{
  return _relation_type;
}

namespace h2sl {
  /** 
   * Feature_Min_X_Region_Abstract_Container_Spatial_Relation class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Min_X_Region_Abstract_Container_Spatial_Relation& other ){
    out << "class:\"Feature_Min_X_Region_Abstract_Container_Spatial_Relation\" ";
    out << "relation_type:\"" << other.relation_type() << "\" ";
    return out;
  }
}
