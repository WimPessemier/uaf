/* This file is part of the UAF (Unified Architecture Framework) project.
 *
 * Copyright (C) 2012 Wim Pessemier (Institute of Astronomy, KULeuven)
 *
 * Project website: http://www.ster.kuleuven.be/uaf
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uaf/util/namespacearray.h"


namespace uaf
{
    using namespace uaf;
    using std::string;
    using std::stringstream;


    // Constructor
    // =============================================================================================
    NamespaceArray::NamespaceArray() {}


    // Destructor
    // =============================================================================================
    NamespaceArray::~NamespaceArray() {}


    // Read the contents of a data value as returned when a client reads the NamespaceArray NodeId.
    // =============================================================================================
    Status NamespaceArray::fromSdk(const OpcUa_DataValue& value)
    {
        Status ret;

        // check if the status of the value is good:
        if (OpcUa_IsGood(value.StatusCode))
        {
            UaStringArray    stringArray;
            OpcUa_StatusCode uaConversionStatusCode;

            // convert the value to a StringArray
            uaConversionStatusCode = UaVariant(value.Value).toStringArray(stringArray);

            // if the conversion succeeded, fill the map with simplified URIs
            if (OpcUa_IsGood(uaConversionStatusCode))
            {
                for (uint16_t i=0; i<stringArray.length(); i++)
                {
                    nameSpaceMap_[i] = NamespaceArray::getSimplifiedUri(
                            string(UaString(&stringArray[i]).toUtf8()));
                }
            }

            // update the return status
            ret.fromSdk(uaConversionStatusCode,
                                   "Could not convert the received data value");

        }
        // if the status of the value was not good, just update the return status
        else
        {
            ret.fromSdk(value.StatusCode, "Could not process the received data value");
        }

        return ret;
    }


    // Look up the NamespaceURI for a given NamespaceIndex
    // =============================================================================================
    bool NamespaceArray::findNamespaceUri(NameSpaceIndex namespaceIndex, string& namespaceUri) const
    {
        // find the NamespaceIndex
        NameSpaceMap::const_iterator iter = nameSpaceMap_.find(namespaceIndex);

        bool found = (iter != nameSpaceMap_.end());

        // if the NamespaceIndex was found, assign the URI
        if (found)
            namespaceUri = iter->second;

        return found;
    }


    // Look up the NamespaceIndex for a given NamespaceURI
    // =============================================================================================
    bool NamespaceArray::findNamespaceIndex(
            const string&   namespaceUri,
            NameSpaceIndex& namespaceIndex) const
    {
        // We need to do a "reverse" lookup, by iterating over the key:value pairs until we have
        // found the correct value.

        // get a simplified URI
        string simplifiedUri = NamespaceArray::getSimplifiedUri(namespaceUri);

        // iterate over the key:value pairs
        NameSpaceMap::const_iterator iter;
        for (iter = nameSpaceMap_.begin(); iter != nameSpaceMap_.end(); ++iter)
        {
            // if the NamespaceURI was found, assign the NamespaceIndex, update the status,
            // and return
            if (iter->second == simplifiedUri)
            {
                namespaceIndex = iter->first;

                // we found the URI, so we can return!
                return true;
            }
        }

        // we haven't returned yet, so the URI was not found...
        return false;
    }


    // Get a string representation of the NamespaceArray (one line per index:URI pair)
    // =============================================================================================
    string NamespaceArray::toString() const
    {
        stringstream ss;

        // iterate over the map to print each key:value pair
        NameSpaceMap::const_iterator iter;
        for (iter = nameSpaceMap_.begin(); iter != nameSpaceMap_.end(); ++iter)
        {
            ss << " " << iter->first << ": " << iter->second << "\n";
        }

        return ss.str();
    }


    // static function!
    // Create a simplified (plain) URI
    // =============================================================================================
    string NamespaceArray::getSimplifiedUri(const string& uri)
    {
        string ret(uri);

        if (ret.size() > 0)
        {
            // transform to lower case
            transform(ret.begin(), ret.end(), ret.begin(), std::ptr_fun<int, int>(tolower));
            // remove a trailing '/' character
            if (ret.substr(ret.length()-1, 1) == string("/"))
            {
                ret = ret.substr(0, ret.length()-1);
            }
        }
        return ret;
    }


    // Convert a uaf::Address to a OpcUa_NodeId
    // =============================================================================================
    Status NamespaceArray::fillOpcUaNodeId(
            const Address&  address,
            OpcUa_NodeId&   opcUaNodeId) const
    {
        if (address.isExpandedNodeId())
            return fillOpcUaNodeId(address.getExpandedNodeId().nodeId(), opcUaNodeId);
        else
            return Status(statuscodes::UnexpectedError,
                          "Can't extract the NodeId from the address");
    }


    // Convert a uaf::NodeId to a OpcUa_NodeId
    // =============================================================================================
    Status NamespaceArray::fillOpcUaNodeId(
            const NodeId& nodeId,
            OpcUa_NodeId& opcUaNodeId) const
    {
        Status ret;

        // we need to get a valid namespace index
        NameSpaceIndex nameSpaceIndex;

        // check if the browse name has a namespace URI for which we can find an index
        if (nodeId.hasNameSpaceUri())
        {

            if (findNamespaceIndex(nodeId.nameSpaceUri(), nameSpaceIndex))
                ret.setGood();
            else
                ret.setStatus(statuscodes::ResolutionError,
                              "Could not resolve the node, unknown namespace URI");
        }
        else if (nodeId.hasNameSpaceIndex())
        {
            // OK we don't have a namespace URI but we do have a namespace index
            nameSpaceIndex = nodeId.nameSpaceIndex();
            ret.setGood();
        }
        else
        {
            // we have no possible means to get the namespace index
            ret.setStatus(statuscodes::ResolutionError,
                          "Could not resolve the node since no namespace index or URI are given");
        }


        // if the namespace index was found, update the OpcUa_NodeId
        if (ret.isGood())
        {
            UaNodeId uaNodeId;
            // update the NodeId identifier
            if (nodeId.identifier().type == nodeididentifiertypes::Numeric)
            {
                uaNodeId.setNodeId(nodeId.identifier().idNumeric, nameSpaceIndex);
                uaNodeId.copyTo(&opcUaNodeId);
            }
            else if (nodeId.identifier().type == nodeididentifiertypes::String)
            {
                uaNodeId.setNodeId(UaString(nodeId.identifier().idString.c_str()), nameSpaceIndex);
                uaNodeId.copyTo(&opcUaNodeId);
            }
            else
                ret.setStatus(
                        statuscodes::UnsupportedError,
                        "UAF does not support NodeIds with nodeId identifiers other than Numeric "
                        "or String");
        }

        return ret;
    }





    // Convert a uaf::QualifiedName to a OpcUa_QualifiedName
    // =============================================================================================
    Status NamespaceArray::fillOpcUaQualifiedName(
            const QualifiedName& qualifiedName,
            OpcUa_QualifiedName& opcUaQualifiedName) const
    {
        Status ret;

        // check if the browse name has a namespace URI for which we can find an index
        if (qualifiedName.hasNameSpaceUri())
        {
            // we need to get a valid namespace index
            NameSpaceIndex nameSpaceIndex;

            if (findNamespaceIndex(qualifiedName.nameSpaceUri(), nameSpaceIndex))
            {
                // copy the contents from the qualified name to the SDK object
                // (except the namespaceUri because that's not relevant for the SDK object anyway)
                QualifiedName(qualifiedName.name(), nameSpaceIndex).toSdk(&opcUaQualifiedName);
                ret.setGood();
            }
            else
            {
                ret.setStatus(statuscodes::ResolutionError,
                              "Could not resolve '%s', unknown namespace URI",
                              qualifiedName.toString().c_str());
            }
        }
        else if (qualifiedName.hasNameSpaceIndex())
        {
            // OK we don't have a namespace URI but we do have a namespace index
            qualifiedName.toSdk(&opcUaQualifiedName);
            ret.setGood();
        }
        else
        {
            // we have no possible means to get the namespace index
            ret.setStatus(statuscodes::ResolutionError,
                          "Could not resolve qualified name '%s' since no namespace index or URI "
                          "are provided");
        }

        return ret;
    }


    // Convert a uaf::RelativePathElement to a OpcUa_RelativePathElement
    // =============================================================================================
    Status NamespaceArray::fillOpcUaRelativePathElement(
            const RelativePathElement& element,
            OpcUa_RelativePathElement& uaElement) const
    {
        Status status;

        // update the target name
        status = fillOpcUaQualifiedName(element.targetName, uaElement.TargetName);

        // update the reference type (unless the target name updating failed)
        if (status.isGood())
            status = fillOpcUaNodeId(element.referenceType, uaElement.ReferenceTypeId);

        // update the other properties of the uaElement (unless something has failed already)
        if (status.isGood())
        {
            // also update the uaRelativePathElements
            if (element.includeSubtypes)
                uaElement.IncludeSubtypes = OpcUa_True;
            else
                uaElement.IncludeSubtypes = OpcUa_False;
            if (element.isInverse)
                uaElement.IsInverse = OpcUa_True;
            else
                uaElement.IsInverse = OpcUa_False;
        }

        return status;
    }


    // Convert an OpcUa_NodeId to a fully resolved uaf::NodeId
    // =============================================================================================
    Status NamespaceArray::fillNodeId(const OpcUa_NodeId& opcUaNodeId, NodeId& nodeId) const
    {
        Status ret;

        string namespaceUri;

        if (findNamespaceUri(opcUaNodeId.NamespaceIndex, namespaceUri))
            ret = nodeId.fromSdk(opcUaNodeId, namespaceUri);
        else
            ret.setStatus(statuscodes::ResolutionError,
                          "Unknown namespace index %d", opcUaNodeId.NamespaceIndex);

        return ret;
    }



}


