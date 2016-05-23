
cmake_minimum_required(VERSION 2.8)

# Задание атрибутов приложения
set(PRJ_NAME source_accounting)
project(${PRJ_NAME})
cmake_policy(SET CMP0020 NEW)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core  REQUIRED)
find_package(Qt5Gui  REQUIRED)
find_package(Qt5Widgets  REQUIRED)
find_package(Qt5Sql  REQUIRED)
find_package(Qt5Network  REQUIRED)

# Формирование списка заголовочных файлов
set(SRC_H
	src/view/Combo_delegate.h
	src/view/MainWindow.h
	src/view/Multilist.h
	src/view/TreeRegions.h
	src/view/ViewDepartments.h
	src/view/ViewSites.h
	
		
	src/core/Department.h
	src/core/DepartmentRegion.h
	src/core/DepartmentType.h
	src/core/Format.h
	src/core/GeodataType.h
	src/core/Language.h
	src/core/Log.h
	src/core/Region.h
	src/core/Scale.h
	src/core/Session.h
	src/core/Site.h
	src/core/SiteLang.h
	src/core/SiteRegion.h
	src/core/SiteType.h
	src/core/State.h
	src/core/Status.h
	src/core/User.h
	src/core/Usertype.h
	
	src/model/Base_item.h
	src/model/Departments.h
	src/model/Item_factory.h
	src/model/Item_model.h
	src/model/RegionItem.h
	src/model/RegionItemChecked.h
	src/model/Resources.h
	src/model/Types.h

	src/database/Database.h
)

# Формирование списка файлов с исходным кодом
set(SRC_C
	src/main.cpp
	
	src/view/Combo_delegate.cpp
	src/view/MainWindow.cpp
	src/view/Multilist.cpp
	src/view/TreeRegions.cpp
	src/view/ViewDepartments.cpp
	src/view/ViewSites.cpp
	
	src/core/Department.cpp
	src/core/DepartmentRegion.cpp
	src/core/DepartmentType.cpp
	src/core/Format.cpp
	src/core/GeodataType.cpp
	src/core/Language.cpp
	src/core/Log.cpp
	src/core/Region.cpp
	src/core/Scale.cpp
	src/core/Session.cpp
	src/core/Site.cpp
	src/core/SiteLang.cpp
	src/core/SiteRegion.cpp
	src/core/SiteType.cpp
	src/core/State.cpp
	src/core/Status.cpp
	src/core/User.cpp
	src/core/Usertype.cpp
	
	src/model/Base_item.cpp
	src/model/Departments.cpp
	src/model/Item_factory.cpp
	src/model/Item_model.cpp
	src/model/RegionItem.cpp
	src/model/RegionItemChecked.cpp
	src/model/Resources.cpp

	src/database/Database.cpp
)

set(SRC_UI
	src/ui/MainWindow.ui
	src/ui/TreeRegions.ui
	src/ui/ViewDepartments.ui
	src/ui/ViewSites.ui
)

set(SRC_RCC
	src/resources/ViewWindowRes.qrc
)

# Инструкции по сборке
include_directories (
	.
	src
	src/core
	src/model
	src/view
	src/database
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Sql_INCLUDE_DIRS}
	${Qt5Network_INCLUDE_DIRS}
)

set(GUI_TYPE WIN32)
qt5_add_resources(SRC_RES ${SRC_RCC})
qt5_wrap_ui(_UI ${SRC_UI})

add_definitions(-D_USE_MATH_DEFINES)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
add_definitions(-DLINUXAPI -std=c++11 )
else()
add_definitions(-DWIN32API )
endif()


add_executable(${PRJ_NAME}  ${SRC_H} ${_UI} ${SRC_C} ${SRC_RES})

if(MSVC)
target_link_libraries(
	${PRJ_NAME}
	Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Network
)

else()
target_link_libraries(
	${PRJ_NAME}
	Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Network
)
endif()

set_target_properties( ${PRJ_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${GEODEV_INSTALL_PATH}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GEODEV_INSTALL_PATH}
)

#INSTALL(TARGETS ${PRJ_NAME} DESTINATION ${GEODEV_INSTALL_PATH})
