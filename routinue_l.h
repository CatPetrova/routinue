#ifndef OCTGUI_DEBUG_ROUTINUE_H_
#define OCTGUI_DEBUG_ROUTINUE_H_

#include <string>

#if defined(UNICODE) || defined(_UNICODE)

#ifndef TCHAR
#define TCHAR wchar_t
#endif

#ifndef _T
#define _T(x) L ## x
#endif

#else

#ifndef TCHAR
#define TCHAR char
#endif

#ifndef _T(x)
#define _T(x) x
#endif

#endif

namespace routinue_l{
  bool GetExecuteFilePath(std::basic_string<TCHAR> *exe_path);
  std::basic_string<TCHAR> GetLocalTime();
  void duration();
  void log();
}

#endif
