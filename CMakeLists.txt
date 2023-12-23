# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.27)
#set(CMAKE_C_COMPILER "/path/to/your/c/compiler/executable")
#set(CMAKE_CXX_COMPILER "/path/to/your/cpp/compiler/executable")

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  ChessBoard
  VERSION 1.0
  LANGUAGES CXX)

#set(wxWidgets_CONFIG_OPTIONS --toolkit=base --version=3.1)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS net core base)

# Include wxWidgets headers
include(${wxWidgets_USE_FILE})

# Add your target executable
add_executable(ChessBoard ChessBoard.cpp)

# Link against wxWidgets libraries
target_link_libraries(ChessBoard PRIVATE ${wxWidgets_LIBRARIES})

# Set the target property to make it a WIN32 executable
#set_property(TARGET ChessBoard PROPERTY WIN32_EXECUTABLE true)

# Set the subsystem to WINDOWS and the entry point to mainCRTStartup for Visual Studio
set_target_properties(ChessBoard PROPERTIES
  LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)
