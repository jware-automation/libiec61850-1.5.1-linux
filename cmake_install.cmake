# Install script for directory: C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/libiec61850")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libiec61850" TYPE FILE FILES
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/inc/hal_base.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/inc/hal_time.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/inc/hal_thread.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/inc/hal_filesystem.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/inc/hal_ethernet.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/inc/hal_socket.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/inc/tls_config.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/common/inc/libiec61850_common_api.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/common/inc/linked_list.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/iec61850/inc/iec61850_client.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/iec61850/inc/iec61850_common.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/iec61850/inc/iec61850_server.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/iec61850/inc/iec61850_model.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/iec61850/inc/iec61850_cdc.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/iec61850/inc/iec61850_dynamic_model.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/iec61850/inc/iec61850_config_file_parser.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/mms/inc/mms_value.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/mms/inc/mms_common.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/mms/inc/mms_types.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/mms/inc/mms_type_spec.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/mms/inc/mms_client_connection.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/mms/inc/mms_server.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/mms/inc/iso_connection_parameters.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/goose/goose_subscriber.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/goose/goose_receiver.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/goose/goose_publisher.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/sampled_values/sv_subscriber.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/sampled_values/sv_publisher.h"
    "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/logging/logging_api.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.29.30133/x86/Microsoft.VC142.CRT/msvcp140.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.29.30133/x86/Microsoft.VC142.CRT/msvcp140_1.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.29.30133/x86/Microsoft.VC142.CRT/msvcp140_2.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.29.30133/x86/Microsoft.VC142.CRT/msvcp140_atomic_wait.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.29.30133/x86/Microsoft.VC142.CRT/msvcp140_codecvt_ids.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.29.30133/x86/Microsoft.VC142.CRT/vcruntime140.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.29.30133/x86/Microsoft.VC142.CRT/concrt140.dll"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE DIRECTORY FILES "")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/hal/cmake_install.cmake")
  include("C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/examples/cmake_install.cmake")
  include("C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/src/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/Janith/Documents/jWare Automation/Oren/libiec61850-1.5.1/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
