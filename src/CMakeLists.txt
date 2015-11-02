set (SOURCES
 graphics.cpp
 tile.cpp
 main.cpp
 mainWindow.cpp
 board.cpp
 boardState.cpp
)

set (HEADERS
 graphics.h
 tile.h
 mainWindow.h
 board.h
 boardState.h
)

qt4_wrap_cpp (SOURCES ${HEADERS})
add_executable (Przesuwanka ${SOURCES})
target_link_libraries (Przesuwanka ${QT_LIBRARIES})

