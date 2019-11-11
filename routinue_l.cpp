#include "stdAfx.h"
#include "routinue_l.h"
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>

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

}