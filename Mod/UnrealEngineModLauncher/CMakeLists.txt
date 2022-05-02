project(UnrealEngineModLauncher)

set (UNREALENGINEMODLAUNCHER_FILES
	resource.h
	UnrealEngineModLauncher.cpp
	u4mdl_logo_ZfM_icon.ico
)

add_executable(${PROJECT_NAME} ${UNREALENGINEMODLAUNCHER_FILES})
target_sources(${PROJECT_NAME} PRIVATE UnrealEngineModLauncher.rc)
source_group(${PROJECT_NAME} FILES ${UNREALENGINEMODLAUNCHER_FILES} UnrealEngineModLauncher.rc)

# definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE)

# install
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION $<CONFIG>/Binaries/Win64
)

install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION $<CONFIG>/Binaries/Win64 OPTIONAL)
