//
// @file:   dll_function.cpp
// @author: FirePrincess
// @date:   2021-09-10
// @brief:  A function from a dll
//

#include "dll_function.h"
#include <windows.h>

struct DLLFunctionBase::Impl {
    HINSTANCE hProcIDDLL = nullptr;
};


DLLFunctionBase::DLLFunctionBase()
    : mImpl(std::make_unique<Impl>())
{

}

DLLFunctionBase::~DLLFunctionBase()
{
    reset();
}

void DLLFunctionBase::reset()
{
    // mFilename = "";
    // mFunctionName = "";
    // mFunction = nullptr;
    if(mImpl->hProcIDDLL != nullptr){
        FreeLibrary(mImpl->hProcIDDLL);
    }
}

DLLFunctionBase::Function DLLFunctionBase::load(std::string const & filename, std::string const & functionName)
{
    reset();

    mImpl->hProcIDDLL = LoadLibrary(filename.c_str());
    if(mImpl->hProcIDDLL == nullptr) {
        return nullptr;
    }

    return reinterpret_cast<Function>(GetProcAddress(mImpl->hProcIDDLL, functionName.c_str()));
}


