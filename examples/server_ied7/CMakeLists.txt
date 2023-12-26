include_directories(
   .
)

set(server_example_SRCS
   server_ied7.c
   static_model.c
)

IF(MSVC)
set_source_files_properties(${server_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(server_ied7
  ${server_example_SRCS}
)

target_link_libraries(server_ied7
    iec61850
)
