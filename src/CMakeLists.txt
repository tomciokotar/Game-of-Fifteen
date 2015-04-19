set (SOURCES
 grafika.cpp
 klocek.cpp
 main.cpp
 mainwindow.cpp
 plansza.cpp
 stanplanszy.cpp
)

set (HEADERS
 grafika.h
 klocek.h
 mainwindow.h
 plansza.h
 stanplanszy.h
)

qt4_wrap_cpp (SOURCES ${HEADERS})
add_executable (Przesuwanka ${SOURCES})
target_link_libraries (Przesuwanka ${QT_LIBRARIES})

