cmake_minimum_required(VERSION 3.8)
project(UnrealEngineModLoader)

set(CMAKE_CXX_STANDARD 17) # required by i.e. #include <filesystem>
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Development;Release;MinSizeRel;RelWithDebInfo" CACHE FILEPATH "Build Configurations" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DEVELOPMENT ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_DEVELOPMENT ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})

# TODO: optimize: avoid restricted default folder "C:/Program Files (x86)/..."
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE FILEPATH "Install path prefix, prependen onto install directories." FORCE)

add_subdirectory(UnrealEngineModLoader)
add_subdirectory(UnrealEngineModLauncher)
add_subdirectory(LoaderAutoInjector) # xinput1_3.dll
#add_subdirectory(ExampleMod) # relic from UnrealEngineModLoader

add_subdirectory(MultiplayerModeCore)

# install
install(FILES "ModLoaderInfo.ini"
	DESTINATION $<CONFIG>/Binaries/Win64
)

install(FILES "Profiles/Tales of Arise.profile"
	DESTINATION $<CONFIG>/Binaries/Win64/Profiles
)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../WindowsNoEditor/WindowsNoEditor")
	message(WARNING "WindowsNoEditor contains another WindowsNoEditor folder! Select repository root folder on Unreal Engine export and delete the sub-folder!")
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../WindowsNoEditor/Arise/Content/Paks/pakchunk32-WindowsNoEditor.pak")
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../WindowsNoEditor/Arise/Content/Paks/pakchunk32-WindowsNoEditor.pak"
		DESTINATION $<CONFIG>/Content/Paks/LogicMods RENAME MultiplayerMod.pak
	)
endif()
