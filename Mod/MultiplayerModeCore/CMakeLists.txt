project(MultiplayerModeCore)

set(BIGOBJ_FILES
	Libs/Arise-SDK/src/SDK/Arise_functions.cpp
)

set(ARISE_SDK_FILES
	Libs/Arise-SDK/src/SDK/Arise_functions.cpp
	Libs/Arise-SDK/src/SDK/Basic.h
	Libs/Arise-SDK/src/SDK/CoreUObject_classes.h
	Libs/Arise-SDK/src/SDK/CoreUObject_functions.cpp
	Libs/Arise-SDK/src/SDK/CoreUObject_parameters.h
	Libs/Arise-SDK/src/SDK/CoreUObject_structs.h
	Libs/Arise-SDK/src/SDK/Engine_functions.cpp
)

set(UE4_FILES
	../UnrealEngineModLoader/UE4/Basic.cpp
	../UnrealEngineModLoader/UE4/Basic.hpp
	../UnrealEngineModLoader/UE4/CoreUObject_classes.hpp
	../UnrealEngineModLoader/UE4/CoreUObject_functions.cpp
	../UnrealEngineModLoader/UE4/CoreUObject_parameters.hpp
	../UnrealEngineModLoader/UE4/CoreUObject_structs.hpp
	../UnrealEngineModLoader/UE4/Ue4.hpp
)

set(ARISE_MULITPLAYER_MODULE_FILES
	Modules/BaseModule.cpp
	Modules/BaseModule.h
	Modules/BlueprintProxyModule.cpp
	Modules/BlueprintProxyModule.h
	Modules/BoostAttackModule.cpp
	Modules/BoostAttackModule.h
	Modules/CameraModule.cpp
	Modules/CameraModule.h
	Modules/ConfigModule.cpp
	Modules/ConfigModule.h
	Modules/ControlModeModule.cpp
	Modules/ControlModeModule.h
	Modules/InputModule.cpp
	Modules/InputModule.h
	Modules/ModActorModule.cpp
	Modules/ModActorModule.h
	Modules/QualityOfLifeModule.cpp
	Modules/QualityOfLifeModule.h
	Modules/ReroutingModule.cpp
	Modules/ReroutingModule.h
	Modules/StepGuardModule.cpp
	Modules/StepGuardModule.h
	Modules/TestWidgetModule.cpp
	Modules/TestWidgetModule.h
	Modules/VibrationModule.cpp
	Modules/VibrationModule.h
)

set(MULTIPLAYERMODECORE_FILES
	dllmain.cpp
	FileWatch.hpp
	InputManager.cpp
	InputManager.h
	MultiplayerMod.cpp
	MultiplayerMod.h
	SDK.cpp
	Tracer.cpp
	Tracer.h
	Utils.cpp
	Utils.h
	VMTHook.cpp
	VMTHook.h
)

set(MOD_FILES
	${ARISE_SDK_FILES}
	${UE4_FILES}
	${ARISE_MULITPLAYER_MODULE_FILES}
	${MULTIPLAYERMODECORE_FILES}
)
set_source_files_properties(${BIGOBJ_FILES} PROPERTIES COMPILE_OPTIONS "/bigobj")

add_library(${PROJECT_NAME} SHARED ${MOD_FILES})
source_group("Arise-SDK" FILES ${ARISE_SDK_FILES})
source_group("UE4" FILES ${UE4_FILES})
source_group("Arise-Multiplayer-Modules" FILES ${ARISE_MULITPLAYER_MODULE_FILES})
source_group("MultiplayerModeCore" FILES ${MULTIPLAYERMODECORE_FILES})

# definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE)

# includes
target_include_directories(${PROJECT_NAME} PUBLIC Libs/Arise-SDK/src)
target_include_directories(${PROJECT_NAME} PUBLIC Libs/steamworks/public)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# linking
target_link_directories(${PROJECT_NAME} PUBLIC Libs/steamworks/redistributable_bin/win64)
target_link_libraries(${PROJECT_NAME} PUBLIC UnrealEngineModLoader steam_api64)

# install
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION $<CONFIG>/Content/CoreMods
)

install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION $<CONFIG>/Content/CoreMods OPTIONAL)
