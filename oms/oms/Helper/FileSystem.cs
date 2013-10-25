// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace OMS.Helper
{
    public class FileSystem
    {
        static public String  Shell(string FileName, string Arguments, bool WaitEnd, bool Log)
        {
            return Shell(FileName, Arguments, "", WaitEnd, false, Log);
        }

        static public String ShellCmd(string Command, bool Log)
        {
            if(Platform.IsUnix())
                return Shell("sh", String.Format("-c '{0}'", Command), true, Log);
            else
                return Shell("cmd.exe", String.Format("/c {0}", Command), true, Log);
        }

        static public String Shell(string FileName, string Arguments, string WorkingDirectory, bool WaitEnd, bool ShowWindow, bool Log)
        {
            if(Log)
                Engine.Form.AddLog(Engine.LogType.ltVerbose,"Shell Command: " + FileName + " " + Arguments);

            try
            {
                // #start a new process
                Process p = new Process();

                //#my command arguments, i.e. what site to ping
                p.StartInfo.Arguments = Arguments;

                if (WorkingDirectory != "")
                    p.StartInfo.WorkingDirectory = WorkingDirectory;

                //#the command to invoke under MSDOS
                p.StartInfo.FileName = FileName;

                if (ShowWindow == false)
                {
                    //#do not show DOS window
                    p.StartInfo.CreateNoWindow = true;
                    p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                }

                if (WaitEnd)
                {
                    p.StartInfo.UseShellExecute = false;
                    p.StartInfo.RedirectStandardOutput = true;
                }

                p.Start();

                if (WaitEnd)
                {
                    string Output = p.StandardOutput.ReadToEnd();
                    p.WaitForExit();
                    if (Log)
                        Engine.Form.AddLog(Engine.LogType.ltVerbose, "Shell Output: " + Output);
                    return Output;
                }
                else
                {
                    return "";
                }
            }
            catch (Exception E)
            {
                if (Log)
                    Engine.Form.AddLog(Engine.LogType.ltVerbose, "Shell Error: " + E.Message);
                return E.Message;
            }

            
        }

        static public String ReadTextFile(String Path)
        {
            StreamReader S = new StreamReader(Path);
            String T = S.ReadToEnd();
            S.Close();
            return T;
        }

        static public void WriteTextFile(String Path, String Text)
        {
            StreamWriter S = new StreamWriter(Path);
            S.Write(Text);
            S.Close();            
        }

        static public String NormalizePath(String Path)
        {
            // Normally Windows accept both convention (slash and backslash), so 'ToUnix' is the default.
            // But still exists software that don't accept slash, for example NSIS, so NormalizePathToWin exists.
            return NormalizePathToUnix(Path);
        }

        static public String NormalizePathToWin(String Path)
        {            
            return Path.Replace("/", "\\");
        }

        static public String NormalizePathToUnix(String Path)
        {
            return Path.Replace("\\", "/");
        }

        static public bool NeedCopy(String Path, List<String> Blacklist)
        {
            if (Blacklist == null)
                return true;

            String Path1 = NormalizePath(Path.ToLower());
                        
            String Path2 = Path1;    
            int PosSlash = Path2.LastIndexOf("/");
            if (PosSlash != -1)
                Path2 = Path2.Substring(PosSlash + 1);

            foreach (string Rule in Blacklist)
            {
                String Rule2 = Rule.ToLower();
                Regex obj = new Regex(Rule2);
                if (obj.IsMatch(Path2))
                    return false;
                if (obj.IsMatch(Path1))
                    return false;
            }
            return true;
        }
                
        static public void FileRemove(String Path)
        {
            if (File.Exists(Path))
                File.Delete(Path);
        }

        static public void FileCopy(String Source, String Destination)
        {
            if (File.Exists(Source) == false)
            {
                Engine.AddLog(Engine.LogType.ltWarning, "Copy file from '" + Source + "', missing.");
                return;
            }

            Engine.AddLog(Engine.LogType.ltStatusBar, "Copy from '" + Source + "' to '" + Destination + "'");
            FileRemove(Destination);

            if (Platform.IsUnix())
            {
                // File.Copy di .NET deferenzia i symlink. Non è fixabile, dicono di usare System.Unix, ma non sto a tirar dentro Mono qui per questo.
                // http://lists.ximian.com/pipermail/mono-devel-list/2007-March/022806.html
                String Flags = "-P";
                if (Platform.IsOSX())
                    Flags += " -R";
                ShellCmd(String.Format("cp {0} '{1}' '{2}'", Flags, NormalizePath(Source), NormalizePath(Destination)),false);
            }
            else
            {
                File.Copy(Source, Destination);
            }
        }

        static public void DirectoryRemove(String Path)
        {
            if (Directory.Exists(Path) == true)
                Directory.Delete(Path, true);
        }

        static public long DirectorySize(DirectoryInfo directory, bool includeSubdirectories)
        {
            long totalSize = 0;

            // Examine all contained files.
            FileInfo[] files = directory.GetFiles();
            foreach (FileInfo file in files)
            {
                totalSize += file.Length;
            }

            // Examine all contained directories.
            if (includeSubdirectories)
            {
                DirectoryInfo[] dirs = directory.GetDirectories();
                foreach (DirectoryInfo dir in dirs)
                {
                    totalSize += DirectorySize(dir, true);
                }
            }

            return totalSize;
        }

        static public void DirectoryCreate(String Path)
        {
            if (Directory.Exists(Path) == false)
            {
                Engine.AddLog(Engine.LogType.ltVerbose, "Creating directory '" + Path + "'");
                Directory.CreateDirectory(Path);
            }
            else
            {
                Engine.AddLog(Engine.LogType.ltVerbose, "Not creating directory '" + Path + "', already exists.");
            }
        }

        static public void DirectoryCopy(String Source, String Destination)
        {
            DirectoryCopy(Source, Destination, null);
        }

        static public void DirectoryCopy(String Source, String Destination, List<string> Blacklist)
        {
            if (Directory.Exists(Source) == false)
            {
                Engine.AddLog(Engine.LogType.ltWarning, "Copy directory from '" + Source + "', missing.");
                return;
            }

            DirectoryInfo di = new DirectoryInfo(Source);

            Directory.CreateDirectory(Destination);

            foreach (FileInfo fi in di.GetFiles())
            {
                if ((fi.Attributes & FileAttributes.Hidden) != FileAttributes.Hidden)
                {
                    //if (!XCopyExclude(fi.Name))
                    {
                        string destPath = Destination + "/" + fi.Name;

                        bool needCopy = NeedCopy(destPath,Blacklist);
                        /*
                        if ((fi.Extension.ToUpper() == ".DLL") && (File.Exists(destPath)))
                            needCopy = false;
                        if ((fi.Extension.ToUpper() == ".EXE") && (File.Exists(destPath)))
                            needCopy = false;
                        */
                        if (needCopy)
                        {
                            if (File.Exists(destPath))
                                File.Delete(destPath);
                            FileCopy(fi.FullName, destPath);                                                        
                        }
                    }
                }
            }

            foreach (DirectoryInfo sdi in di.GetDirectories())
            {
                if(NeedCopy(sdi.FullName, Blacklist))
                {
                    DirectoryCopy(sdi.FullName, Destination + "/" + sdi.Name, Blacklist);
                }
            }
        }

        static public void Compress(String Destination, String Source, bool ForceZip)
        {
            Helper.FileSystem.FileRemove(Destination);            

            if(ForceZip)
            {
                /*
                // -rr3p  = recovery records = 3p
                // -md4096 = select dictionary size = 4096
                // -m5 = set compression method = best
                // -afzip = zip format
                // -s = solid
                // -ep1 = exclude base path
                // -apOsiris = append 'Osiris' to path
                String WinRarSwitch = " -rr3p -ep1 -s -md4096 -m5 -r -afzip";
                Shell("C:\\Program Files\\WinRAR\\winrar.exe", String.Format("a {0} \"{1}\" \"{2}/*.*\"", WinRarSwitch, Destination, I.Parent.FullName), "", true, true);
                */

                String SevenZipSwitch = "-tzip -mx9";

                SevenZipSwitch += " -r -x!_work -x!*.svn -x!xx*.*";
           
                //String Command = String.Format("a {0} \"{1}\" \"{2}/*\"", SevenZipSwitch, Destination, NormalizePath(I.Parent.FullName));
                String Command = String.Format("a {0} \"{1}\" \"{2}/*\"", SevenZipSwitch, Destination, Source);

                if (Platform.IsWindows())
                    Command = Environment.CurrentDirectory + "/../../utils/7za.exe " + Command;
                else
                    Command = "7za " + Command;
                ShellCmd(Command,true);                
            }
            else
            {
                // Questo giro, per far sì che la directory di root del tar sia la directory stessa...
                // ex. tar -czf /media/Data/osiris/oms/repository/test.tar.gz -C '/media/Data/osiris/distribute/output/osiris-sps_0.13.0_linux-i386-noinstall' osiris-sps_0.13.0

                // Questo giro, incredibile ma non son riuscito a passar i parametri giusti al "tar". Peccato che 7zip non supporta il tar.gz.
                String Files = "";

                DirectoryInfo DI = new DirectoryInfo(Source);
                foreach(FileInfo FI in DI.GetFiles())
                    Files += " '" + FI.Name + "'";
                foreach(DirectoryInfo DI2 in DI.GetDirectories())
                    Files += " '" + DI2.Name + "'";
                
                //ShellCmd(String.Format("tar -czf '{0}' -C '{1}' '{2}'", Destination, NormalizePath(I.Parent.FullName), I.Name));                
                ShellCmd(String.Format("tar -czf '{0}' -C '{1}' {2}", Destination, Source, Files),true);                
            }            
        }
    }
}
