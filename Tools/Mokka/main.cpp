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

#include "mokkaConfigure.h"

#include "MainWindow.h"
#include "LoggerWidget.h"

#include <QApplication>

#if defined(_MSC_VER)
  #include <windows.h> // For WinMain
  #include <io.h>      // For _open_osfhandle
  #include <fcntl.h>   // For _O_TEXT
#endif

#include <pcl/pcl.h> // Must be included at the end (to detect the use of windows.h)

// Under Windows, the main function is transformed in a WinMain function ...
#if defined(_MSC_VER)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  int argc = __argc;
  char** argv = __argv;

  if (argc > 1) // Command line options
  {
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
      int hCrt;
      FILE *hf;
      // Standard output
      hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
      if (hCrt == -1)
      {
        MessageBoxW(NULL,
                    (LPCWSTR)L"Error: Impossible to open the standard output! Are you using the MSVC command prompt?  \n\nPlease use a regular command prompt when you want to add command line options",
                    (LPCWSTR)L"Mokka",
                    MB_ICONERROR | MB_OK);
        return -1;
      }
      hf = _fdopen(hCrt, "w");
      *stdout = *hf;
      setvbuf( stdout, NULL, _IONBF, 0 );
      // Error output
      hCrt = _open_osfhandle((long) GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);
      if (hCrt == -1)
      {
        MessageBoxW(NULL,
                    (LPCWSTR)L"Error: Impossible to open the error output! Are you using the MSVC command prompt?  \n\nPlease use a regular command prompt when you want to add command line options",
                    (LPCWSTR)L"Mokka",
                    MB_ICONERROR | MB_OK);
        return -1;
      }
      hf = _fdopen(hCrt, "w");
      *stderr = *hf;
      setvbuf( stderr, NULL, _IONBF, 0 );
      // Sync cout and cerr
      std::ios::sync_with_stdio();
    }
    /*
    else
    {
      unsigned int result = GetLastError();
      if (result == ERROR_ACCESS_DENIED)
      {
        MessageBoxW(NULL,
                   (LPCWSTR)L"Error: Impossible to attach the console!",
                   (LPCWSTR)L"Mokka",
                    MB_ICONERROR | MB_OK);
        return -1;
      }
      else if (result == ERROR_INVALID_HANDLE)
      {
        // Could use the AllocConsole() function.
        MessageBoxW(NULL,
                   (LPCWSTR)L"Error: Impossible to find the console!",
                   (LPCWSTR)L"Mokka",
                    MB_ICONERROR | MB_OK);
        return -1;
      }
    }
    */
  }
#else
int main(int argc, char *argv[])
{
#endif
  pcl::CmdLineParser clp("Motion Kinematic & Kinetic Analyzer", btkStringifyMacro(MOKKA_VERSION_STRING));
  clp.UseDefaultOption(pcl::Help | pcl::Version);
  pcl::FlagOpt play("play","p","Start to play the acquisition", false);
  pcl::ValueOpt<std::string> visualConfig("visual-config","c","Set the default visual configuration", "file", false);
  pcl::UnlabeledOpt<std::string> acquisition("Filename of the acquisition to load", false);
  
  clp.Add(play).Add(visualConfig).Add(acquisition);
  if (clp.Run(argc, argv) != 0)
  {
#if defined(_MSC_VER)
    std::cout << std::endl << "(Press Enter to go back to the command prompt)" << std::endl;
#endif
    return clp.GetStatus();
  }
  
  LoggerWidget::redirectCout();
  LoggerWidget::redirectCerr();
  qInstallMsgHandler(LoggerWidget::messageHandler);
  
  QApplication app(argc, argv);
  LOG_INFO(QObject::tr("Starting Mokka.")); // The QApplication must be created before this line ("QPixmap: Must construct a QApplication before a QPaintDevice").
  MainWindow mw;
  QObject::connect(&app, SIGNAL(focusChanged(QWidget*, QWidget*)), &mw, SLOT(toggleEditActions(QWidget*, QWidget*)));
  mw.show();
  
  if (acquisition.IsAssigned())
    mw.openFile(QString::fromStdString(acquisition.GetValue()));
  if(visualConfig.IsAssigned())
    mw.loadConfiguration(QString::fromStdString(visualConfig.GetValue()));
  if (play.IsAssigned()) // Should be the last option
    mw.play();
    
  QTimer::singleShot(500, &mw, SLOT(checkSoftwareUpdateStartup()));
    
  int res = app.exec();
    
#if defined(_MSC_VER)
  if (argc > 1)
    std::cout << std::endl << "(Press Enter to go back to the command prompt)" << std::endl;
#endif

  return res;
};