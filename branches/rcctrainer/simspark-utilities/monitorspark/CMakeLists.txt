########### next target ###############
set(monitorspark_SRCS
   main.cpp
)

add_executable(monitorspark ${monitorspark_SRCS})

target_link_libraries(monitorspark ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES})

set_target_properties(monitorspark PROPERTIES
    INSTALL_RPATH "${RPATH_DIRECTORIES}")

install(TARGETS monitorspark DESTINATION ${BINDIR})


########### install files ###############

install(FILES monitorspark.rb DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
