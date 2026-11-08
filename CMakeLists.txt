cmake_minimum_required(VERSION 3.20)

project(HealthcareManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DoctorTest
    src/main.cpp
    src/doctor.cpp
    src/PrimaryIndex.cpp
    src/SecondaryIndex.cpp
    src/AppointmentsFile.cpp
)

target_include_directories(DoctorTest PRIVATE include)

set_property(TARGET DoctorTest PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

file(COPY "${CMAKE_SOURCE_DIR}/data"
     DESTINATION "${CMAKE_BINARY_DIR}")