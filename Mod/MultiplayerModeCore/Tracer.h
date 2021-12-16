#pragma once
#include "iostream"
#include "Mod/Mod.h"
#include <fstream>
#include "MultiplayerMod.h"
#include <stack>


const uint64_t CPF_Parm = 0x0000000000000080;
const uint64_t CPF_ReturnParm = 0x0000000000000400;
const uint64_t CPF_OutParm = 0x0000000000000100;
const uint64_t CPF_ReferenceParm = 0x0000000008000000;
const uint64_t CPF_ZeroConstructor = 0x0000000000000200;
const uint8_t EX_EndFunctionParms = 0x16;
const uint8_t FUNC_Native = 0x00000400;

struct UPropertyEx : SDK::UField {
public:
    // Persistent variables.
    int32			ArrayDim;
    int32			ElementSize;
    uint64	PropertyFlags;
    uint16			RepIndex;

    uint8 BlueprintReplicationCondition;
    int32		Offset_Internal;

    unsigned char UnknownData00[41];                                      // 0x0030(0x0040) MISSED OFFSET

};

struct UEnumPropertyEx : UPropertyEx {
public:
    SDK::UNumericProperty* UnderlyingProp; // The property which represents the underlying type of the enum
    SDK::UEnum* Enum; //
};

struct UStructPropertyEx : UPropertyEx {
public:
    SDK::UScriptStruct* Struct; //
};


template <typename CT, typename ... A> struct function
    : public function<decltype(&CT::operator())(A...)> {};

template <typename C> struct function<C> {
private:
    C mObject;

public:
    function(const C& obj)
        : mObject(obj) {}

    template<typename... Args> typename
        std::result_of<C(Args...)>::type operator()(Args... a) {
        return this->mObject.operator()(a...);
    }

    template<typename... Args> typename
        std::result_of<const C(Args...)>::type operator()(Args... a) const {
        return this->mObject.operator()(a...);
    }
};

namespace make {
    template<typename C> auto function(const C& obj) {
        return ::function<C>(obj);
    }
}



class Tracer
{
public:
    static Tracer* GetInstance() {
        if (_Instance == nullptr) {
            _Instance = new Tracer();
        }

        return _Instance;
    }

    void Hook();
    void Start();
    void Stop();

    bool IsTracing() {
        return _isTracing;
    }

    void Toggle() {
        if (!_isTracing) Start();
        else Stop();
    }

    void OnBeginTick();
    void OnEndTick();
    void OnEvent(std::string evt);

    void OnEnter(std::string functionName);
    void OnExit(std::string functionName, std::string suffix, long durationNano);

    void *GetPointer(UE4::UFunction* function);

    std::string ToString(SDK::UProperty* prop, void* result);
    std::string GetParams(SDK::UFunction *fn, uint32 codeOffset, UE4::UObject* Context, UE4::FFrame& Stack, void* result);
    SDK::UFunction* GetFunction(SDK::UObject*, std::string name);
private:
    ~Tracer() {
        if (_file.is_open()) {
            _file.close();
        }
    }

    static Tracer *_Instance;

    std::ofstream _file;
    bool _isTracing = false;
    int _indentation = 0;
    std::map<std::string, void *> _functionTable;
    std::stack<uint64_t> _instructionStack;

    uint64_t _instructionCounter = 0;
    static FNativeFuncPtr* _GNatives;

    std::map<std::string, SDK::UFunction*> _fnTable;

    std::chrono::time_point<std::chrono::steady_clock> _tickStartTime;
};

