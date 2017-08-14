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

#include "uaf/util/argument.h"

namespace uaf
{
    using namespace uaf;
    using std::string;
    using std::stringstream;


    // Constructor
    // =============================================================================================
    Argument::Argument()
    : name(), valueRank(0)
    {}


    // Constructor
    // =============================================================================================
    Argument::Argument(const ExtensionObject& extensionObject)
    {
        UaExtensionObject uaExtensionObject;
        extensionObject.toSdk(uaExtensionObject);
        UaArgument uaArgument(uaExtensionObject);
        fromSdk(uaArgument);

        name = uaArgument.getName().toUtf8();
        dataType.fromSdk(uaArgument.getDataType());
        valueRank = uaArgument.getValueRank();
        UaUInt32Array uaArr;
        uaArgument.getArrayDimensions(uaArr);
        arrayDimensions.clear();
        for (uint32_t i=0; i<uaArr.length(); i++)
        {
            arrayDimensions.push_back(uaArr[i]);
        }
        description.fromSdk(uaArgument.getDescription());
    }

    // Constructor
    // =============================================================================================
    Argument::Argument(const std::string& name, const uaf::NodeId& dataType, int32_t valueRank,
            std::vector<uint32_t> arrayDimensions, const uaf::LocalizedText& description)
    : name(name), dataType(dataType), valueRank(valueRank), arrayDimensions(arrayDimensions),
      description(description)
    {
    }


    // Get a string representation
    // =============================================================================================
    string Argument::toString(const string& indent, size_t colon) const
    {
        stringstream ss;

        ss << indent << " - name";
        ss << fillToPos(ss, colon);
        ss << ": " << name << "\n";

        ss << indent << " - dataType";
        ss << fillToPos(ss, colon);
        ss << ": " << dataType.toString() << "\n";

        ss << indent << " - valueRank";
        ss << fillToPos(ss, colon);
        ss << ": " << valueRank << "\n";

        ss << indent << " - arrayDimensions";
        ss << fillToPos(ss, colon);
        ss << ": " << uint32ArrayToString(arrayDimensions) << "\n";

        ss << indent << " - description";
        ss << fillToPos(ss, colon);
        ss << ": " << description.toString();

        return ss.str();
    }


    // Convert from an UaArgument
    // =============================================================================================
    void Argument::fromSdk(const UaArgument& uaArgument)
    {
        name = uaArgument.getName().toUtf8();
        dataType.fromSdk(uaArgument.getDataType());
        valueRank = uaArgument.getValueRank();
        UaUInt32Array uaArr;
        uaArgument.getArrayDimensions(uaArr);
        arrayDimensions.clear();
        for (uint32_t i=0; i<uaArr.length(); i++)
        {
            arrayDimensions.push_back(uaArr[i]);
        }
        description.fromSdk(uaArgument.getDescription());
    }


    // Convert to an UaArgument
    // =============================================================================================
    void Argument::toSdk(UaArgument& uaArgument) const
    {
        uaArgument.setName(UaString(name.c_str()));

        UaNodeId uaNodeId;
        dataType.toSdk(uaNodeId);
        uaArgument.setDataType(uaNodeId);

        uaArgument.setValueRank(valueRank);

        UaUInt32Array uaArr;
        uaArr.resize(arrayDimensions.size());
        for (std::size_t i=0; i<arrayDimensions.size(); i++)
        {
            uaArr[i] = arrayDimensions[i];
        }
        uaArgument.setArrayDimensions(uaArr);

        UaLocalizedText uaDescription;
        description.toSdk(uaDescription);
        uaArgument.setDescription(uaDescription);
    }

}


