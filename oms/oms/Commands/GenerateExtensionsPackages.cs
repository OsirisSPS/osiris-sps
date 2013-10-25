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
using System.Net;
using System.Text;

namespace OMS.Commands
{
    public class GenerateExtensionsPackages : ICommand
    {
       

        private void CopyToPreinstalled(string id)
        {
            Engine.Form.AddLog(Engine.LogType.ltInfo, "Copy preinstalled extension " + id);

            String PathFrom = Engine.PathOutput + "/extensions/" + id + ".osiris";
            String PathTo = Engine.PathShare + "/extensions/" + id + ".osiris";

            File.Copy(PathFrom, PathTo, true);


        }

        public override void Run()
        {
            base.Run();
                        
            Engine.Form.AddLog("Generate extensions packages");

            DirectoryInfo di = new DirectoryInfo(Engine.PathData + "/extensions");

            foreach (DirectoryInfo di2 in di.GetDirectories())
            {
                String id = di2.Name;

                bool build = true;


                if (id.Length != 40)
                    build = false;

                if (id == "DB535FE72E91BD84ADA5D46745F2121E1D058402") // Language Pack - English
                    build = false;

                if (id == "9F4EBA0F1F02D218D684D92A39EF8F52E1DA9704") // Language Pack - Italian
                    build = false;

                if (id == "F86BDAADDDAE54753F3DC7608925780632CE4BF1") // Language Pack - Invariant
                    build = false;

                if (File.Exists(di2.FullName + "/not_deploy.txt") == true)
                    build = false;

                if (File.Exists(di2.FullName + "/manifest.xml") == false)
                    build = false;

                if(build)
                {
                    Engine.Form.AddLog(Engine.LogType.ltInfo, "Build " + id + ".osiris");

                    string PathTemporary = Engine.PathTemporary + "/Extension";
                    Helper.FileSystem.DirectoryCreate(PathTemporary);

                    Helper.FileSystem.DirectoryCopy(di2.FullName, PathTemporary, Engine.BlacklistBase);
                    
                    String PathOut = Engine.PathOutput + "/extensions/" + id + ".osiris";

                    Helper.FileSystem.Compress(PathOut, PathTemporary, true);

                    Helper.FileSystem.DirectoryRemove(PathTemporary);

                    Engine.Form.UploadToServer("Blue", PathOut, "/isis_data/addons_new", "");
                }
                else
                {
                    Engine.Form.AddLog(Engine.LogType.ltInfo, "Skip " + id + ".osiris");
                }                
            }
            

            Engine.Form.UploadToServer("Blue", Engine.PathOmsData + "/template.jobneedrun", "/isis_data", "addons_check.jobneedrun");            

            CopyToPreinstalled("148B613D055759C619D5F4EFD9FDB978387E97CB"); // Web Interface
            CopyToPreinstalled("AF4E66017B2F4E240E47EA9240F6F7E8C9EF7D78"); // Advertising
            CopyToPreinstalled("9A53510BB471C48AF9B7954466D123438C387368"); // Html Snippet
            CopyToPreinstalled("FBB11DCD3134703D1A2179451C5898B5E3A1DCBB"); // Custom CSS
           
            Engine.Form.AddLog(Engine.LogType.ltInfo, "Done.");
        }
    }
}
