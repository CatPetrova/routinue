#include "stdAfx.h"
#include "routinue_l.h"
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <cassert>

namespace routinue_l{

bool GetExecuteFilePath(std::basic_string<TCHAR> *exe_path){
  constexpr int kMaxPath = 260;
  unsigned long ret_code = 0;
  TCHAR path[kMaxPath] = _T("\0");
  bool ret = false;

  ret_code = GetModuleFileName(nullptr, path, sizeof(path));
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

}