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
    public class DeployExtensionsAnubis : ICommand
    {
        private void FetchToPreinstalled(string id)
        {
            Engine.Form.AddLog(Engine.LogType.ltInfo, "Download from Anubis, extension " + id);

            String UrlFrom = "http://www.osiris-sps.org/anubis/addons/" + id + ".osiris";
            String PathTo = Engine.PathShare + "/extensions/" + id + ".osiris";

            WebClient Client = new WebClient();
            Client.DownloadFile(UrlFrom, " " + PathTo);
        }

        private void FetchAndUpload(string id)
        {
            Engine.Form.AddLog(Engine.LogType.ltInfo, "Download from Anubis, extension " + id);

            String UrlFrom = "http://www.osiris-sps.org/anubis/addons/" + id + ".osiris";
            String PathTo = Engine.PathOutput + "/extensions/" + id + ".osiris";

            WebClient Client = new WebClient();
            Client.DownloadFile(UrlFrom, " " + PathTo);

            Engine.Form.UploadToServer("Blue", PathTo, "/isis_data/addons_new", "");
        }


        public override void Run()
        {
            base.Run();

            Engine.Form.AddLog("Deploy Extensions Anubis");

            // Development code of these extensions are not under SVN. 
            // Also isn't under SVN because Osiris Install System will be delete .svn directories on extensions update, the page /main/addons.
            // Anubis generate it and deploy to Isis. During development, we update the extensions from Osiris itself.
            FetchToPreinstalled("DB535FE72E91BD84ADA5D46745F2121E1D058402"); // Language Pack - English
            FetchToPreinstalled("9F4EBA0F1F02D218D684D92A39EF8F52E1DA9704"); // Language Pack - Italian
            FetchToPreinstalled("F86BDAADDDAE54753F3DC7608925780632CE4BF1"); // Language Pack - Invariant
                        
            FetchAndUpload("A15635A70058CAEBD21D7473CE8310D410CFD547"); // Osiris Language Pack - Russian
            FetchAndUpload("9F4EBA0F1F02D218D684D92A39EF8F52E1DA9704"); // Osiris Language Pack - Italian
            FetchAndUpload("9BD6234B4AE6FB62C132353B6B181D7515E34D46"); // Osiris Language Pack - German
            FetchAndUpload("13ABA7A88BC7FD3DC9DBACD6E4817EDC13319E94"); // Osiris Language Pack - Spanish
            FetchAndUpload("C985304A3A7C6EA5B5E8BF8E6EE6D12B9D7C5312"); // Osiris Language Pack - Portuguese
            FetchAndUpload("F86BDAADDDAE54753F3DC7608925780632CE4BF1"); // Osiris Language Pack - Invariant
            FetchAndUpload("DB535FE72E91BD84ADA5D46745F2121E1D058402"); // Osiris Language Pack - English
            FetchAndUpload("C5606EDAD40A4029A198C8D39760DDA94E22C584"); // Osiris Language Pack - Netherland
            FetchAndUpload("C6222C74FF3B26904BC6AFB519E6E457CC77E1A8"); // Osiris Language Pack - Japanese
            FetchAndUpload("76FB856842D3E085A7A82B55350D8F409D15B023"); // Osiris Language Pack - French
            FetchAndUpload("6E43DDFBBD44D8930177B7084EDD970992A9DBBE"); // Osiris Language Pack - Greek


            //CopyToPreinstalled("DB535FE72E91BD84ADA5D46745F2121E1D058402"); // Language Pack - English
            //CopyToPreinstalled("9F4EBA0F1F02D218D684D92A39EF8F52E1DA9704"); // Language Pack - Italian
            //CopyToPreinstalled("F86BDAADDDAE54753F3DC7608925780632CE4BF1"); // Language Pack - Invariant

            Engine.Form.UploadToServer("Blue", Engine.PathOmsData + "/template.jobneedrun", "/isis_data", "addons_check.jobneedrun");            

            Engine.Form.AddLog(Engine.LogType.ltInfo, "Done.");
        }
    }
}
