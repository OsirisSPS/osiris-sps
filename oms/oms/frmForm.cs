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
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Tamir.SharpSsh;

namespace OMS
{
    public partial class frmForm : Form
    {
        public frmForm()
        {
            InitializeComponent();
        }


        private void InternalTest()
        {
            String Rule="^.*\\\\_.*$";
            String Path="_portal_0.5.xml";
            Regex obj = new Regex(Rule);
            bool X = obj.IsMatch(Path);

            String x = "";
        }

        public void AddLog(string Msg)
        {
            AddLog(Engine.LogType.ltInfo, Msg);
        }

        public void AddLog(Exception Ex)
        {
            AddLog(Engine.LogType.ltError, Ex.Message);
        }

        public void AddLog(Engine.LogType Level, string Msg)
        {
            lblLog.Text = Msg;
            lblLog.Refresh();

            Debug.WriteLine(Msg);

            if (Level != Engine.LogType.ltStatusBar)
            {
                if ((Level >= Engine.LogType.ltInfo) || (chkLogsVerbose.Checked))
                {
                    ListViewItem item = lstLogs.Items.Add(Msg);
                    //item.ImageIndex = (int) Level;

                    if (Level == Engine.LogType.ltInfo)
                        item.ImageIndex = 0;
                    else if (Level == Engine.LogType.ltWarning)
                        item.ImageIndex = 1;
                    else if (Level == Engine.LogType.ltError)
                        item.ImageIndex = 2;
                    else if (Level == Engine.LogType.ltCritical)
                    {
                        item.ImageIndex = 2;
                        LogMessage Dlg = new LogMessage();
                        Dlg.Message = Msg;
                        Dlg.ShowDialog();
                    }
                    else if (Level == Engine.LogType.ltVerbose)
                        item.ImageIndex = 0;

                    item.Selected = true;
                    lstLogs.EnsureVisible(lstLogs.Items.Count - 1);
                }
            }

            Application.DoEvents();
        }

        void AddPackage(string Name, string Platform, string Arch, string Type)
        {
            ListViewItem Item = new ListViewItem();
            Item.Text = Name;
            Item.Checked = true;
            Item.SubItems.Add(Platform);
            Item.SubItems.Add(Arch);
            Item.SubItems.Add(Type);
            lstPackages.Items.Add(Item);          
        }

        public void UploadToServer(string ServerName, string PathFrom, string PathTo, string NameTo)
        {
            if( (ServerName == "Orange") && (txtPasswordOrange.Text == "") )
            {
                Engine.AddLog(Engine.LogType.ltWarning, "Deploy of " + PathFrom + " skipped, no server password provided for " + ServerName);
                return;
            }

            if( (ServerName == "Blue") && (txtPasswordBlue.Text == "") )
            {
                Engine.AddLog(Engine.LogType.ltWarning, "Deploy of " + PathFrom + " skipped, no server password provided for " + ServerName);
                return;
            }


            try
            {
                int port = 0;
                if (ServerName == "Orange")
                {
                    Engine.Scp = new Scp(txtHostOrange.Text, txtUserOrange.Text, txtPasswordOrange.Text);
                    AddLog("Connection to Orange: " + txtHostOrange.Text + ":" + txtPortOrange.Text + " with " + txtUserOrange.Text + "/" + txtPasswordOrange.Text);
                    port = Convert.ToInt32(txtPortOrange.Text);
                    Engine.Scp.Connect();
                }
                else if (ServerName == "Blue")
                {
                    Engine.Scp = new Scp(txtHostBlue.Text, txtUserBlue.Text, txtPasswordBlue.Text);
                    AddLog("Connection to Blue: " + txtHostBlue.Text + ":" + txtPortBlue.Text + " with " + txtUserBlue.Text + "/" + txtPasswordBlue.Text);
                    port = Convert.ToInt32(txtPortBlue.Text);                    
                }
                else throw new Exception("Unknown server '" + ServerName + "'");

                Engine.Scp.OnTransferStart += new FileTransferEvent(MyServer_OnTransferStart);
                Engine.Scp.OnTransferProgress += new FileTransferEvent(MyServer_OnTransferProgress);
                Engine.Scp.OnTransferEnd += new FileTransferEvent(MyServer_OnTransferEnd);

                Engine.Scp.Connect(port);
            }
            catch (Exception E)
            {
                Engine.AddLog(Engine.LogType.ltError, E.Message);
                return;
            }

            Engine.AddLog("Deploy " + PathFrom);

            if (NameTo == "")
            {
                FileInfo FI = new FileInfo(PathFrom);
                NameTo = FI.Name;
            }

            if (ServerName == "Orange")
                PathTo = "/home/var/www/osiris-sps" + PathTo + "/" + NameTo;
            if (ServerName == "Blue")
                PathTo = "/home" + PathTo + "/" + NameTo;


            Engine.Scp.To(PathFrom, PathTo);

            Engine.Scp.Close();
        }
                
