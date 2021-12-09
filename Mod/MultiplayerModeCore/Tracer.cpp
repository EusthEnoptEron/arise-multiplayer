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

	auto fnName = Context->GetName() + "::" + Result.GetName();
	tracer->OnEnter(fnName);
	auto begin = std::chrono::high_resolution_clock::now();
	EX_VirtualFunction(Context, Stack, result);
	auto end = std::chrono::high_resolution_clock::now();

	tracer->OnExit(fnName, std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
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

	auto fnName = Context->GetName() + "::" + fn->GetName();
	tracer->OnEnter(fnName);
	auto begin = std::chrono::high_resolution_clock::now();
	EX_FinalFunction(Context, Stack, result);
	auto end = std::chrono::high_resolution_clock::now();

	tracer->OnExit(fnName, std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
}


void *Tracer::GetPointer(UE4::UFunction* function) {
	return _functionTable[function->GetFullName()];
}


void Tracer::OnEnter(std::string functionName) {
	_instructionCounter++;

	_file << std::string(_instructionStack.size() * 2, ' ');
	_file << functionName;
	_file << "\n";

	_instructionStack.push(_instructionCounter);
}

void Tracer::OnExit(std::string functionName, long durationNano) {
	uint64_t counter = _instructionStack.top();
	_instructionStack.pop();

	if (counter != _instructionCounter) {
		// Counter changed, so we had child instructions
		_file << std::string(_instructionStack.size() * 2, ' ');
		_file << "End " << functionName << " (";
		_file << (durationNano * 1.0E-6);
		_file << "ms)";
		_file << "\n";
	}
	else {
		// Counter didn't change, so put on same line
		_file.seekp(-2, _file.cur);
		_file << " (";
		_file << (durationNano * 1.0E-6);
		_file << "ms)";
		_file << "\n";
	}

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

	OnEnter("Tick");
	_tickStartTime = std::chrono::high_resolution_clock::now();
}

void Tracer::OnEndTick() {
	if (!_isTracing || _instructionStack.size() == 0) return;

	auto end = std::chrono::high_resolution_clock::now();

	OnExit("Tick", std::chrono::duration_cast<std::chrono::nanoseconds>(end - _tickStartTime).count());
}

void Tracer::OnEvent(std::string evt) {
	_file << std::string(_instructionStack.size() * 2, ' ');
	_file << evt;
	_file << "\n";
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
