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

#ifndef UAF_ARGUMENT_H_
#define UAF_ARGUMENT_H_


// STD
#include <vector>
#include <string>
#include <sstream>
#include <stdint.h>
// SDK
#include "uabase/uaargument.h"
// UAF
#include "uaf/util/util.h"
#include "uaf/util/extensionobject.h"
#include "uaf/util/localizedtext.h"
#include "uaf/util/stringifiable.h"


namespace uaf
{


    /*******************************************************************************************//**
     * An argument can be an input or output argument of a method.
     *
     * @ingroup Util
     **********************************************************************************************/
    class UAF_EXPORT Argument
    {
    public:


        /**
         * Construct an empty argument.
         */
        Argument();


        /**
         * Construct an argument from an ExtensionObject.
         */
        Argument(const uaf::ExtensionObject& extensionObject);

        /**
         * Construct a ByteString with the specified length and the given data.
         *
         * @param length    Number of bytes.
         * @param data      Pointer to the bytes.
         */
        Argument(const std::string& name, const uaf::NodeId& dataType, int32_t valueRank,
                 std::vector<uint32_t> arrayDimensions, const uaf::LocalizedText& description);


        std::string name;
        uaf::NodeId dataType;
        int32_t valueRank;
        std::vector<uint32_t> arrayDimensions;
        uaf::LocalizedText description;


        /**
         * Get a string representation.
         */
        std::string toString(const std::string& indent="", std::size_t colon=18) const;


        /**
         * Get the contents of the Argument from a SDK UaArgument instance.
         *
         * @param uaByteArray  SDK UaArgument instance.
         */
        void fromSdk(const UaArgument& uaArgument);


        /**
         * Copy the contents to an SDK instance.
         *
         * @param uaByteString  SDK UaArgument instance.
         */
        void toSdk(UaArgument& uaArgument) const;


    private:

    };

}



#endif /* UAF_ARGUMENT_H_ */
