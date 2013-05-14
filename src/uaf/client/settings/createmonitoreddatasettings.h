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

#ifndef UAFC_CREATEMONITOREDDATASETTINGS_H_
#define UAFC_CREATEMONITOREDDATASETTINGS_H_



// STD
// SDK
// UAF
#include "uaf/client/clientexport.h"
#include "uaf/client/settings/baseservicesettings.h"



namespace uafc
{

    /*******************************************************************************************//**
    * An uafc::CreateMonitoredDataSettings object holds the service settings that are particular
    * for the CreateMonitoredItems service (used to monitor data changes).
    *
    * @ingroup ClientSettings
    ***********************************************************************************************/
    class UAFC_EXPORT CreateMonitoredDataSettings : public uafc::BaseServiceSettings
    {
    public:

        /**
         * Create default CreateMonitoredData settings.
         */
        CreateMonitoredDataSettings() : uafc::BaseServiceSettings() {}

        // ToDo add timestampstoreturn
    };



}


#endif /* UAFC_CREATEMONITOREDDATASETTINGS_H_ */
