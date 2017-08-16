# examples/pyuaf/client/how_to_read_and_write_structures.py
"""
EXAMPLE: how to inspect method arguments
====================================================================================================

To run the example, start the UaServerCPP of UnifiedAutomation first on the same machine. 
($SDK/bin/uaservercpp, this executable is part of the SDK).
"""

import time, os, sys

import pyuaf
from pyuaf.client           import Client
from pyuaf.client.settings  import ClientSettings, SessionSettings
from pyuaf.util             import Address, NodeId, ExtensionObject, SdkStatus, RelativePathElement
from pyuaf.util             import QualifiedName, Argument
from pyuaf.util             import GenericStructureValue, GenericStructureVector, GenericUnionValue
from pyuaf.util             import primitives
from pyuaf.util             import opcuaidentifiers, attributeids, opcuatypes, structurefielddatatypes
from pyuaf.util.errors      import UafError


# define the namespace URI and server URI of the UaServerCPP demo server
demoServerUri = "urn:UnifiedAutomation:UaServerCpp"
demoNamespaceUri = "http://www.unifiedautomation.com/DemoServer/"


# define the ClientSettings:
settings = ClientSettings()
settings.applicationName = "MyClient"
settings.discoveryUrls.append("opc.tcp://localhost:48010")

# create the client
myClient = Client(settings)


# we start from the method address: 
methodAdr = Address( NodeId("Demo.Method.VectorAdd", demoNamespaceUri), demoServerUri )

# from the method address, we can derive the address of the input arguments
inArgsAdr = Address( methodAdr , [RelativePathElement(QualifiedName("InputArguments", 0))] )

# read the input args
result = myClient.read( [inArgsAdr] )

target = result.targets[0]

for i in xrange(len(target.data)):
    print("========= Argument %d ==========" % i)
    extensionObject = target.data[i]
    argument = Argument(extensionObject)
    
    print(argument)
    
    print("--------- definition ---------")
    # using the datatypeId, get the definition of the structure
    definition = myClient.structureDefinition(argument.dataType)

    print(definition)


