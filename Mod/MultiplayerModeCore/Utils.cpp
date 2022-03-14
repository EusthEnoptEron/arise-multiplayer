#include "Utils.h"
#include "MultiplayerMod.h"

UE4::UClass* FastGetClass(UE4::UObject* obj) {
	return (UE4::UClass*)((SDK::UObject*)(obj))->Class;
}

UE4::UStruct* FastGetSuperField(UE4::UStruct* obj) {
	return (UE4::UStruct*)((SDK::UStruct*)(obj))->SuperField;
}


bool FastIsA(UE4::UObject* obj, UE4::UClass* cmp)
{
	for (auto super = FastGetClass(obj); super; super = static_cast<UE4::UClass*>(FastGetSuperField(super)))
	{
		if (super == cmp)
		{
			return true;
		}
	}

	return false;
}

std::string GetName(UE4::FFrame& Stack) {
	FScriptName name = { 0 };
	// Get axis name
	auto saveCode = Stack.Code;
	int32_t comp = *(Stack.Code++);
	MultiplayerMod::GNatives[comp](Stack.Object, Stack, &name);
	Stack.Code = saveCode;

	return name.GetName();
}


void PrintStackTrace(UE4::UObject* Context, UE4::FFrame& Stack) {
	Log::Info("%s [%s]", Context->GetName().c_str(), Context->GetClass()->GetName().c_str());

	UE4::FFrame* frame = &Stack;
	while (frame != nullptr) {
		Log::Info("%s::%s", frame->Object->GetName().c_str(), frame->Node->GetName().c_str());
		frame = frame->PreviousFrame;
	}
	Log::Info("");
}

SDK::FTransform GetIdentity() {

	SDK::FTransform identity;
	identity.Rotation = SDK::FQuat();
	identity.Scale3D = SDK::FVector();
	identity.Rotation.W = 1.0f;
	identity.Scale3D.X = identity.Scale3D.Y = identity.Scale3D.Z = 1.0f;

	return identity;
}


SDK::FTransform GetTransform(const SDK::AActor* actor)
{
	auto rootComponent = actor ? actor->RootComponent : nullptr;
	if (rootComponent) {
		auto transformRef = (SDK::FTransform*)(rootComponent->UnknownData02 + 8);
		return SDK::FTransform(*transformRef);
	}
	else {
		static SDK::FTransform identity = GetIdentity();
		return identity;
	}
}

UE4::UFunction* FindFunction(const std::string path)
{
	const auto fn = UE4::UObject::FindObject<UE4::UFunction>(path);
	if (!fn) {
		Log::Error("Function not found: %s", path.c_str());
	}
	return fn;
}
