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
using System.IO;
using System.Text;

namespace OMS.Helper
{
    public class Nsis
    {
        public static bool ExcludeForNSIS(string Name)
        {
            //if (Name.ToUpper() == "DATA") return true;
            //if (Name.ToUpper() == "SKINS") return true;
            //if (Name.ToUpper() == "PLUGINS") return true;
            //if (Name.ToUpper() == "SYMBOLS") return true;
            //if ( (!chkSymbols.Checked) && (Name.ToUpper() == "SYMBOLS") ) return true;

            return false;
        }


        public static void ReplaceNsisScript(ref string NsisScript, string Name, string AbsolutePath, string RelativePath, List<String> NotToOverwrite)
        {
            String InstText = "";
            String UninstText = "";

            GenerateNsisScript(Helper.FileSystem.NormalizePathToWin(AbsolutePath), Helper.FileSystem.NormalizePathToWin(RelativePath), ref InstText, ref UninstText, NotToOverwrite);

            NsisScript = NsisScript.Replace(String.Format("#OS_Inst_{0}_Files#", Name), InstText);
            NsisScript = NsisScript.Replace(String.Format("#OS_Uninst_{0}_Files#", Name), UninstText);
        }

        public static void GenerateNsisScript(String AbsolutePath, String RelativePath, ref String InstText, ref String UninstText, List<String> NotToOverwrite)
        {
            DirectoryInfo di = new DirectoryInfo(AbsolutePath);
            if (ExcludeForNSIS(di.Name)) return;

            InstText = InstText + String.Format("\t\tSetOutPath \"$INSTDIR{0}\"\n", RelativePath);

            foreach (FileInfo fi in di.GetFiles())
            {
                if ((fi.Attributes & FileAttributes.Hidden) != FileAttributes.Hidden)
                {
                    if (!ExcludeForNSIS(fi.Name))
                    {
                        bool skipOverwrite = NotToOverwrite.Contains(fi.Name);
                        if (skipOverwrite)
                            InstText = InstText + "\t\tSetOverwrite off\n";
                        InstText = InstText + string.Format("\t\tFile \"{0}\"\n", fi.FullName);
                        if (skipOverwrite)
                            InstText = InstText + "\t\tSetOverwrite on\n";
                        UninstText = UninstText + string.Format("\tDelete \"$INSTDIR{0}\\{1}\"\n", RelativePath, fi.Name);
                    }
                }
            }

            foreach (DirectoryInfo sdi in di.GetDirectories())
            {
                if ((sdi.Attributes & FileAttributes.Hidden) != FileAttributes.Hidden)
                {
                    GenerateNsisScript(sdi.FullName, RelativePath + "\\" + sdi.Name, ref InstText, ref UninstText, NotToOverwrite);
                }
            }

            UninstText = UninstText + String.Format("\tRMDIR \"$INSTDIR{0}\"\n", RelativePath);
        }

    }
}
