// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2003-2005  ONERA
//
// This file is part of CERTI
//
// CERTI is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation ; either version 2 of the License, or
// (at your option) any later version.
//
// CERTI is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// $Id: DataDistribution.cc,v 3.20 2005/03/28 19:08:02 breholee Exp $
// ----------------------------------------------------------------------------

#include <config.h>
#include "DataDistribution.hh"

#include "ObjectClassAttribute.hh"
#include "FedRegion.hh"

#include <cassert>

using std::string ;
using std::endl ;

namespace certi {
namespace rtia {

static pdCDebug D("RTIA_DDM", __FILE__);

// ----------------------------------------------------------------------------
DataDistribution::DataDistribution(RootObject *root_object,
                                   FederationManagement *fed_management,
                                   Communications *communications)
    : rootObject(root_object),
      fm(fed_management),
      comm(communications) { }

// ----------------------------------------------------------------------------
SpaceHandle
DataDistribution::getRoutingSpaceHandle(string name) const 
{
    return rootObject->getRoutingSpaceHandle(name);
}

// ----------------------------------------------------------------------------
// getRoutingSpaceName
//
string
DataDistribution::getRoutingSpaceName(SpaceHandle handle) const
{
    return rootObject->getRoutingSpaceName(handle);
}

// ----------------------------------------------------------------------------
// getDimensionHandle
//
DimensionHandle
DataDistribution::getDimensionHandle(string dimension, SpaceHandle space) const
    throw (SpaceNotDefined, NameNotFound)
{
    return rootObject->getRoutingSpace(space).getDimensionHandle(dimension);
}

// ----------------------------------------------------------------------------
// getDimensionName
//
string
DataDistribution::getDimensionName(DimensionHandle dimension,
				   SpaceHandle space) const
    throw (SpaceNotDefined, DimensionNotDefined)
{
    return rootObject->getRoutingSpace(space).getDimensionName(dimension);
}

// ----------------------------------------------------------------------------
// getAttributeSpaceHandle
//
SpaceHandle
DataDistribution::getAttributeSpace(AttributeHandle attribute,
                                    ObjectClassHandle object_class) const
    throw (ObjectClassNotDefined, AttributeNotDefined)
{
    return rootObject->ObjectClasses->getWithHandle(object_class)->
        getAttribute(attribute)->getSpace();
}

// ----------------------------------------------------------------------------
// getInteractionSpaceHandle
//
SpaceHandle
DataDistribution::getInteractionSpace(InteractionClassHandle interaction) const
    throw (InteractionClassNotDefined)
{
    return rootObject->Interactions->getByHandle(interaction)->getSpace();
}

// ----------------------------------------------------------------------------
// createRegion
//
long
DataDistribution::createRegion(SpaceHandle space,
                               unsigned long nb_extents,
                               TypeException &e)
    throw (SpaceNotDefined)
{
    D[pdDebug] << "Start creating region in space " << space << "..." << endl ;

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_CREATE_REGION ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.space = space ;
    req.nbExtents = nb_extents ;

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_CREATE_REGION, req.federate);
    e = rep.exception ;

    if (e == e_NO_EXCEPTION) {
	D[pdDebug] << "Create region " << rep.region << endl ;
        RTIRegion *region = new RTIRegion(rep.region,
					  rootObject->getRoutingSpace(space),
					  nb_extents);

	assert(region->getNumberOfExtents() == nb_extents);
        rootObject->addRegion(region);

        return rep.region ;
    }
    else
        return 0 ;
}

// ----------------------------------------------------------------------------
// modifyRegion
//
void
DataDistribution::modifyRegion(RegionHandle handle,
			       const std::vector<Extent> &extents,
			       TypeException &e)
{
    D[pdDebug] << "Modify region " << handle << "..." << endl ;

    // check region
    RTIRegion *region = rootObject->getRegion(handle);

    // Request to RTIG
    NetworkMessage req, rep ;
    req.type = NetworkMessage::DDM_MODIFY_REGION ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.region = handle ;
    req.setExtents(extents);

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_MODIFY_REGION, req.federate);
    e = rep.exception ;

    if (e == e_NO_EXCEPTION) {
	region->replaceExtents(extents);	
	D[pdDebug] << "Modified region " << handle << endl ;
    }
}

// ----------------------------------------------------------------------------
// deleteRegion
//
void
DataDistribution::deleteRegion(long handle, TypeException &e)
    throw (RegionNotKnown, RegionInUse)
{
    D[pdDebug] << "Delete region " << handle << "..." << endl ;

    // check region
    rootObject->getRegion(handle);

    // Request to RTIG
    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_DELETE_REGION ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.region = handle ;

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_DELETE_REGION, req.federate);
    e = rep.exception ;

    if (e == e_NO_EXCEPTION) {
        rootObject->deleteRegion(handle);
        D[pdDebug] << "Deleted region " << handle << endl ;
    }
}

