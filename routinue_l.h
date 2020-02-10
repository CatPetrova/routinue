#ifndef OCTGUI_DEBUG_ROUTINUE_H_
#define OCTGUI_DEBUG_ROUTINUE_H_

#include <string>
#include <queue>
#include <chrono>
#include <cstdint>
#include <list>

namespace routinue_l{
enum class CompFloatingRes {
    kEqual = 0
  , kLess = 1
  , kGreater = 2
};

bool GetExecuteFilePath(std::basic_string<TCHAR> *exe_path);
bool DirectoryExists(const std::basic_string<TCHAR> &dir_path);
bool MakeDirectory(const std::basic_string<TCHAR> &dir_path);

std::basic_string<TCHAR> GetLocalTime();
std::basic_string<TCHAR> GetThreadStr();

void duration();
void log();

CompFloatingRes CompFloating(const double &num1, const double &num2);

//返回值是一个wchat_t数组，需要手动释放:delete []
wchar_t *MultiByteToWideChar(const std::string &mbcs);
void MultiByteToWideChar(const std::string &mbcs, std::wstring *utf_16);

//返回值是一个wchat_t数组，需要手动释放:delete []
char *WideCharToMultiByte(const std::wstring &utf_16);
void WideCharToMultiByte(const std::wstring &utf_16, std::string *mbcs);

class Time {
public:
  static void init();
  static int64_t GetCostTime();
private:
  static std::queue<std::chrono::high_resolution_clock::time_point> time_que_;
};
}

#endif