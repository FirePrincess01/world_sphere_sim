//
// @file:   dll_function_hotswap.h
// @author: FirePrincess
// @date:   2021-09-10
// @brief:  A function from a dll
//

#pragma once

#include "dll_function.h"
#include <filesystem>
#include <fstream>


template<typename T>
class DllFunctionHotswap;

//
// @class:  DllFunctionHotswap
// @author: FirePrincess
// @date:   2021-09-10
// @brief:  Loads a dll and keeps a reference to a 
//
template<typename TRet, typename... TArgs>
class DllFunctionHotswap<TRet(TArgs...)>
{
public:
    using Function = TRet (*)(TArgs...);

    DllFunctionHotswap(std::string const filename, std::string const functionName);

    TRet operator()(TArgs ...args) const;

    void update();

private:
    std::string mFilename;
    std::string mFunctionName;

    DllFunction<TRet(TArgs...)> mFunc;

    std::filesystem::file_time_type mLastWrite = {};
    std::uintmax_t mFileSize = {};
    int changed = 0;

};

///////////////////////////////////////////////////////////////////////////////
// Implementation

template<typename TRet, typename... TArgs>
inline DllFunctionHotswap<TRet(TArgs...)>::DllFunctionHotswap(std::string const filename, 
    std::string const functionName)
    : mFilename(filename),
      mFunctionName(functionName)
{

}

template<typename TRet, typename... TArgs>
TRet DllFunctionHotswap<TRet(TArgs...)>::operator()(TArgs ...args) const
{
    mFunc(args...);
}

void copyFile(std::string const & src, std::string const & dst)
{
  std::ifstream ist;
  std::ofstream ost;

  ist.open(src, std::ios::in | std::ios::binary);
  ost.open(dst, std::ios::out | std::ios::binary);
  ost << ist.rdbuf();

  ist.close();
  ost.close();
}

template<typename TRet, typename... TArgs>
void DllFunctionHotswap<TRet(TArgs...)>::update()
{   
    try 
    {
       if(std::filesystem::last_write_time(mFilename) != mLastWrite 
        && std::filesystem::file_size(mFilename) != 0) 
        {
            mLastWrite = std::filesystem::last_write_time(mFilename);

            mFunc.reset();
            // copyFile(mFilename, mFilename + "_copy");

            std::filesystem::remove(mFilename + "_copy");
            std::filesystem::copy_file(mFilename, mFilename + "_copy");

            std::cout << "orig:" << std::filesystem::file_size(mFilename) << std::endl;
            std::cout << "copy:" << std::filesystem::file_size(mFilename + "_copy") << std::endl;

            mFunc.load(mFilename + "_copy", mFunctionName);
        }
    }
    catch(std::exception const & e) 
    {
        std::cout << e.what() << std::endl;
    }
}