
set(substation_simulation_control_app_SRCS
   substation_simulation_control_app.c
)

IF(MSVC)
set_source_files_properties(${substation_simulation_control_app_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(substation_simulation_control_app
  ${substation_simulation_control_app_SRCS}
)

target_link_libraries(substation_simulation_control_app
    iec61850
)
