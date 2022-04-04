QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    custombtn.cpp \
    main.cpp \
#    rtdb/comm.cpp \
#    rtdb/multicast.cpp \
    rtdb/rtdb_api.cpp \
    slidepage.cpp \
    subwindow.cpp \
    tcpclient.cpp \
    window.cpp

HEADERS += \
    custombtn.h \
    rtdb/include/core/Angle.hpp \
    rtdb/include/core/Circle.hpp \
    rtdb/include/core/DPoint.hpp \
    rtdb/include/core/Line.hpp \
    rtdb/include/core/PPoint.hpp \
    rtdb/include/core/core.hpp \
    rtdb/include/core/time.hpp \
    rtdb/include/rtdb/multicast.h \
    rtdb/include/rtdb/rtdb_api.h \
    rtdb/include/rtdb/rtdb_comm.h \
    rtdb/include/rtdb/rtdb_defs.h \
    rtdb/include/rtdb/rtdb_user.h \
    rtdb/include/world_model/MTTracker.h \
    rtdb/include/world_model/ball.h \
    rtdb/include/world_model/common.h \
    rtdb/include/world_model/iAuction.h \
    rtdb/include/world_model/multicast.h \
    rtdb/include/world_model/obstacles.h \
    rtdb/include/world_model/robot.h \
    rtdb/include/world_model/strategyinfo.h \
    rtdb/include/world_model/teammatesinfo.h \
    rtdb/include/world_model/world_model.h \
    slidepage.h \
    subwindow.h \
    tcpclient.h \
    window.h

FORMS += \
    window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    config/comm.conf \
    config/example.conf \
    config/nubot.conf \
    rtdb/CMakeFiles/CMakeDirectoryInformation.cmake \
    rtdb/CMakeFiles/comm.dir/DependInfo.cmake \
    rtdb/CMakeFiles/comm.dir/build.make \
    rtdb/CMakeFiles/comm.dir/cmake_clean.cmake \
    rtdb/CMakeFiles/comm.dir/depend.make \
    rtdb/CMakeFiles/comm.dir/flags.make \
    rtdb/CMakeFiles/comm.dir/link.txt \
    rtdb/CMakeFiles/comm.dir/progress.make \
    rtdb/CMakeFiles/multicast.dir/DependInfo.cmake \
    rtdb/CMakeFiles/multicast.dir/build.make \
    rtdb/CMakeFiles/multicast.dir/cmake_clean.cmake \
    rtdb/CMakeFiles/multicast.dir/cmake_clean_target.cmake \
    rtdb/CMakeFiles/multicast.dir/depend.make \
    rtdb/CMakeFiles/multicast.dir/flags.make \
    rtdb/CMakeFiles/multicast.dir/link.txt \
    rtdb/CMakeFiles/multicast.dir/progress.make \
    rtdb/CMakeFiles/progress.marks \
    rtdb/CMakeFiles/read.dir/DependInfo.cmake \
    rtdb/CMakeFiles/read.dir/build.make \
    rtdb/CMakeFiles/read.dir/cmake_clean.cmake \
    rtdb/CMakeFiles/read.dir/depend.make \
    rtdb/CMakeFiles/read.dir/flags.make \
    rtdb/CMakeFiles/read.dir/link.txt \
    rtdb/CMakeFiles/read.dir/progress.make \
    rtdb/CMakeFiles/rtdb.dir/DependInfo.cmake \
    rtdb/CMakeFiles/rtdb.dir/build.make \
    rtdb/CMakeFiles/rtdb.dir/cmake_clean.cmake \
    rtdb/CMakeFiles/rtdb.dir/cmake_clean_target.cmake \
    rtdb/CMakeFiles/rtdb.dir/depend.make \
    rtdb/CMakeFiles/rtdb.dir/flags.make \
    rtdb/CMakeFiles/rtdb.dir/link.txt \
    rtdb/CMakeFiles/rtdb.dir/progress.make \
    rtdb/CMakeFiles/write.dir/DependInfo.cmake \
    rtdb/CMakeFiles/write.dir/build.make \
    rtdb/CMakeFiles/write.dir/cmake_clean.cmake \
    rtdb/CMakeFiles/write.dir/depend.make \
    rtdb/CMakeFiles/write.dir/flags.make \
    rtdb/CMakeFiles/write.dir/link.txt \
    rtdb/CMakeFiles/write.dir/progress.make \
    rtdb/CMakeLists.txt \
    rtdb/CTestTestfile.cmake \
    rtdb/cmake_install.cmake \
    rtdb/include/rtdb/rtdb.ini \
    rtdb/parser/CMakeFiles/CMakeDirectoryInformation.cmake \
    rtdb/parser/CMakeFiles/progress.marks \
    rtdb/parser/CMakeFiles/xrtdb.dir/DependInfo.cmake \
    rtdb/parser/CMakeFiles/xrtdb.dir/build.make \
    rtdb/parser/CMakeFiles/xrtdb.dir/cmake_clean.cmake \
    rtdb/parser/CMakeFiles/xrtdb.dir/depend.make \
    rtdb/parser/CMakeFiles/xrtdb.dir/flags.make \
    rtdb/parser/CMakeFiles/xrtdb.dir/link.txt \
    rtdb/parser/CMakeFiles/xrtdb.dir/progress.make \
    rtdb/parser/CMakeLists.txt \
    rtdb/parser/CTestTestfile.cmake \
    rtdb/parser/README \
    rtdb/parser/cmake_install.cmake
