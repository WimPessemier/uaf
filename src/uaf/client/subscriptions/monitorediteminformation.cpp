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

#include "uaf/client/subscriptions/monitorediteminformation.h"

namespace uafc
{
    using namespace uaf;
    using namespace uafc;
    using std::string;
    using std::stringstream;
    using std::size_t;


    // Constructor
    // =============================================================================================
    MonitoredItemInformation::MonitoredItemInformation()
    : clientConnectionId(0),
      clientSubscriptionHandle(0),
      clientMonitoredItemHandle(uaf::NOTIFICATIONHANDLE_NOT_ASSIGNED)
    {}


    // Constructor
    // =============================================================================================
    MonitoredItemInformation::MonitoredItemInformation(
            uaf::ClientConnectionId             clientConnectionId,
            uaf::ClientSubscriptionHandle       clientSubscriptionHandle,
            uaf::ClientMonitoredItemHandle      clientMonitoredItemHandle,
            const MonitoredItemSettings&        settings)
    : clientConnectionId(clientConnectionId),
      clientSubscriptionHandle(clientSubscriptionHandle),
      clientMonitoredItemHandle(clientMonitoredItemHandle),
      settings(settings)
    {}

    // Get a new client connection id
    // =============================================================================================
    string MonitoredItemInformation::toString(const string& indent, size_t colon) const
    {
        stringstream ss;

        ss << indent << " - clientConnectionId";
        ss << fillToPos(ss, colon);
        ss << ": " << clientConnectionId << "\n";

        ss << indent << " - clientSubscriptionHandle";
        ss << fillToPos(ss, colon);
        ss << ": " << clientSubscriptionHandle << "\n";

        ss << indent << " - notificationHandle";
        ss << fillToPos(ss, colon) << ": ";
        if (clientMonitoredItemHandle == uaf::NOTIFICATIONHANDLE_NOT_ASSIGNED)
            ss << "NOT ASSIGNED\n";
        else
            ss << clientMonitoredItemHandle << "\n";

        ss << indent << " - settings";
        ss << fillToPos(ss, colon);
        ss << ": " << settings.toString();

        return ss.str();
    }


    // operator==
    // =============================================================================================
    bool operator==(
            const MonitoredItemInformation& object1,
            const MonitoredItemInformation& object2)
    {
        return    object1.clientConnectionId == object2.clientConnectionId
               && object1.clientSubscriptionHandle == object2.clientSubscriptionHandle
               && object1.clientMonitoredItemHandle == object2.clientMonitoredItemHandle
               && object1.settings == object2.settings;
    }


    // operator!=
    // =============================================================================================
    bool operator!=(
            const MonitoredItemInformation& object1,
            const MonitoredItemInformation& object2)
    {
        return !(object1 == object2);
    }


    // operator<
    // =============================================================================================
    bool operator<(
            const MonitoredItemInformation& object1,
            const MonitoredItemInformation& object2)
    {
        if (object1.clientConnectionId != object2.clientConnectionId)
            return object1.clientConnectionId < object2.clientConnectionId;
        else if (object1.clientSubscriptionHandle != object2.clientSubscriptionHandle)
            return object1.clientSubscriptionHandle < object2.clientSubscriptionHandle;
        else if (object1.clientMonitoredItemHandle != object2.clientMonitoredItemHandle)
            return object1.clientMonitoredItemHandle < object2.clientMonitoredItemHandle;
        else
            return object1.settings < object2.settings;
    }
}