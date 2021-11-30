#include "Tracer.h"
#include "Utilities/MinHook.h"

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

	tracer->OnEnter(Context->GetName() + "::" + Result.GetName());
	EX_VirtualFunction(Context, Stack, result);
	tracer->OnExit();
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
	auto fn = (UE4::UFunction *)TempCode;

	tracer->OnEnter(Context->GetName() + "::" + fn->GetName());
	EX_FinalFunction(Context, Stack, result);
	tracer->OnExit();
}


void *Tracer::GetPointer(UE4::UFunction* function) {
	return _functionTable[function->GetFullName()];
}


void Tracer::OnEnter(std::string functionName) {
	_file << std::string(_indentation, ' ');
	_file << functionName;
	_file << "\n";

	_indentation += 2;
}

void Tracer::OnExit() {
	_indentation -= 2;
}

void Tracer::Start() {
	if (_isTracing) return;

	_file.open("trace.txt", std::ios::trunc | std::ios::out);
	_isTracing = true;
}

void Tracer::Stop() {
	if (!_isTracing) return;

	_isTracing = false;
	_file.close();
}

void Tracer::Hook()
{
	auto offset = (DWORD64)GetModuleHandleW(0);
	_GNatives = (FNativeFuncPtr*)((DWORD64)(offset + 0x4BC4D80));


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