// ----------------------------------------------------------------------------
void
DataDistribution::associateRegion(ObjectHandle object,
				  RegionHandle region,
				  AttributeHandle *attr,
				  int nb,
				  TypeException &e)
    throw (RegionNotKnown)
{
    D[pdDebug] << "Associate Region " << region << std::endl ;

    RTIRegion *r = rootObject->getRegion(region);

    D[pdDebug] << "- unassociate object " << object << std::endl ;
    rootObject->getObject(object)->unassociate(r);
    for (int i = 0 ; i < nb ; ++i) {
	D[pdDebug] << "- associate attribute " << attr[i] << std::endl ;
	rootObject->getObjectAttribute(object, attr[i])->associate(r);	
    }

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_ASSOCIATE_REGION ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.object = object ;
    req.region = region ;
    req.setAHS(attr, nb);

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_ASSOCIATE_REGION,
		      req.federate);

    e = rep.exception ;
}

// ----------------------------------------------------------------------------
ObjectHandle
DataDistribution::registerObject(ObjectClassHandle class_handle,
				 const std::string name,
				 const AttributeHandle *attrs,
				 int nb,
				 const std::vector<RegionHandle> regions,
				 TypeException &e)
{
    D[pdDebug] << "Register object of class " << class_handle << " with "
	       << regions.size() << " region(s)." << std::endl ;

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_REGISTER_OBJECT ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.objectClass = class_handle ;
    req.setTag(name.c_str());
    req.setAHS(attrs, nb);
    req.setRegions(regions);

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_REGISTER_OBJECT,
		      req.federate);

    e = rep.exception ;

    if (e == e_NO_EXCEPTION) {
        rootObject->registerObjectInstance(fm->federate, class_handle, rep.object,
                                           rep.label);
	for (int i = 0 ; i < nb ; ++i) {
	    D[pdDebug] << "Register attribute [" << i << "] Attr: " << attrs[i]
		       << " Region: " << regions[i] << std::endl ;
		
	    ObjectAttribute *attribute = rootObject->getObjectAttribute(rep.object, attrs[i]);
	    RTIRegion *region = rootObject->getRegion(regions[i]);
	    attribute->associate(region);
	}
        return rep.object ;
    }
    else return 0 ;
}

// ----------------------------------------------------------------------------
void
DataDistribution::unassociateRegion(ObjectHandle object,
				    RegionHandle region,
				    TypeException &e)
    throw (ObjectNotKnown, InvalidRegionContext, RegionNotKnown)
{
    D[pdDebug] << "Unassociate Region " << region << std::endl ;

    RTIRegion *r = rootObject->getRegion(region);

    rootObject->getObject(object)->unassociate(r);

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_UNASSOCIATE_REGION ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.object = object ;
    req.region = region ;

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_UNASSOCIATE_REGION,
		      req.federate);

   e = rep.exception ;
}

// ----------------------------------------------------------------------------
void
DataDistribution::subscribe(ObjectClassHandle obj_class,
			    RegionHandle region,
			    AttributeHandle *attr,
			    int nb,
			    TypeException &e)
    throw (RegionNotKnown)
{
    D[pdDebug] << "Subscribe attributes with region " << region << endl ;
    rootObject->getRegion(region);

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_SUBSCRIBE_ATTRIBUTES ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.objectClass = obj_class ;
    req.region = region ;
    req.setAHS(attr, nb);

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_SUBSCRIBE_ATTRIBUTES,
		      req.federate);

    e = rep.exception ;
}

// ----------------------------------------------------------------------------
void
DataDistribution::unsubscribeAttributes(ObjectClassHandle obj_class,
					RegionHandle region,
					TypeException &e)
    throw (RegionNotKnown)
{
    D[pdDebug] << "Unsubscribe class " << obj_class 
	       << " with region " << region << endl ;
    rootObject->getRegion(region);

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_UNSUBSCRIBE_ATTRIBUTES ;
    req.federation = fm->_numero_federation ;
    req.federate = fm->federate ;
    req.objectClass = obj_class ;
    req.region = region ;

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_UNSUBSCRIBE_ATTRIBUTES,
		      req.federate);

    e = rep.exception ;
}

// ----------------------------------------------------------------------------
void
DataDistribution::subscribeInteraction(InteractionClassHandle int_class,
				       RegionHandle region,
				       TypeException &e)
    throw (RegionNotKnown)
{
    D[pdDebug] << "Subscribe interaction with region " << region << endl ;
    rootObject->getRegion(region);

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_SUBSCRIBE_INTERACTION ;
    req.interactionClass = int_class ;
    req.region = region ;

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_SUBSCRIBE_INTERACTION,
		      req.federate);

    e = rep.exception ;
}

// ----------------------------------------------------------------------------
void
DataDistribution::unsubscribeInteraction(InteractionClassHandle int_class,
					 RegionHandle region,
					 TypeException &e)
    throw (RegionNotKnown)
{
    D[pdDebug] << "Unsubscribe interaction with region " << region << endl ;
    rootObject->getRegion(region);

    NetworkMessage req, rep ;

    req.type = NetworkMessage::DDM_UNSUBSCRIBE_INTERACTION ;
    req.interactionClass = int_class ;
    req.region = region ;

    comm->sendMessage(&req);
    comm->waitMessage(&rep, NetworkMessage::DDM_UNSUBSCRIBE_INTERACTION,
		      req.federate);

    e = rep.exception ;
}

}} // namespace certi::rtia

// $Id: DataDistribution.cc,v 3.20 2005/03/28 19:08:02 breholee Exp $
