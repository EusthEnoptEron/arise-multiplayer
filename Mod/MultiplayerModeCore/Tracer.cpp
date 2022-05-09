#include "Tracer.h"
#include "Utilities/MinHook.h"
#include "Utils.h"

#define MAX_uint16		((uint16)	0xffff)

Tracer* Tracer::_Instance;
FNativeFuncPtr* Tracer::_GNatives;

FNativeFuncPtr EX_VirtualFunction;


void EX_VirtualFunctionHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	auto tracer = Tracer::GetInstance();

	static auto thread_id = std::this_thread::get_id();
	auto current_thread_id = std::this_thread::get_id();

	if (current_thread_id != thread_id || !tracer->IsTracing()) {
		EX_VirtualFunction(Context, Stack, result);
		return;
	}

	FScriptName Result;
	Result = *(FScriptName*)Stack.Code;

	auto fnName = Context->GetName() + "::" + Result.GetName();
	auto fn = tracer->GetFunction((SDK::UObject *)Context, Result.GetName());
	std::string params = "(" + tracer->GetParams(fn, sizeof(FScriptName), Context, Stack, result) + ")";

	if (Stack.PreviousFrame == nullptr) {
		tracer->OnEnter(Stack.Object->GetName() + "::" + Stack.Node->GetName() + "()");
	}
	tracer->OnEnter(fnName + params);
	auto begin = std::chrono::high_resolution_clock::now();
	EX_VirtualFunction(Context, Stack, result);
	auto end = std::chrono::high_resolution_clock::now();

	std::string suffix = "";
	if (fn != nullptr && fn->ReturnValueOffset != MAX_uint16) {
		for (UPropertyEx* Property = (UPropertyEx*)fn->Children;  Property && (Property->PropertyFlags & (CPF_Parm)) == CPF_Parm; Property = (UPropertyEx*)Property->Next) {
			if ((Property->PropertyFlags & (CPF_ReturnParm)) == CPF_ReturnParm) {

				suffix = " => " + tracer->ToString((SDK::UProperty*)Property, result);
			}
		}
	}


	tracer->OnExit(fnName, suffix, std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
	if (Stack.PreviousFrame == nullptr) {
		tracer->OnExit(Stack.Object->GetName() + "::" + Stack.Node->GetName(), "", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
	}
}

FNativeFuncPtr EX_FinalFunction;
void EX_FinalFunctionHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	auto tracer = Tracer::GetInstance();

	static auto thread_id = std::this_thread::get_id();
	auto current_thread_id = std::this_thread::get_id();

	if (current_thread_id != thread_id || !tracer->IsTracing()) {
		EX_FinalFunction(Context, Stack, result);
		return;
	}

	uint64_t TempCode;
	TempCode = *(uint64_t*)Stack.Code;
	auto fn = (SDK::UFunction *)TempCode;

	auto fnName = Context->GetName() + "::" + fn->GetName();
	std::string params = "(" + tracer->GetParams(fn, sizeof(uint64_t), Context, Stack, result) + ")";

	if (Stack.PreviousFrame == nullptr) {
		tracer->OnEnter(Stack.Object->GetName() + "::" + Stack.Node->GetName() + "()");
	}
	tracer->OnEnter(fnName + params);
	auto begin = std::chrono::high_resolution_clock::now();
	EX_FinalFunction(Context, Stack, result);
	auto end = std::chrono::high_resolution_clock::now();

	std::string suffix = "";
	if (fn != nullptr && fn->ReturnValueOffset != MAX_uint16) {
		for (UPropertyEx* Property = (UPropertyEx*)fn->Children; Property && (Property->PropertyFlags & (CPF_Parm)) == CPF_Parm; Property = (UPropertyEx*)Property->Next) {
			if ((Property->PropertyFlags & (CPF_ReturnParm)) == CPF_ReturnParm) {
				suffix = " => " + tracer->ToString((SDK::UProperty*)Property, result);
			}
		}
	}

	tracer->OnExit(fnName, suffix, std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
		if (Stack.PreviousFrame == nullptr) {
		tracer->OnExit(Stack.Object->GetName() + "::" + Stack.Node->GetName(), "", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
	}
}


FNativeFuncPtr EX_CallMath;
void EX_CallMathHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	auto tracer = Tracer::GetInstance();

	static auto thread_id = std::this_thread::get_id();
	auto current_thread_id = std::this_thread::get_id();

	if (current_thread_id != thread_id || !tracer->IsTracing()) {
		EX_FinalFunction(Context, Stack, result);
		return;
	}

	uint64_t TempCode;
	TempCode = *(uint64_t*)Stack.Code;
	auto fn = (SDK::UFunction*)TempCode;

	auto fnName = Context->GetName() + "::" + fn->GetName();
	std::string params = "(" + tracer->GetParams(fn, sizeof(uint64_t), Context, Stack, result) + ")";

	if (Stack.PreviousFrame == nullptr) {
		tracer->OnEnter(Stack.Object->GetName() + "::" + Stack.Node->GetName() + "()");
	}
	tracer->OnEnter(fnName + params);
	auto begin = std::chrono::high_resolution_clock::now();
	EX_CallMath(Context, Stack, result);
	auto end = std::chrono::high_resolution_clock::now();

	std::string suffix = "";
	if (fn != nullptr && fn->ReturnValueOffset != MAX_uint16) {
		for (UPropertyEx* Property = (UPropertyEx*)fn->Children; Property && (Property->PropertyFlags & (CPF_Parm)) == CPF_Parm; Property = (UPropertyEx*)Property->Next) {
			if ((Property->PropertyFlags & (CPF_ReturnParm)) == CPF_ReturnParm) {
				suffix = " => " + tracer->ToString((SDK::UProperty*)Property, result);
			}
		}
	}

	tracer->OnExit(fnName, suffix, std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
	if (Stack.PreviousFrame == nullptr) {
		tracer->OnExit(Stack.Object->GetName() + "::" + Stack.Node->GetName(), "", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
	}
}


void *Tracer::GetPointer(UE4::UFunction* function) {
	return _functionTable[function->GetFullName()];
}


void Tracer::OnEnter(std::string functionName) {
	_instructionCounter++;

	_file << std::string(_instructionStack.size() * 2, ' ');
	_file << functionName << "; {\n";
	_instructionStack.push(_instructionCounter);
}

void Tracer::OnExit(std::string functionName, std::string suffix, long durationNano) {
	uint64_t counter = _instructionStack.top();
	_instructionStack.pop();

	if (counter != _instructionCounter) {
		// Counter changed, so we had child instructions
		_file << std::string(_instructionStack.size() * 2, ' ');
		_file << "} ";
	}
	else {
		// Counter didn't change, so put on same line
		_file.seekp(-static_cast<int32>(std::string(" {\n").size()), _file.cur);
	}

	_file << "//";
	_file << suffix;
	_file << " (";
	_file << (durationNano * 1.0E-6);
	_file << "ms)";
	_file << "\n";
}

void Tracer::Start() {
	if (_isTracing) return;

	// Reset
	_instructionStack.empty();
	_instructionCounter = 0;

	_file.open("trace.txt", std::ios::trunc | std::ios::out);
	_isTracing = true;
}

void Tracer::Stop() {
	if (!_isTracing) return;

	_isTracing = false;
	_file.close();
}

void Tracer::OnBeginTick() {
	if (!_isTracing) return;

	OnEnter("Tick()");
	_tickStartTime = std::chrono::high_resolution_clock::now();
}

void Tracer::OnEndTick() {
	if (!_isTracing || _instructionStack.size() == 0) return;

	auto end = std::chrono::high_resolution_clock::now();

	OnExit("Tick()", "", std::chrono::duration_cast<std::chrono::nanoseconds>(end - _tickStartTime).count());
}

void Tracer::OnEvent(std::string evt) {
	_file << std::string(_instructionStack.size() * 2, ' ');
	_file << evt;
	_file << "\n";
}

void Tracer::Hook()
{
	//auto offset = (DWORD64)GetModuleHandleW(0);
	//_GNatives = (FNativeFuncPtr*)((DWORD64)(offset + 0x4BC4D80));
	_GNatives = MultiplayerMod::GNatives;


	MinHook::Add((DWORD_PTR)_GNatives[0x1B],
		&EX_VirtualFunctionHook,
		&EX_VirtualFunction,
		"EX_VirtualFunction"
	);

	MinHook::Add((DWORD_PTR)_GNatives[0x1C],
		&EX_FinalFunctionHook,
		&EX_FinalFunction,
		"EX_FinalFunction"
	);

	MinHook::Add((DWORD_PTR)_GNatives[0x68],
		&EX_CallMathHook,
		&EX_CallMath,
		"EX_CallMath"
	);

	
	//for (int i = 0; i < UE4::UObject::GObjects->GetAsChunckArray().Num(); i++)
	//{
	//	auto fn = make::function([](UE4::UObject *Context, UE4::FFrame *Stack, void *result) {  });

	//	auto obj = UE4::UObject::GObjects->GetAsChunckArray().GetByIndex(i).Object;
	//	if (obj != nullptr && obj->IsA(UE4::UFunction::StaticClass())) {
	//		auto ufn = (UE4::UFunction *)obj;
	//		void* ufnPointer = ufn->GetFunction();
	//		if (ufnPointer != processInternals) {
	//			Log::Info("HOOKING %s (%p)", ufn->GetFullName().c_str(), ufnPointer);
	//			void* originalFn;

	//			MinHook::Add((DWORD_PTR)ufnPointer,
	//				&ConstHook,
	//				&originalFn,
	//				ufn->GetFullName()
	//			);

	//			_functionTable[ufn->GetFullName()] = originalFn;
	//			//break;
	//		}
	//	}
	//}
}


std::string Tracer::ToString(SDK::UProperty* prop, void *result) {
	if (result == nullptr) return "NULL";

	//Log::Info("=> %s", Property->Class->GetName());
	if (prop->IsA(SDK::UBoolProperty::StaticClass())) {
		return (*(bool*)result) ? "true" : "false";
	}
	else if(prop->IsA(SDK::UFloatProperty::StaticClass())) {
		return std::to_string((*(float*)result));
	}
	else if (prop->IsA(SDK::UDoubleProperty::StaticClass())) {
		return std::to_string((*(double*)result));
	}
	else if (prop->IsA(SDK::UIntProperty::StaticClass())) {
		return std::to_string((*(int32*)result));
	}
	else if (prop->IsA(SDK::UUInt32Property::StaticClass())) {
		return std::to_string((*(uint32*)result));
	}
	else if (prop->IsA(SDK::UInt64Property::StaticClass())) {
		return std::to_string((*(int64*)result));
	}
	else if (prop->IsA(SDK::UInt8Property::StaticClass())) {
		return std::to_string((*(int8*)result));
	}
	else if (prop->IsA(SDK::UByteProperty::StaticClass())) {
		return std::to_string((*(uint8*)result));
	}
	else if (prop->IsA(SDK::UStructProperty::StaticClass())) {
		auto Struct = ((UStructPropertyEx*)prop)->Struct;
		auto name = Struct->GetName();
		if (name == "Vector") {
			auto v = *(SDK::FVector*)result;
			return "[" + std::to_string(v.X) + ", " + std::to_string(v.Y) + ", " + std::to_string(v.Z) + "]";
		} else if (name == "Vector2D") {
			auto v = *(SDK::FVector2D*)result;
			return "[" + std::to_string(v.X) + ", " + std::to_string(v.Y) + "]";
		} else if (name == "Rotator") {
			auto r = *(SDK::FRotator*)result;
			return "[" + std::to_string(r.Pitch) + ", " + std::to_string(r.Yaw) + ", " + std::to_string(r.Roll) + "]";
		}

		return name + " (Struct)";
	}
	else if (prop->IsA(SDK::UObjectProperty::StaticClass())) {
		auto pointer = *(SDK::UObject**)result;
		if (pointer != nullptr) {
			auto name = pointer->Name;
			return name.GetName();
		}
		else {
			return "NULL";
		}
	}
	else if (prop->IsA(SDK::UStrProperty::StaticClass())) {
		auto str = (SDK::FString*)result;
		if (str->IsValid()) {
			return str->ToString();
		}
		else {
			return "";
		}
	}
	else if (prop->IsA(SDK::UNameProperty::StaticClass())) {
		return ((SDK::FName*)result)->GetName();
	}
	else if (prop->IsA(SDK::UEnumProperty::StaticClass())) {
		return ToString(((UEnumPropertyEx*)prop)->UnderlyingProp, result);
	}
	else if (prop->IsA(SDK::UEnum::StaticClass())) {
		return std::to_string((*(uint8*)result));
	}
	else {
		return "??? ("+ (prop->Class != nullptr ? prop->Class->GetName() : "???") + ")";
	}
	//fnName = fnName + " => ???";
	//break;
}

SDK::UFunction* Tracer::GetFunction(SDK::UObject* owner, std::string name)
{
	std::string fullName = owner->GetName() + "::" + name;

	auto result = _fnTable.find(fullName);
	if (result != _fnTable.end()) {
		return result->second;
	}

	auto chunkArray = UE4::UObject::GObjects->GetAsChunckArray();
	for (int i = 0; i < chunkArray.Num(); ++i)
	{
		auto object = (SDK::UObject*)chunkArray.GetByIndex(i).Object;

		if (object == nullptr)
		{
			continue;
		}

		if (object->Outer == owner->Class)
		{
			if (object->GetName() == name)
			{
				_fnTable[fullName] = (SDK::UFunction*)object;
				return (SDK::UFunction*)object;
			}
		}

	}

	_fnTable[fullName] = nullptr;
	return nullptr;
}

std::string Tracer::GetParams(SDK::UFunction* Function, uint32 codeOffset, UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	std::string params = "";

	if (Function != nullptr && Function->NumParms > 0 && (((UPropertyEx*)Function->Children)->PropertyFlags & CPF_ReturnParm) == 0) {
		auto SaveCode = Stack.Code;

		//Log::Info("FN %s", fnName.c_str());

		uint8* Frame = (uint8*)calloc(1, Function->PropertySize);
		//Log::Info("Created memory: %p (%d)", memory, fn->PropertySize);

		// Step over function pointer
		Stack.Code += codeOffset;
		//Log::Info(fnName);
		for (auto Property = (UPropertyEx*)Function->Children; *Stack.Code != EX_EndFunctionParms; Property = (UPropertyEx*)Property->Next)
		{
			//Log::Info("%d", (int32)*Stack.Code);
			Stack.MostRecentPropertyAddress = NULL;

			// Skip the return parameter case, as we've already handled it above
			const bool bIsReturnParam = ((Property->PropertyFlags & CPF_ReturnParm) != 0);
			if (bIsReturnParam)
			{
				continue;
			}

			int32 B = *Stack.Code++;
			if (Property->PropertyFlags & CPF_OutParm)
			{
				//Log::Info("Out %p", B);
				MultiplayerMod::GNatives[B](Stack.Object, Stack, NULL);

				if (Stack.MostRecentPropertyAddress != nullptr && (Property->PropertyFlags & CPF_ReferenceParm) != 0) {
					params += ToString((SDK::UProperty*)Property, Stack.MostRecentPropertyAddress) + ", ";
				}
			}
			else {
				uint8* addr = Frame + Property->Offset_Internal;
				if (Property->PropertyFlags & CPF_ZeroConstructor) {
					//Log::Info("Reset: %p (%d)", addr, Property->ArrayDim * Property->ElementSize);
					memset(addr, 0, Property->ArrayDim * Property->ElementSize);
					//Log::Info("MemsetA %d", Property->ArrayDim * Property->ElementSize);
				}
				else {
					// InitializeValueInternal(ContainerPtrToValuePtr<void>(Dest));
					//Log::Info("OH SHIT");
					memset(addr, 0, Property->ArrayDim * Property->ElementSize);
					//Log::Info("MemsetB %d", Property->ArrayDim * Property->ElementSize);
				}

				//Log::Info("b:%p (%d) (%p) (%s)", addr, (int64)Property->ArrayDim * Property->ElementSize, MultiplayerMod::GNatives[B], Property->GetName().c_str());
				//Log::Info("GNatives: %p", B);
				MultiplayerMod::GNatives[B](Stack.Object, Stack, addr);
				params += ToString((SDK::UProperty*)Property, addr) + ", ";
				//Log::Info("a:ok");
			}
		}

		Stack.Code = SaveCode;
		free(Frame);
	}

	if (params.length() > 0) {
		params = std::string(params.begin(), params.end() - 2);
	}
	return params;
}