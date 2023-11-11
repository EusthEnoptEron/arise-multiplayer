#Games Basic Information
[GameInfo]

#Set to 1 (true) if the games engine version is 4.23 and up
UsesFNamePool=0

#Set to 1 (true) if the game engine version is 4.18 and up (this can vary)
IsUsingFChunkedFixedUObjectArray=1

#Fallback if Spawn Actor can't be found or refuses to work. You should almost NEVER use.
IsUsingDeferedSpawn=0

#UE4.22 changes the namepool weird, only set this to 1 if the game uses 4.22
#IsUsing4_22=0

# Pattern Example
[GInfo]
IsGInfoPatterns=1

# 0x132D824
GName=48 83 EC 28 48 8B 05 ?? ?? ?? ?? 48 85 C0 75 7A 48 8B
GNameFirstOpCodes=7
GNameTotalByteInstruction=11

# 0x44F1320
GObject=CC 4C 8B C9 4C 8D 15 ?? ?? ?? ?? 4C 89 11 45 84 C0
GObjectFirstOpCodes=7
GObjectTotalByteInstruction=11

# 0x4C56028
GWorld=88 44 24 48 48 8B D9 48 8B 0D ?? ?? ?? ?? B8 F0 FF 00 00 66
GWorldFirstOpCodes=10
GWorldTotalByteInstruction=14

[FunctionInfo]
IsFunctionPatterns=1

GameStateInit=40 53 48 83 EC 20 48 8B 41 10 48 8B D9 48 8B 91 88 03 00 00 48 89 82 28
BeginPlay=48 89 5C 24 10 48 89 74 24 18 57 41 56 41 57 48 81 EC 00 01 00 00
StaticLoadObject=4C 89 4C 24 20 48 89 54 24 10 48 89 4C 24 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 F9
SpawnActorFTrans=48 8B C4 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 08 FF FF FF
ProcessEvent=40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC F0 00 00 00
CreateDefaultObject=4C 8B DC 57 48 81 EC 00 02 00 00
CallFunctionByNameWithArguments=40 55 53 56 57 41 54 41 55 41 56 41 57 48 81 EC 28 03 00 00