﻿# CMakeList.txt: проект CMake для server 2.0; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("server 2.0")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CMakeTarget


	"headers/Fabric/AnswerManager.h"
	"headers/Fabric/fabric.h" 
	"headers/Fabric/threadSafeFixedQueue.h" 
	"headers/Fabric/worker.h" 

	"headers/Memory/PoolManager.h"


	"headers/TaskContainer/TaskContainer.h"

	"headers/ASSERT.h"
	"headers/GlobalInclude.h"
	"headers/APITaskManager.h"





	"src/AnswerManager.cpp"
	"src/APITaskManager.cpp"
	"src/fabric.cpp"
	"src/GlobalInclude.cpp" 
	"src/PoolManager.cpp"
	"src/threadSafeFixedQueue.cpp" 
	"src/worker.cpp" 
	"src/TaskContainer.cpp"


	"server.cpp"
	"server.h"
   "tests/AADSMonteCarlo.h" "tests/AADSMonteCarlo.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
