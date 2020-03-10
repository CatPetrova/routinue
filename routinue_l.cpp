#include "stdafx.h"
#include "routinue_l.h"
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <queue>
#include <cassert>
#include <algorithm>
#include <Windows.h>

#pragma warning(disable: 4800)

namespace routinue_l{

bool GetExecuteFilePath(std::basic_string<TCHAR> *exe_path){
  constexpr int kMaxPath = 260;
  unsigned long ret_code = 0;
  TCHAR path[kMaxPath] = _T("\0");
  bool ret = false;

  ret_code = GetModuleFileName(nullptr, path, sizeof(path) / sizeof(TCHAR));
  if ((ret_code != 0) && (GetLastError() == ERROR_SUCCESS)) {
    exe_path->assign(path);
    exe_path->erase(exe_path->rfind(_T('\\')));

    ret = true;
  }
  else {
    ret = false;
  }

  return ret;
}

std::basic_string<TCHAR> GetLocalTime() {
  std::time_t time = std::time(nullptr);
  std::tm tm; 
  std::basic_stringstream<TCHAR> time_stream;

  localtime_s(&tm, &time);
  time_stream << std::put_time(&tm, _T("%Y_%m_%d_%H_%M_%S"));

  return time_stream.str();
}

void duration(){
  std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
  std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
  std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
  // Note: ms.count() return integer.

}

// #include <string>
// #include <sstream>
// #inlucde <fstream>
void log() {
  std::basic_string<TCHAR> log_file_name;
  std::basic_string<TCHAR> exe_path;
  std::basic_ofstream<TCHAR> log_file;

  log_file_name.assign(_T("D:\\path\\a.txt"));
  log_file.open(log_file_name, std::ofstream::out);
  log_file << _T("run at:") << __LINE__ << std::endl << std::flush;
  log_file.close();

}

bool DirectoryExists(const std::basic_string<TCHAR> &dir_path) {
  bool ret = false;    
  std::basic_string<TCHAR> path(dir_path);
  int length = static_cast<int>(path.size());
  WIN32_FIND_DATA wfd;
  HANDLE hFind;

  assert(length > 0);
  assert(path.length() >= 3);

  if (path.length() == 3){//磁盘根目录
    if (GetDriveType(path.c_str()) == DRIVE_NO_ROOT_DIR) return false;
    else return true;
  }

  if (path.at(length - 1) == _T('\\'))
    path.replace(length - 1, 1, 1, _T('\0')); // 删除末尾的"\"

  hFind = FindFirstFile(path.c_str(), &wfd);  // 查找该文件夹
  if (hFind == INVALID_HANDLE_VALUE){
    ret = false;  // 没有找到配备，目录肯定不存在
  }
  else{
    if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  // 检查找到的结果是否目录
      ret = true; // 是目录,目录存在
    else
      ret = false; // 不是目录,目录不存在

    FindClose(hFind);
  }

  return ret;
}
bool MakeDirectory(const std::basic_string<TCHAR>& dir_path) {
  std::basic_string<TCHAR> path(dir_path);
  std::basic_string<TCHAR> parent_path(_T("\0"));
  int length = static_cast<int>(path.size());
  bool ret = false;

  if (path.empty())
    return false;

  if (path.length() <= 3) //磁盘根目录 无法创建
    return false;

  if (DirectoryExists(path))
    return true;

  if (path.at(length - 1) == _T('\\'))
    path.replace(length - 1, 1, 1, _T('\0')); // 删除末尾的"\"

  int backslash_pos = static_cast<int>(path.rfind(_T('\\')));
  if (backslash_pos != std::basic_string<TCHAR>::npos) {
    parent_path.assign(path.substr(0, backslash_pos + 1));

    if (parent_path.empty())
      return false;
    if (parent_path.size() > 3) { // 非根目录
      if (!DirectoryExists(parent_path)) {
        if (!MakeDirectory(parent_path)) {
          return false;
        }
      }
    }
  }

  SECURITY_ATTRIBUTES SecurityAttributes;
  SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
  SecurityAttributes.lpSecurityDescriptor = NULL;
  SecurityAttributes.bInheritHandle = 0;

  ret = static_cast<bool>(CreateDirectory(path.c_str(), &SecurityAttributes));

  return ret;
}

CompFloatingRes CompFloating(const double &num1, const double &num2) {
  constexpr double kPosInfinitesimal = 0.000000000001;
  constexpr double kNegInfinitesimal = -0.000000000001;
  double diff = num1 - num2;

  if ((diff < kNegInfinitesimal))
    return CompFloatingRes::kLess;
  if (diff > kPosInfinitesimal) 
    return CompFloatingRes::kGreater;
  return CompFloatingRes::kEqual;
}

wchar_t *MultiByteToWideChar(const std::string &mbcs) {
  wchar_t *wide_char = nullptr;
  int wide_char_size = 0;

  wide_char_size = ::MultiByteToWideChar(CP_ACP,
                                        0,
                                        mbcs.c_str(),
                                        static_cast<int>(mbcs.size()),
                                        wide_char,
                                        0) + 1;
  wide_char = new wchar_t[wide_char_size]();
  std::fill(wide_char, wide_char + wide_char_size, L'\0');
  ::MultiByteToWideChar(CP_ACP,
                        0,
                        mbcs.c_str(),
                        static_cast<int>(mbcs.size()),
                        wide_char,
                        wide_char_size);
  
  return wide_char;
}

void MultiByteToWideChar(const std::string &mbcs, std::wstring *utf_16) {
  wchar_t *wide_char = nullptr;

  wide_char = MultiByteToWideChar(mbcs);
  utf_16->assign(wide_char);
  delete []wide_char;
  wide_char = nullptr;
}

char *WideCharToMultiByte(const std::wstring &utf_16) {
  char *mbcs = nullptr;
  int mbcs_size = 0;

  mbcs_size = ::WideCharToMultiByte(CP_ACP,
                                    0,
                                    utf_16.c_str(),
                                    static_cast<int>(utf_16.size()),
                                    mbcs,
                                    0,
                                    NULL,
                                    FALSE) + 1;
  mbcs = new char[mbcs_size]();
  std::fill(mbcs, mbcs + mbcs_size, '\0');
  memset(mbcs, '\0', mbcs_size * sizeof(char));
  ::WideCharToMultiByte(CP_ACP,
                        0,
                        utf_16.c_str(),
                        static_cast<int>(utf_16.size()),
                        mbcs,
                        mbcs_size,
                        NULL,
                        FALSE);
  return mbcs;
}

void WideCharToMultiByte(const std::wstring &utf_16, std::string *mbcs) {
  char *mbcs_string = nullptr;

  mbcs_string = WideCharToMultiByte(utf_16);
  mbcs->assign(mbcs_string);
  delete []mbcs_string;
  mbcs_string = nullptr;
}

std::queue<std::chrono::high_resolution_clock::time_point> Time::time_que_;
int64_t Time::GetCostTime() {
  int64_t cost_time = 0;
  std::chrono::high_resolution_clock::time_point tm_pt = std::chrono::high_resolution_clock::now();
  if (time_que_.size() == 1) {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(tm_pt - time_que_.front());
    cost_time = ms.count();
    time_que_.pop();
  }
  time_que_.push(tm_pt);
  return cost_time;
}

void Time::init() {
  while(!time_que_.empty())
    time_que_.pop();
  time_que_.push(std::chrono::high_resolution_clock::now());
}

std::basic_string<TCHAR> GetThreadStr() {
  std::basic_stringstream<TCHAR> id_stream;

  std::thread::id id = std::this_thread::get_id();
  id_stream << id;
  return id_stream.str();
}
}