        private void DoWork()
        {
            try
            {
                lstLogs.Items.Clear();

                AddLog(Engine.LogType.ltInfo, "Start.");

                //InternalTest();
                //return;

                /*
                if (chkDeploy.Checked)
                {                    
                    Engine.Scp = new Scp(txtHost.Text, txtUser.Text, txtPassword.Text);

                    Engine.Scp.OnTransferStart += new FileTransferEvent(MyServer_OnTransferStart);
                    Engine.Scp.OnTransferProgress += new FileTransferEvent(MyServer_OnTransferProgress);
                    Engine.Scp.OnTransferEnd += new FileTransferEvent(MyServer_OnTransferEnd);                     * 

                    try
                    {
                        AddLog("Connection to " + txtHost.Text + ":" + txtPassword.Text + " with " + txtUser.Text + "/" + txtPassword.Text);
                        Engine.Scp.Connect(Convert.ToInt32(txtPort.Text));
                    }
                    catch (Exception E)
                    {
                        Engine.AddLog(Engine.LogType.ltError, E.Message);
                        return;
                    }
                }
                */

                foreach (ListViewItem Item in lstPackages.Items)
                {
                    if (Item.Checked)
                    {
                        String Name = Item.Text;
                        String Platform = Item.SubItems[1].Text;
                        String Arch = Item.SubItems[2].Text;
                        String Type = Item.SubItems[3].Text;

                        String Version2 = txtVersion.Text;
                        String Version3 = txtVersion.Text + "." + txtVersion3.Text;

                        String Compiler = "";
                        if (Platform == "windows")
                            Compiler = cboCompilerWindows.Text;
                        else if (Platform == "linux")
                            Compiler = cboCompilerLinux.Text;
                        else if (Platform == "macosx")
                            Compiler = cboCompilerMacOSX.Text;

                        if(Compiler != "")
                            Engine.Generate(chkPackage.Checked, chkDeploy.Checked, chkReport.Checked, Name, Version2, Version3, Platform, Arch, Type, Compiler);
                    }
                }

                /*
                if (chkDeploy.Checked)
                {
                    Engine.Scp.Close();
                }
                */

                AddLog(Engine.LogType.ltInfo, "Done.");
            }
            catch (Exception E)
            {
                AddLog(Engine.LogType.ltError, "Unhandled exception: " + E.Message);

                if (chkDeploy.Checked)
                {
                    Engine.Scp.Close();
                }
            }
        }

        

        

        /*
        void ClearUnwantedFiles(DirectoryInfo di)        
        {
            //Shell("cmd.exe", String.Format("/c rd \"{0}\\.svn\" /s /q", di.FullName), true);
            Shell("cmd.exe", String.Format("/c rd \"{0}\\_work\" /s /q", di.FullName), true);
            Shell("cmd.exe", String.Format("/c del \"{0}\\xx*.*\" ", di.FullName), true);
            Shell("cmd.exe", String.Format("/c del \"{0}\\Thumbs.db\" ", di.FullName), true);
            Shell("cmd.exe", String.Format("/c del \"{0}\\*.ilk\" ", di.FullName), true);
            
            foreach (DirectoryInfo sdi in di.GetDirectories())
            {
                ClearUnwantedFiles(sdi);
            }
        }
        */

        /*
        void CopyDir(string From, string To)
        {
            if (removeOld)
            {
                Shell("cmd.exe", String.Format("/c rd \"{0}\\{1}\" /S /Q", PathBinWin32Release, RelativeName), true);
            }

            //Shell("cmd.exe", String.Format("/c xcopy \"{0}\\{2}\" \"{1}\\{2}\" /I /E", PathBinWin32Debug, PathBinWin32Release, RelativeName), true);
            XCopy(PathBinWin32Debug + "\\" + RelativeName, PathBinWin32Release + "\\" + RelativeName, "");
        }
        */







