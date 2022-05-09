cmake_minimum_required(VERSION 3.8)
project(UnrealEngineModLoader)

set(CMAKE_CXX_STANDARD 17) # required by i.e. #include <filesystem>
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Development;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "Build Configurations" FORCE)
# TODO: optimize: avoid restricted default folder "C:/Program Files (x86)/..."
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Install path prefix, prependend onto install directories." FORCE)

add_subdirectory(UnrealEngineModLoader)
add_subdirectory(UnrealEngineModLauncher)
add_subdirectory(LoaderAutoInjector) # xinput1_3.dll
#add_subdirectory(ExampleMod) # relic from UnrealEngineModLoader

add_subdirectory(MultiplayerModeCore)

set(CMAKE_ASM_MASM_FLAGS_DEVELOPMENT "${CMAKE_ASM_MASM_FLAGS_RELEASE}" CACHE STRING "Flags used by the ASM_MASM compiler during DEVELOPMENT builds.")
set(CMAKE_CXX_FLAGS_DEVELOPMENT "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "Flags used by the CXX compiler during DEVELOPMENT builds.")
set(CMAKE_C_FLAGS_DEVELOPMENT "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "Flags used by the C compiler during DEVELOPMENT builds.")
set(CMAKE_EXE_LINKER_FLAGS_DEVELOPMENT "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "Flags used by the linker during DEVELOPMENT builds.")
set(CMAKE_MODULE_LINKER_FLAGS_DEVELOPMENT "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}" CACHE STRING "Flags used by the linker during the creation of modules during DEVELOPMENT builds.")
set(CMAKE_RC_FLAGS_DEVELOPMENT "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "Flags Windows Resource Compiler during DEVELOPMENT builds.")
set(CMAKE_SHARED_LINKER_FLAGS_DEVELOPMENT "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "Flags used by the linker during the creation of shared libraries during DEVELOPMENT builds.")
set(CMAKE_STATIC_LINKER_FLAGS_DEVELOPMENT "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "Flags used by the linker during the creation of static libraries during DEVELOPMENT builds.")

# install
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/ModLoaderInfo.ini"
	DESTINATION $<CONFIG>/Binaries/Win64
)

set(TALES_OF_ARISE_PROFILE "${CMAKE_CURRENT_SOURCE_DIR}/Profiles/Tales of Arise.profile" CACHE FILEPATH "'Tales of Arise.profile', which shall be copied to Profiles subfolder.")
install(FILES ${TALES_OF_ARISE_PROFILE}
	DESTINATION $<CONFIG>/Binaries/Win64/Profiles/
	RENAME "Tales of Arise.profile"
)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../WindowsNoEditor/WindowsNoEditor")
	message(WARNING "WindowsNoEditor contains another WindowsNoEditor folder! Select repository root folder on Unreal Engine export and delete the sub-folder!")
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../WindowsNoEditor/Arise/Content/Paks/pakchunk32-WindowsNoEditor.pak")
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../WindowsNoEditor/Arise/Content/Paks/pakchunk32-WindowsNoEditor.pak"
		DESTINATION $<CONFIG>/Content/Paks/LogicMods RENAME MultiplayerMod.pak
	)
endif()
