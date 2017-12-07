cmake_minimum_required(VERSION 3.7)

project(MutatorProject)

include_directories(
	"source"
	"source/mutator"
)


file(GLOB distormFiles
	"source/distorm/*.c"
	"source/distorm/*.h"
)

file(GLOB asmjitFiles 
	"source/asmjit/*.cpp"
	"source/asmjit/*.h"
	"source/asmjit/base/*.cpp"
	"source/asmjit/base/*.h"
	"source/asmjit/x86/*.cpp"
	"source/asmjit/x86/*.h"
)

file(GLOB asmtkFiles 
	"source/asmtk/*.cpp"
	"source/asmtk/*.h"
)

file(GLOB objeditFiles 
	"source/exeedit/*.cpp"
	"source/exeedit/*.h"
)

file(GLOB clientFiles
	"source/client/*.cpp"
	"source/client/*.h"
)

file(GLOB mutatorFiles
	"source/mutator/*.cpp"
	"source/mutator/*.h"
	"source/mutator/Random/*.cpp"
	"source/mutator/Random/*.h"
	"source/mutator/Schemes/*.cpp"
	"source/mutator/Schemes/*.h"
	"source/mutator/InstructionNodes/*.cpp"
	"source/mutator/InstructionNodes/*.h"
)

file(GLOB jsonFiles
	"source/json/*.cpp"
	"source/json/*.h"
)


add_library(distorm		STATIC ${distormFiles})
add_library(asmjit		STATIC ${asmjitFiles})
add_library(asmtk		STATIC ${asmtkFiles})
add_library(exeedit		STATIC ${objeditFiles})
add_library(mutator		STATIC ${mutatorFiles})
add_library(json		STATIC ${jsonFiles})


add_executable(mutatorClient ${clientFiles})
target_link_libraries(mutatorClient distorm asmjit asmtk exeedit mutator)

set_target_properties(distorm			PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(asmjit			PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(asmtk				PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(exeedit			PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(mutator			PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(json				PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(mutatorClient		PROPERTIES LINKER_LANGUAGE CXX)

add_definitions(-DASMJIT_STATIC)