        protected override void OnClosed(EventArgs e)
        {
            Properties.Settings.Default.Save();

            base.OnClosed(e);
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            Helper.Platform.Init();
            
            AddPackage("Osiris", "windows", "i386", "Portable");
            AddPackage("Osiris", "windows", "i386", "Installer");
            AddPackage("Osiris", "windows", "i386", "Firefox");
            AddPackage("Osiris", "windows", "x86_64", "Portable");
            AddPackage("Osiris", "windows", "x86_64", "Installer");
            AddPackage("Osiris", "windows", "x86_64", "Firefox");
            AddPackage("Osiris", "linux", "i386", "Portable");
            AddPackage("Osiris", "linux", "i386", "Debian");
            AddPackage("Osiris", "linux", "i386", "Firefox");
            AddPackage("Osiris", "linux", "x86_64", "Portable");
            AddPackage("Osiris", "linux", "x86_64", "Debian");
            AddPackage("Osiris", "linux", "x86_64", "Firefox");
            AddPackage("Osiris", "macosx", "x86_64", "Portable");
            AddPackage("Osiris", "macosx", "x86_64", "DMG");
            AddPackage("Osiris", "macosx", "x86_64", "Firefox");
            AddPackage("VS Debugging Package", "windows", "i386", "Debug");
            AddPackage("VS Debugging Package", "windows", "x86_64", "Debug");

            txtReadme.Text = Helper.FileSystem.ReadTextFile(Engine.PathOmsData + "/readme.txt");
            cboCompilerWindows.SelectedIndex = 0;
            cboCompilerLinux.SelectedIndex = 0;
            cboCompilerMacOSX.SelectedIndex = 0;

            // Settings binder
            txtPasswordOrange.DataBindings.Add("Text", Properties.Settings.Default, "txtPasswordOrange", false, DataSourceUpdateMode.OnPropertyChanged);
            txtPasswordBlue.DataBindings.Add("Text", Properties.Settings.Default, "txtPasswordBlue", false, DataSourceUpdateMode.OnPropertyChanged);
            
            Engine.Welcome();            
        }

        private void cmdStart_Click(object sender, EventArgs e)
        {
            txtVersion.Enabled = false;
            chkPackage.Enabled = false;
            chkDeploy.Enabled = false;
            chkReport.Enabled = false;
            txtHostOrange.Enabled = false;
            txtPortOrange.Enabled = false;
            txtUserOrange.Enabled = false;
            txtPasswordOrange.Enabled = false;
            cmdStart.Enabled = false;

            tabMain.SelectedTab = tabLog;

            DoWork();

            txtVersion.Enabled = true;
            chkPackage.Enabled = true;
            chkDeploy.Enabled = true;
            chkReport.Enabled = true;
            txtHostOrange.Enabled = true;
            txtPortOrange.Enabled = true;
            txtUserOrange.Enabled = true;
            txtPasswordOrange.Enabled = true;
            cmdStart.Enabled = true;
            
        }

        void MyServer_OnTransferStart(string src, string dst, int transferredBytes, int totalBytes, string message)
        {            
        }

        void MyServer_OnTransferEnd(string src, string dst, int transferredBytes, int totalBytes, string message)
        {            
        }

        void MyServer_OnTransferProgress(string src, string dst, int transferredBytes, int totalBytes, string message)
        {
            pgrProgress.Maximum = totalBytes;
            pgrProgress.Value = transferredBytes;            
            AddLog(Engine.LogType.ltStatusBar, String.Format("{0} ({1} / {2} , From: '{3}' to '{4}')",message,transferredBytes,totalBytes,src,dst));                        
        }

        private void cmdPackagesDeselectAll_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem Item in lstPackages.Items)
            {
                Item.Checked = false;
            }
        }

        private void chkPackage_CheckedChanged(object sender, EventArgs e)
        {
            chkReport.Enabled = chkPackage.Checked;
            if (chkReport.Enabled == false)
                chkReport.Checked = false;
        }

        private void RunCommand(Commands.ICommand cmd)
        {
            try
            {
                tabMain.SelectedTab = tabLog;
                cmd.Run();
            }
            catch (Exception ex)
            {
                AddLog(ex);
            }
        }

        private void cmdGenerateExtensions_Click(object sender, EventArgs e)
        {
            Commands.GenerateExtensionsPackages cmd = new OMS.Commands.GenerateExtensionsPackages();
            RunCommand(cmd);
        }

        private void cmdDeployExtensionsAnubis_Click(object sender, EventArgs e)
        {
            Commands.DeployExtensionsAnubis cmd = new OMS.Commands.DeployExtensionsAnubis();
            RunCommand(cmd);
        }

        private void cmdCommandUpdateLicenseInSources_Click(object sender, EventArgs e)
        {
            Commands.UpdateLicenseInSources cmd = new OMS.Commands.UpdateLicenseInSources();
            RunCommand(cmd);
            
        }

        

        
        

    }
}
