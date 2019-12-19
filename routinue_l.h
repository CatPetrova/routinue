#ifndef OCTGUI_DEBUG_ROUTINUE_H_
#define OCTGUI_DEBUG_ROUTINUE_H_

#include <string>

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

  void duration();
  void log();

  CompFloatingRes CompFloating(const double &num1, const double &num2);
}

#endif
