cmake_minimum_required(VERSION 3.5)

project(RetailSaleApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)

set(PROJECT_SOURCES
        main.cpp
        loginwindow.cpp
        loginwindow.h
        loginwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RetailSaleApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mainwindowclient.h mainwindowclient.cpp mainwindowclient.ui
        mainwindowmanager.h mainwindowmanager.cpp mainwindowmanager.ui
        mainwindowwarehouse.h mainwindowwarehouse.cpp mainwindowwarehouse.ui
        dialog_edit_goods_manager.h dialog_edit_goods_manager.cpp dialog_edit_goods_manager.ui
        dialog_add_goods_manager.h dialog_add_goods_manager.cpp dialog_add_goods_manager.ui
        dialog_discount_manager.h dialog_discount_manager.cpp dialog_discount_manager.ui
        dialog_edit_clients_manager.h dialog_edit_clients_manager.cpp dialog_edit_clients_manager.ui
        dialog_create_order.h dialog_create_order.cpp dialog_create_order.ui
        dialog_view_orders.h dialog_view_orders.cpp dialog_view_orders.ui
        dialog_profile_edit.h dialog_profile_edit.cpp dialog_profile_edit.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RetailSaleApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RetailSaleApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RetailSaleApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(RetailSaleApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RetailSaleApp PRIVATE Qt6::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RetailSaleApp)
endif()
set_target_properties(RetailSaleApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RetailSaleApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RetailSaleApp)
endif()
