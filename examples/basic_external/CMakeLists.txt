cmake_minimum_required(VERSION 3.0)

project(docks_example)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

# This will look for Qt, do find_package yourself manually before 
# if you want to look for a specific version for instance.
find_package(KDDockWidgets REQUIRED)

add_executable(docks_example
    main.cpp
    ExampleDockableWidget.cpp
)

target_link_libraries(docks_example 
    PRIVATE 
        KDAB::kddockwidgets
)

