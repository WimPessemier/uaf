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

#include "uaf/client/subscriptions/datachangenotification.h"





namespace uaf
{
    using namespace uaf;
    using std::string;
    using std::stringstream;
    using std::vector;
    using std::size_t;





    // Constructor
    // =============================================================================================
    DataChangeNotification::DataChangeNotification()
    : MonitoredItemNotification()
    {}


    // Get a string representation
    // =============================================================================================
    string DataChangeNotification::toString(const string& indent, size_t colon) const
    {
        stringstream ss;

        ss << MonitoredItemNotification::toString(indent, colon);
        ss << "\n";
        ss << indent << " - status";
        ss << fillToPos(ss, colon);
        ss << ": " << status.toString() << "\n";
        ss << indent << " - serverTimestamp";
        ss << fillToPos(ss, colon);
        ss << ": " << serverTimestamp.toString() << "\n";
        ss << indent << " - sourceTimestamp";
        ss << fillToPos(ss, colon);
        ss << ": " << sourceTimestamp.toString() << "\n";
        ss << indent << " - data";
        ss << fillToPos(ss, colon);
        ss << ": " << data.toString();
        return ss.str();
    }


    // operator==
    // =============================================================================================
    bool operator==(const DataChangeNotification& object1, const DataChangeNotification& object2)
    {
        return    object1.clientHandle == object2.clientHandle
               && object1.status == object2.status
               && object1.data == object2.data
               && object1.sourceTimestamp == object2.sourceTimestamp
               && object1.serverTimestamp == object2.serverTimestamp;
    }


    // operator!=
    // =============================================================================================
    bool operator!=(const DataChangeNotification& object1, const DataChangeNotification& object2)
    {
        return !(object1 == object2);
    }


    // operator<
    // =============================================================================================
    bool operator<(const DataChangeNotification& object1, const DataChangeNotification& object2)
    {
        if (object1.clientHandle != object2.clientHandle)
            return object1.clientHandle < object2.clientHandle;
        else if (object1.status != object2.status)
            return object1.status < object2.status;
        else if (object1.sourceTimestamp != object2.sourceTimestamp)
            return object1.sourceTimestamp < object2.sourceTimestamp;
        else if (object1.serverTimestamp != object2.serverTimestamp)
            return object1.serverTimestamp < object2.serverTimestamp;
        else
            return object1.data < object2.data;
    }


}




