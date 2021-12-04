#pragma once
#include "iostream"
#include "Mod/Mod.h"
#include <fstream>
#include "MultiplayerMod.h"
#include <stack>

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
    void OnExit(std::string functionName, long durationNano);

    void *GetPointer(UE4::UFunction* function);

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


    std::chrono::time_point<std::chrono::steady_clock> _tickStartTime;
};

