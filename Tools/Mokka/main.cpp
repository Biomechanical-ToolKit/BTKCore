/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "MainWindow.h"

#include <QApplication>

#include <pcl/pcl.h>

#define xstr(s) str(s)
#define str(s) #s

int main(int argc, char *argv[])
{
  std::string rn = xstr(BTK_VERSION_MAJOR); rn += "."; rn += xstr(BTK_VERSION_MINOR);
  pcl::CmdLineParser clp("Motion Kinematic & Kinetic Analyzer", rn);
  clp.UseDefaultOption(pcl::Help | pcl::Version);
  pcl::FlagOpt play("play","p","Start to play the acquisition", false);
  pcl::ValueOpt<std::string> visualConfig("visual-config","c","Set the default visual configuration", "file", false);
  pcl::UnlabeledOpt<std::string> acquisition("Filename of the acquisition to load", false);
  
  clp.Add(play).Add(visualConfig).Add(acquisition);
  if (clp.Run(argc, argv) != 0)
    return clp.GetStatus();
  
  QApplication app(argc, argv);
  MainWindow mw;
  mw.show();
  
  if (acquisition.IsAssigned())
    mw.openFile(QString::fromStdString(acquisition.GetValue()));
  if(visualConfig.IsAssigned())
    mw.loadMarkerConfiguration(QString::fromStdString(visualConfig.GetValue()));
  if (play.IsAssigned()) // Should be the last option
    mw.play();
  
  return app.exec();
};
