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
using System.Text;

namespace OMS.Helper
{
    static class Platform
    {
        static public String UName;
        static public String Architecture;

        static public void Init()
        {
            if (IsUnix())
            {
                UName = Helper.FileSystem.ShellCmd("uname",false).Trim();
                Architecture = Helper.FileSystem.ShellCmd("uname -m",false);
            }
            else
            {
                UName = "Windows";
                Architecture = "Unknown.";
            }
        }

        static public bool IsUnix()
        {
            return (Environment.OSVersion.Platform.ToString() == "Unix");
        }

        static public bool IsWindows()
        {
            return UName == "Windows";
        }

        static public bool IsLinux()
        {
            return UName == "Linux";
        }

        static public bool IsOSX()
        {
            return UName == "Darwin";
        }

        static public bool IsAdmin()
        {
            if (IsUnix())
                return (Environment.UserName == "root");
            else
            {
                // TODO
                return true;
            }
        }
    }
}
