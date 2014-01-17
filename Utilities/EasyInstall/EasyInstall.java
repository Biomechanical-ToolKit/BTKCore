/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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
 
import java.io.*;
import javax.swing.*;
 
class EasyInstall
{
  public static void main(String[] args)
  {
    String currDir = new File("").getAbsolutePath();
    Process p = null;
    try
    {
      if (isWindows())
      {
        String EasyInstall_MSVC_cmd = currDir + "\\Batch\\EasyInstall_MSVC.cmd";
        FileWriter fstream = new FileWriter(EasyInstall_MSVC_cmd);
        BufferedWriter out = new BufferedWriter(fstream);
        out.write("@ECHO OFF\n"
                + "cd \""+ currDir +"\\Batch\"\n"
                + currDir.substring(0,2) + "\n"
                + "call .\\EasyInstallWithRedistributableMatlabToolbox_MSVC.bat\n"
                + "exit\n");
        out.close();
        String EasyInstall_MSVC_start_cmd = currDir + "\\Batch\\EasyInstall_MSVC_start.cmd";
        fstream = new FileWriter(EasyInstall_MSVC_start_cmd);
        out = new BufferedWriter(fstream);
        out.write("start cmd /c \"" + EasyInstall_MSVC_cmd + "\"\n");
        out.close();
        p = Runtime.getRuntime().exec("cmd /c \"" + EasyInstall_MSVC_start_cmd + "\"");
        p.waitFor();
      }
      else if (isMac())
      {
        // To be able to close automaticaly the terminal under MacOS X 10.5+
        String EasyInstall_GCC_terminal = currDir + "/Batch/EasyInstall_GCC.terminal";
        FileWriter fstream = new FileWriter(EasyInstall_GCC_terminal);
        BufferedWriter out = new BufferedWriter(fstream);
        out.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
                + "<plist version=\"1.0\">\n"
                + "<dict>\n"
                + "\t<key>CommandString</key>\n"
                + "\t<string>cd " + currDir + ";" + currDir + "/Batch/EasyInstallWithRedistributableMatlabToolbox_GCC_MacOSX.sh;exit</string>\n"
                + "\t<key>RunCommandAsShell</key>\n"
                + "\t<false/>\n"
                + "\t<key>columnCount</key>\n"
                + "\t<integer>116</integer>\n"
                + "\t<key>name</key>\n"
                + "\t<string>EasyInstall_GCC</string>\n"
                + "\t<key>rowCount</key>\n"
                + "\t<integer>29</integer>\n"
                + "\t<key>shellExitAction</key>\n"
                + "\t<integer>0</integer>\n"
                + "\t<key>type</key>\n"
                + "\t<string>Window Settings</string>\n"
                + "</dict>\n"
                + "</plist>\n");
        out.close();
        p = Runtime.getRuntime().exec("open " + EasyInstall_GCC_terminal);
        p.waitFor();
      }
      else if (isUnix())
      {
        // Under linux, the command 'new File("").getAbsolutePath();' returns the home path 
        // The easy way to determine the path of the JAR file is to ask to the user
        // where it is ...
        JOptionPane.showMessageDialog(null, "Unfortunately, Java under linux doesn't give the good current directory and it is not possible to active the batch.\nTo build BTK with Matlab under Linux, double click on the file 'EasyInstallWithRedistributableMatlabToolbox_GCC_Unix.sh'\nin the subfolder Batch.");
      }
      else
        throw new Exception("Unsupported OS. Contact the developers.");
    }
    catch (Exception err)
    {
      JOptionPane.showMessageDialog(null, 
                                    "Unexpected error: " + err.getMessage(), "BTK Easy Install",
                                    JOptionPane.ERROR_MESSAGE);
    }
  };
  
  public static boolean isWindows() {return (System.getProperty("os.name").toLowerCase().indexOf( "win" ) >= 0);};
  public static boolean isMac() {return (System.getProperty("os.name").toLowerCase().indexOf( "mac" ) >= 0);};
  public static boolean isUnix()
  {
    String os = System.getProperty("os.name").toLowerCase();
    return ((os.indexOf( "nix") >=0) || (os.indexOf( "nux") >=0));
  };
};