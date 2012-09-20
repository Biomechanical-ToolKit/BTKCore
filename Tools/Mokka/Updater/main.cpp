/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <string>
#include <algorithm> // std::replace

typedef std::basic_string<TCHAR> tstring;

int moveFolder(const tstring& source, const tstring& destination, const tstring& updateExecPath);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  int argc = __argc;
  TCHAR** argv = __argv;
  
  if (argc < 5)
  {
    MessageBox(NULL, TEXT("Informations are missing\nImpossible to update the application."), TEXT("Updater error"), MB_ICONWARNING | MB_OK);
    return -1;
  }
  else if (argc > 6)
  {
    MessageBox(NULL, TEXT("Invalid informations\nImpossible to update the application."), TEXT("Updater error"), MB_ICONWARNING | MB_OK);
    return -1;
  }
  
  tstring execPath(argv[0]);
  tstring sourcePath(argv[1]);
  tstring destinationPath(argv[2]);
  DWORD applicationPid = atoi(argv[3]);
  tstring applicationName(argv[4]);
  tstring commandLine = "\"" + applicationName + "\"";
  if (argc > 5)
    commandLine += " " + tstring(argv[5]);
  
  // Be sure that the application to update is closed
  HANDLE applicationProcess = NULL;
  if ((applicationProcess = OpenProcess(SYNCHRONIZE, FALSE, applicationPid)) != NULL)
  {
    WaitForSingleObject(applicationProcess, INFINITE);
    CloseHandle(applicationProcess); 
  }
  Sleep(1000); // 1 second: Not necessary, but just to be sure that all the DLLs are unloaded
  
  std::replace(execPath.begin(), execPath.end(), '\\', '/');
  if (moveFolder(sourcePath, destinationPath, execPath) == 0)
  {
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset(&pi,0,sizeof(pi));
    memset(&si,0,sizeof(si));
    si.cb = sizeof(si);
    if(CreateProcess(applicationName.c_str(), (LPTSTR)commandLine.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi))
    {
      EnumWindows(EnumWindowsProc, (LPARAM)(pi.dwProcessId));
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }
    else
    {
      MessageBox(NULL, TEXT("Impossible to restart the updated application."), TEXT("Updater error"), MB_OK); 
      return (-1);
    }
  }
  return 0;
};

int moveFolder(const tstring& source, const tstring& destination, const tstring& updateExecPath)
{
  tstring source_ = source + "\\*";
  HANDLE hFind = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA ffd;
  hFind = FindFirstFile(source_.c_str(), &ffd);
  if (hFind == INVALID_HANDLE_VALUE) 
  {
    MessageBox(NULL, TEXT("Impossible to retrieve the files to updates."), TEXT("Updater error"), MB_ICONWARNING | MB_OK); 
    return -1;
  } 
  do
  {
    tstring fileName(ffd.cFileName);
    tstring source_ = source + "/" + fileName;
    tstring destination_ = destination + "/" + fileName;
    if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      if ((fileName.compare(".") == 0) || (fileName.compare("..") == 0))
        continue;
      else
      {
        CreateDirectory(destination_.c_str(), NULL);
        if (moveFolder(source_, destination_, updateExecPath) != 0)
        {
          FindClose(hFind);
          return -1;
        }
      }
    }
    else
    {
      // Special case for the updater
      if (destination_.compare(updateExecPath) == 0)
      {
        TCHAR value[33]; _stprintf_s(value, 32, _T("%ld"), GetCurrentProcessId());
        tstring updateExecPath_ = updateExecPath + "." + tstring(value);
        if (!MoveFileEx(updateExecPath.c_str(), updateExecPath_.c_str(), MOVEFILE_WRITE_THROUGH))
          break;
        // Remove this temporary file when rebooting the computer
        MoveFileEx(updateExecPath_.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
      }
      if (!MoveFileEx(source_.c_str(), destination_.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH)
        || !SetFileAttributes(destination_.c_str(), FILE_ATTRIBUTE_NORMAL))
        break;
    }
  }
  while (FindNextFile(hFind, &ffd) != 0);
  DWORD err = GetLastError();
  FindClose(hFind);
  if (err != ERROR_NO_MORE_FILES) 
  {
    MessageBox(NULL, TEXT("Impossible to update all the files\nYou need to update the software manually..."), TEXT("Updater error"), MB_ICONWARNING | MB_OK);
    return -1;
  }
  
  return 0;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
  DWORD dwPID;
  GetWindowThreadProcessId(hwnd, &dwPID);
  if(dwPID == lParam)
  {
    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return FALSE;
  }
  return TRUE;
};