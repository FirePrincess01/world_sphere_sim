//
// @file:   dll_function.h
// @author: FirePrincess
// @date:   2021-09-10
// @brief:  A function from a dll
//

#pragma once

#include <string>
#include <memory>

class DLLFunctionBase 
{
public:
    using Function = int (*)();

    DLLFunctionBase();
    ~DLLFunctionBase();

    Function load(std::string const & filename, std::string const & functionName);
    void reset();
private:
    struct Impl;
    std::unique_ptr<Impl> mImpl;
};


template<typename T>
class DllFunction;

template<typename TRet, typename... TArgs>
class DllFunction<TRet(TArgs...)>
{
public:
    // using Function = void (*)(int &);
     using Function = TRet (*)(TArgs...);

    bool load(std::string const filename, std::string const functionName);
    bool reload();
    void reset();

    bool loaded();

    TRet operator()(TArgs ...args) const;

private:
    Function mFunction = nullptr;

    std::string mFilename;
    std::string mFunctionName;

    DLLFunctionBase mBase;
};

///////////////////////////////////////////////////////////////////////////////
// Implementation

template<typename TRet, typename... TArgs>
inline bool DllFunction<TRet(TArgs...)>::load(std::string const filename, std::string const functionName)
{
    reset();

    mFunction = reinterpret_cast<Function>(mBase.load(filename, functionName));
    if(mFunction == nullptr) {
        return false;
    }

    mFilename = filename;
    mFunctionName = functionName;
    return true;
}

template<typename TRet, typename... TArgs>
inline bool DllFunction<TRet(TArgs...)>::reload()
{   
    auto const filename = mFilename;
    auto const functionName = mFunctionName;
    return load(filename, functionName);
}

template<typename TRet, typename... TArgs>
inline void DllFunction<TRet(TArgs...)>::reset()
{
    mFilename = "";
    mFunctionName = "";
    mFunction = nullptr;
    mBase.reset();
}

template<typename TRet, typename... TArgs>
bool DllFunction<TRet(TArgs...)>::loaded()
{
    return mFunction != nullptr;
}

template<typename TRet, typename... TArgs>
inline TRet DllFunction<TRet(TArgs...)>::operator()(TArgs ...args) const {
    if(mFunction != nullptr) {
        mFunction(args...);
    }
}