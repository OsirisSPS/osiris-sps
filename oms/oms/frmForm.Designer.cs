namespace OMS
{
    partial class frmForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmForm));
			this.imgLogs = new System.Windows.Forms.ImageList(this.components);
			this.tabMain = new System.Windows.Forms.TabControl();
			this.tabWelcome = new System.Windows.Forms.TabPage();
			this.txtReadme = new System.Windows.Forms.TextBox();
			this.tabSettings = new System.Windows.Forms.TabPage();
			this.grpDeployBlue = new System.Windows.Forms.GroupBox();
			this.txtPasswordBlue = new System.Windows.Forms.TextBox();
			this.lblPasswordBlue = new System.Windows.Forms.Label();
			this.txtUserBlue = new System.Windows.Forms.TextBox();
			this.lblUserBlue = new System.Windows.Forms.Label();
			this.txtPortBlue = new System.Windows.Forms.TextBox();
			this.lblPortBlue = new System.Windows.Forms.Label();
			this.txtHostBlue = new System.Windows.Forms.TextBox();
			this.lblHostBlue = new System.Windows.Forms.Label();
			this.grpCompiler = new System.Windows.Forms.GroupBox();
			this.cboCompilerMacOSX = new System.Windows.Forms.ComboBox();
			this.label6 = new System.Windows.Forms.Label();
			this.cboCompilerLinux = new System.Windows.Forms.ComboBox();
			this.cboCompilerWindows = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.cmdPackagesDeselectAll = new System.Windows.Forms.Button();
			this.grpPackageFirefox = new System.Windows.Forms.GroupBox();
			this.txtFirefoxVersionMax = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.txtFirefoxVersionMin = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.lblVersion3 = new System.Windows.Forms.Label();
			this.txtVersion3 = new System.Windows.Forms.TextBox();
			this.grpPackage = new System.Windows.Forms.GroupBox();
			this.lstPackages = new System.Windows.Forms.ListView();
			this.colProject = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.colPlatform = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.colArchitecture = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.colType = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.grpDeployOrange = new System.Windows.Forms.GroupBox();
			this.txtPasswordOrange = new System.Windows.Forms.TextBox();
			this.lblPasswordOrange = new System.Windows.Forms.Label();
			this.txtUserOrange = new System.Windows.Forms.TextBox();
			this.lblUserOrange = new System.Windows.Forms.Label();
			this.txtPortOrange = new System.Windows.Forms.TextBox();
			this.lblPortOrange = new System.Windows.Forms.Label();
			this.txtHostOrange = new System.Windows.Forms.TextBox();
			this.lblHostOrange = new System.Windows.Forms.Label();
			this.chkLogsVerbose = new System.Windows.Forms.CheckBox();
			this.chkReport = new System.Windows.Forms.CheckBox();
			this.chkDeploy = new System.Windows.Forms.CheckBox();
			this.chkPackage = new System.Windows.Forms.CheckBox();
			this.label1 = new System.Windows.Forms.Label();
			this.cmdStart = new System.Windows.Forms.Button();
			this.txtVersion = new System.Windows.Forms.TextBox();
			this.tabCommands = new System.Windows.Forms.TabPage();
			this.cmdDeployExtensionsAnubis = new System.Windows.Forms.Button();
			this.cmdCommandUpdateLicenseInSources = new System.Windows.Forms.Button();
			this.cmdGenerateExtensions = new System.Windows.Forms.Button();
			this.tabLog = new System.Windows.Forms.TabPage();
			this.splLogMain = new System.Windows.Forms.SplitContainer();
			this.lstLogs = new System.Windows.Forms.ListView();
			this.colLogMessage = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.splLogStatus = new System.Windows.Forms.SplitContainer();
			this.pgrProgress = new System.Windows.Forms.ProgressBar();
			this.lblLog = new System.Windows.Forms.Label();
			this.tabMain.SuspendLayout();
			this.tabWelcome.SuspendLayout();
			this.tabSettings.SuspendLayout();
			this.grpDeployBlue.SuspendLayout();
			this.grpCompiler.SuspendLayout();
			this.grpPackageFirefox.SuspendLayout();
			this.grpPackage.SuspendLayout();
			this.grpDeployOrange.SuspendLayout();
			this.tabCommands.SuspendLayout();
			this.tabLog.SuspendLayout();
			this.splLogMain.Panel1.SuspendLayout();
			this.splLogMain.Panel2.SuspendLayout();
			this.splLogMain.SuspendLayout();
			this.splLogStatus.Panel1.SuspendLayout();
			this.splLogStatus.Panel2.SuspendLayout();
			this.splLogStatus.SuspendLayout();
			this.SuspendLayout();
			// 
			// imgLogs
			// 
			this.imgLogs.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imgLogs.ImageStream")));
			this.imgLogs.TransparentColor = System.Drawing.Color.Transparent;
			this.imgLogs.Images.SetKeyName(0, "icon_info.png");
			this.imgLogs.Images.SetKeyName(1, "icon_warning.png");
			this.imgLogs.Images.SetKeyName(2, "icon_error.png");
			// 
			// tabMain
			// 
			this.tabMain.Controls.Add(this.tabWelcome);
			this.tabMain.Controls.Add(this.tabSettings);
			this.tabMain.Controls.Add(this.tabCommands);
			this.tabMain.Controls.Add(this.tabLog);
			this.tabMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabMain.Location = new System.Drawing.Point(0, 0);
			this.tabMain.Name = "tabMain";
			this.tabMain.SelectedIndex = 0;
			this.tabMain.Size = new System.Drawing.Size(765, 460);
			this.tabMain.TabIndex = 30;
			// 
			// tabWelcome
			// 
			this.tabWelcome.Controls.Add(this.txtReadme);
			this.tabWelcome.Location = new System.Drawing.Point(4, 22);
			this.tabWelcome.Name = "tabWelcome";
			this.tabWelcome.Padding = new System.Windows.Forms.Padding(3);
			this.tabWelcome.Size = new System.Drawing.Size(757, 434);
			this.tabWelcome.TabIndex = 0;
			this.tabWelcome.Text = "Welcome";
			this.tabWelcome.UseVisualStyleBackColor = true;
			// 
			// txtReadme
			// 
			this.txtReadme.Dock = System.Windows.Forms.DockStyle.Fill;
			this.txtReadme.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtReadme.Location = new System.Drawing.Point(3, 3);
			this.txtReadme.Multiline = true;
			this.txtReadme.Name = "txtReadme";
			this.txtReadme.ReadOnly = true;
			this.txtReadme.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.txtReadme.Size = new System.Drawing.Size(751, 428);
			this.txtReadme.TabIndex = 0;
			// 
			// tabSettings
			// 
			this.tabSettings.Controls.Add(this.grpDeployBlue);
			this.tabSettings.Controls.Add(this.grpCompiler);
			this.tabSettings.Controls.Add(this.cmdPackagesDeselectAll);
			this.tabSettings.Controls.Add(this.grpPackageFirefox);
			this.tabSettings.Controls.Add(this.lblVersion3);
			this.tabSettings.Controls.Add(this.txtVersion3);
			this.tabSettings.Controls.Add(this.grpPackage);
			this.tabSettings.Controls.Add(this.grpDeployOrange);
			this.tabSettings.Controls.Add(this.chkLogsVerbose);
			this.tabSettings.Controls.Add(this.chkReport);
			this.tabSettings.Controls.Add(this.chkDeploy);
			this.tabSettings.Controls.Add(this.chkPackage);
			this.tabSettings.Controls.Add(this.label1);
			this.tabSettings.Controls.Add(this.cmdStart);
			this.tabSettings.Controls.Add(this.txtVersion);
			this.tabSettings.Location = new System.Drawing.Point(4, 22);
			this.tabSettings.Name = "tabSettings";
			this.tabSettings.Padding = new System.Windows.Forms.Padding(3);
			this.tabSettings.Size = new System.Drawing.Size(757, 434);
			this.tabSettings.TabIndex = 1;
			this.tabSettings.Text = "Build";
			this.tabSettings.UseVisualStyleBackColor = true;
			// 
			// grpDeployBlue
			// 
			this.grpDeployBlue.Controls.Add(this.txtPasswordBlue);
			this.grpDeployBlue.Controls.Add(this.lblPasswordBlue);
			this.grpDeployBlue.Controls.Add(this.txtUserBlue);
			this.grpDeployBlue.Controls.Add(this.lblUserBlue);
			this.grpDeployBlue.Controls.Add(this.txtPortBlue);
			this.grpDeployBlue.Controls.Add(this.lblPortBlue);
			this.grpDeployBlue.Controls.Add(this.txtHostBlue);
			this.grpDeployBlue.Controls.Add(this.lblHostBlue);
			this.grpDeployBlue.Location = new System.Drawing.Point(8, 181);
			this.grpDeployBlue.Name = "grpDeployBlue";
			this.grpDeployBlue.Size = new System.Drawing.Size(306, 77);
			this.grpDeployBlue.TabIndex = 43;
			this.grpDeployBlue.TabStop = false;
			this.grpDeployBlue.Text = "Deployment - Blue Machine (Isis Official)";
			// 
			// txtPasswordBlue
			// 
			this.txtPasswordBlue.Location = new System.Drawing.Point(189, 45);
			this.txtPasswordBlue.Name = "txtPasswordBlue";
			this.txtPasswordBlue.Size = new System.Drawing.Size(90, 20);
			this.txtPasswordBlue.TabIndex = 28;
			this.txtPasswordBlue.UseSystemPasswordChar = true;
			// 
			// lblPasswordBlue
			// 
			this.lblPasswordBlue.AutoSize = true;
			this.lblPasswordBlue.Location = new System.Drawing.Point(135, 48);
			this.lblPasswordBlue.Name = "lblPasswordBlue";
			this.lblPasswordBlue.Size = new System.Drawing.Size(56, 13);
			this.lblPasswordBlue.TabIndex = 27;
			this.lblPasswordBlue.Text = "Password:";
			// 
			// txtUserBlue
			// 
			this.txtUserBlue.Location = new System.Drawing.Point(42, 45);
			this.txtUserBlue.Name = "txtUserBlue";
			this.txtUserBlue.Size = new System.Drawing.Size(90, 20);
			this.txtUserBlue.TabIndex = 26;
			this.txtUserBlue.Text = "root";
			// 
			// lblUserBlue
			// 
			this.lblUserBlue.AutoSize = true;
			this.lblUserBlue.Location = new System.Drawing.Point(9, 48);
			this.lblUserBlue.Name = "lblUserBlue";
			this.lblUserBlue.Size = new System.Drawing.Size(32, 13);
			this.lblUserBlue.TabIndex = 25;
			this.lblUserBlue.Text = "User:";
			// 
			// txtPortBlue
			// 
			this.txtPortBlue.Location = new System.Drawing.Point(236, 19);
			this.txtPortBlue.Name = "txtPortBlue";
			this.txtPortBlue.Size = new System.Drawing.Size(43, 20);
			this.txtPortBlue.TabIndex = 24;
			this.txtPortBlue.Text = "22";
			// 
			// lblPortBlue
			// 
			this.lblPortBlue.AutoSize = true;
			this.lblPortBlue.Location = new System.Drawing.Point(202, 22);
			this.lblPortBlue.Name = "lblPortBlue";
			this.lblPortBlue.Size = new System.Drawing.Size(29, 13);
			this.lblPortBlue.TabIndex = 23;
			this.lblPortBlue.Text = "Port:";
			// 
			// txtHostBlue
			// 
			this.txtHostBlue.Location = new System.Drawing.Point(42, 19);
			this.txtHostBlue.Name = "txtHostBlue";
			this.txtHostBlue.Size = new System.Drawing.Size(154, 20);
			this.txtHostBlue.TabIndex = 22;
			this.txtHostBlue.Text = "blue.osiris-sps.org";
			// 
			// lblHostBlue
			// 
			this.lblHostBlue.AutoSize = true;
			this.lblHostBlue.Location = new System.Drawing.Point(9, 22);
			this.lblHostBlue.Name = "lblHostBlue";
			this.lblHostBlue.Size = new System.Drawing.Size(32, 13);
			this.lblHostBlue.TabIndex = 21;
			this.lblHostBlue.Text = "Host:";
			// 
			// grpCompiler
			// 
			this.grpCompiler.Controls.Add(this.cboCompilerMacOSX);
			this.grpCompiler.Controls.Add(this.label6);
			this.grpCompiler.Controls.Add(this.cboCompilerLinux);
			this.grpCompiler.Controls.Add(this.cboCompilerWindows);
			this.grpCompiler.Controls.Add(this.label2);
			this.grpCompiler.Controls.Add(this.label4);
			this.grpCompiler.Location = new System.Drawing.Point(167, 266);
			this.grpCompiler.Name = "grpCompiler";
			this.grpCompiler.Size = new System.Drawing.Size(148, 109);
			this.grpCompiler.TabIndex = 42;
			this.grpCompiler.TabStop = false;
			this.grpCompiler.Text = "Compiler";
			// 
			// cboCompilerMacOSX
			// 
			this.cboCompilerMacOSX.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboCompilerMacOSX.FormattingEnabled = true;
			this.cboCompilerMacOSX.Items.AddRange(new object[] {
            "gcc"});
			this.cboCompilerMacOSX.Location = new System.Drawing.Point(78, 73);
			this.cboCompilerMacOSX.Name = "cboCompilerMacOSX";
			this.cboCompilerMacOSX.Size = new System.Drawing.Size(64, 21);
			this.cboCompilerMacOSX.TabIndex = 30;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(9, 77);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(59, 13);
			this.label6.TabIndex = 29;
			this.label6.Text = "Mac OS-X:";
			// 
			// cboCompilerLinux
			// 
			this.cboCompilerLinux.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboCompilerLinux.FormattingEnabled = true;
			this.cboCompilerLinux.Items.AddRange(new object[] {
            "gcc"});
			this.cboCompilerLinux.Location = new System.Drawing.Point(78, 46);
			this.cboCompilerLinux.Name = "cboCompilerLinux";
			this.cboCompilerLinux.Size = new System.Drawing.Size(64, 21);
			this.cboCompilerLinux.TabIndex = 28;
			// 
			// cboCompilerWindows
			// 
			this.cboCompilerWindows.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboCompilerWindows.FormattingEnabled = true;
			this.cboCompilerWindows.Items.AddRange(new object[] {
            "vc10",
            "vc8"});
			this.cboCompilerWindows.Location = new System.Drawing.Point(78, 20);
			this.cboCompilerWindows.Name = "cboCompilerWindows";
			this.cboCompilerWindows.Size = new System.Drawing.Size(64, 21);
			this.cboCompilerWindows.TabIndex = 27;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(9, 50);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(35, 13);
			this.label2.TabIndex = 25;
			this.label2.Text = "Linux:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(9, 24);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(54, 13);
			this.label4.TabIndex = 21;
			this.label4.Text = "Windows:";
			// 
			// cmdPackagesDeselectAll
			// 
			this.cmdPackagesDeselectAll.Location = new System.Drawing.Point(595, 402);
			this.cmdPackagesDeselectAll.Name = "cmdPackagesDeselectAll";
			this.cmdPackagesDeselectAll.Size = new System.Drawing.Size(155, 24);
			this.cmdPackagesDeselectAll.TabIndex = 41;
			this.cmdPackagesDeselectAll.Text = "Deselect All";
			this.cmdPackagesDeselectAll.UseVisualStyleBackColor = true;
			this.cmdPackagesDeselectAll.Click += new System.EventHandler(this.cmdPackagesDeselectAll_Click);
			// 
			// grpPackageFirefox
			// 
			this.grpPackageFirefox.Controls.Add(this.txtFirefoxVersionMax);
			this.grpPackageFirefox.Controls.Add(this.label3);
			this.grpPackageFirefox.Controls.Add(this.txtFirefoxVersionMin);
			this.grpPackageFirefox.Controls.Add(this.label5);
			this.grpPackageFirefox.Location = new System.Drawing.Point(9, 266);
			this.grpPackageFirefox.Name = "grpPackageFirefox";
			this.grpPackageFirefox.Size = new System.Drawing.Size(148, 77);
			this.grpPackageFirefox.TabIndex = 38;
			this.grpPackageFirefox.TabStop = false;
			this.grpPackageFirefox.Text = "Type: Firefox";
			// 
			// txtFirefoxVersionMax
			// 
			this.txtFirefoxVersionMax.Location = new System.Drawing.Point(80, 47);
			this.txtFirefoxVersionMax.Name = "txtFirefoxVersionMax";
			this.txtFirefoxVersionMax.Size = new System.Drawing.Size(52, 20);
			this.txtFirefoxVersionMax.TabIndex = 26;
			this.txtFirefoxVersionMax.Text = "50.6.*";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(9, 50);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(68, 13);
			this.label3.TabIndex = 25;
			this.label3.Text = "Version Max:";
			// 
			// txtFirefoxVersionMin
			// 
			this.txtFirefoxVersionMin.Location = new System.Drawing.Point(80, 21);
			this.txtFirefoxVersionMin.Name = "txtFirefoxVersionMin";
			this.txtFirefoxVersionMin.Size = new System.Drawing.Size(52, 20);
			this.txtFirefoxVersionMin.TabIndex = 22;
			this.txtFirefoxVersionMin.Text = "3.0";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(9, 24);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(65, 13);
			this.label5.TabIndex = 21;
			this.label5.Text = "Version Min:";
			// 
			// lblVersion3
			// 
			this.lblVersion3.AutoSize = true;
			this.lblVersion3.Location = new System.Drawing.Point(163, 9);
			this.lblVersion3.Name = "lblVersion3";
			this.lblVersion3.Size = new System.Drawing.Size(63, 13);
			this.lblVersion3.TabIndex = 40;
			this.lblVersion3.Text = "Subversion:";
			// 
			// txtVersion3
			// 
			this.txtVersion3.Location = new System.Drawing.Point(229, 6);
			this.txtVersion3.Name = "txtVersion3";
			this.txtVersion3.Size = new System.Drawing.Size(39, 20);
			this.txtVersion3.TabIndex = 39;
			this.txtVersion3.Text = "0";
			// 
			// grpPackage
			// 
			this.grpPackage.Controls.Add(this.lstPackages);
			this.grpPackage.Location = new System.Drawing.Point(333, 9);
			this.grpPackage.Name = "grpPackage";
			this.grpPackage.Size = new System.Drawing.Size(420, 387);
			this.grpPackage.TabIndex = 38;
			this.grpPackage.TabStop = false;
			this.grpPackage.Text = "Packages";
			// 
			// lstPackages
			// 
			this.lstPackages.CheckBoxes = true;
			this.lstPackages.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colProject,
            this.colPlatform,
            this.colArchitecture,
            this.colType});
			this.lstPackages.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lstPackages.FullRowSelect = true;
			this.lstPackages.GridLines = true;
			this.lstPackages.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.lstPackages.HideSelection = false;
			this.lstPackages.LargeImageList = this.imgLogs;
			this.lstPackages.Location = new System.Drawing.Point(3, 16);
			this.lstPackages.Name = "lstPackages";
			this.lstPackages.Size = new System.Drawing.Size(414, 368);
			this.lstPackages.SmallImageList = this.imgLogs;
			this.lstPackages.TabIndex = 30;
			this.lstPackages.UseCompatibleStateImageBehavior = false;
			this.lstPackages.View = System.Windows.Forms.View.Details;
			// 
			// colProject
			// 
			this.colProject.Text = "Project";
			this.colProject.Width = 150;
			// 
			// colPlatform
			// 
			this.colPlatform.Text = "Platform";
			this.colPlatform.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// colArchitecture
			// 
			this.colArchitecture.Text = "Architecture";
			this.colArchitecture.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.colArchitecture.Width = 70;
			// 
			// colType
			// 
			this.colType.Text = "Type";
			this.colType.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.colType.Width = 120;
			// 
			// grpDeployOrange
			// 
			this.grpDeployOrange.Controls.Add(this.txtPasswordOrange);
			this.grpDeployOrange.Controls.Add(this.lblPasswordOrange);
			this.grpDeployOrange.Controls.Add(this.txtUserOrange);
			this.grpDeployOrange.Controls.Add(this.lblUserOrange);
			this.grpDeployOrange.Controls.Add(this.txtPortOrange);
			this.grpDeployOrange.Controls.Add(this.lblPortOrange);
			this.grpDeployOrange.Controls.Add(this.txtHostOrange);
			this.grpDeployOrange.Controls.Add(this.lblHostOrange);
			this.grpDeployOrange.Location = new System.Drawing.Point(9, 98);
			this.grpDeployOrange.Name = "grpDeployOrange";
			this.grpDeployOrange.Size = new System.Drawing.Size(306, 77);
			this.grpDeployOrange.TabIndex = 37;
			this.grpDeployOrange.TabStop = false;
			this.grpDeployOrange.Text = "Deployment - Orange Machine (Osiris project)";
			// 
			// txtPasswordOrange
			// 
			this.txtPasswordOrange.Location = new System.Drawing.Point(189, 45);
			this.txtPasswordOrange.Name = "txtPasswordOrange";
			this.txtPasswordOrange.Size = new System.Drawing.Size(90, 20);
			this.txtPasswordOrange.TabIndex = 28;
			this.txtPasswordOrange.UseSystemPasswordChar = true;
			// 
			// lblPasswordOrange
			// 
			this.lblPasswordOrange.AutoSize = true;
			this.lblPasswordOrange.Location = new System.Drawing.Point(135, 48);
			this.lblPasswordOrange.Name = "lblPasswordOrange";
			this.lblPasswordOrange.Size = new System.Drawing.Size(56, 13);
			this.lblPasswordOrange.TabIndex = 27;
			this.lblPasswordOrange.Text = "Password:";
			// 
			// txtUserOrange
			// 
			this.txtUserOrange.Location = new System.Drawing.Point(42, 45);
			this.txtUserOrange.Name = "txtUserOrange";
			this.txtUserOrange.Size = new System.Drawing.Size(90, 20);
			this.txtUserOrange.TabIndex = 26;
			this.txtUserOrange.Text = "root";
			// 
			// lblUserOrange
			// 
			this.lblUserOrange.AutoSize = true;
			this.lblUserOrange.Location = new System.Drawing.Point(9, 48);
			this.lblUserOrange.Name = "lblUserOrange";
			this.lblUserOrange.Size = new System.Drawing.Size(32, 13);
			this.lblUserOrange.TabIndex = 25;
			this.lblUserOrange.Text = "User:";
			// 
			// txtPortOrange
			// 
			this.txtPortOrange.Location = new System.Drawing.Point(236, 19);
			this.txtPortOrange.Name = "txtPortOrange";
			this.txtPortOrange.Size = new System.Drawing.Size(43, 20);
			this.txtPortOrange.TabIndex = 24;
			this.txtPortOrange.Text = "22";
			// 
			// lblPortOrange
			// 
			this.lblPortOrange.AutoSize = true;
			this.lblPortOrange.Location = new System.Drawing.Point(202, 22);
			this.lblPortOrange.Name = "lblPortOrange";
			this.lblPortOrange.Size = new System.Drawing.Size(29, 13);
			this.lblPortOrange.TabIndex = 23;
			this.lblPortOrange.Text = "Port:";
			// 
			// txtHostOrange
			// 
			this.txtHostOrange.Location = new System.Drawing.Point(42, 19);
			this.txtHostOrange.Name = "txtHostOrange";
			this.txtHostOrange.Size = new System.Drawing.Size(154, 20);
			this.txtHostOrange.TabIndex = 22;
			this.txtHostOrange.Text = "orange.osiris-sps.org";
			// 
			// lblHostOrange
			// 
			this.lblHostOrange.AutoSize = true;
			this.lblHostOrange.Location = new System.Drawing.Point(9, 22);
			this.lblHostOrange.Name = "lblHostOrange";
			this.lblHostOrange.Size = new System.Drawing.Size(32, 13);
			this.lblHostOrange.TabIndex = 21;
			this.lblHostOrange.Text = "Host:";
			// 
			// chkLogsVerbose
			// 
			this.chkLogsVerbose.AutoSize = true;
			this.chkLogsVerbose.Location = new System.Drawing.Point(84, 34);
			this.chkLogsVerbose.Name = "chkLogsVerbose";
			this.chkLogsVerbose.Size = new System.Drawing.Size(86, 17);
			this.chkLogsVerbose.TabIndex = 36;
			this.chkLogsVerbose.Text = "Log Verbose";
			this.chkLogsVerbose.UseVisualStyleBackColor = true;
			// 
			// chkReport
			// 
			this.chkReport.AutoSize = true;
			this.chkReport.Checked = true;
			this.chkReport.CheckState = System.Windows.Forms.CheckState.Checked;
			this.chkReport.Location = new System.Drawing.Point(9, 55);
			this.chkReport.Name = "chkReport";
			this.chkReport.Size = new System.Drawing.Size(58, 17);
			this.chkReport.TabIndex = 35;
			this.chkReport.Text = "Report";
			this.chkReport.UseVisualStyleBackColor = true;
			// 
			// chkDeploy
			// 
			this.chkDeploy.AutoSize = true;
			this.chkDeploy.Location = new System.Drawing.Point(9, 76);
			this.chkDeploy.Name = "chkDeploy";
			this.chkDeploy.Size = new System.Drawing.Size(59, 17);
			this.chkDeploy.TabIndex = 34;
			this.chkDeploy.Text = "Deploy";
			this.chkDeploy.UseVisualStyleBackColor = true;
			// 
			// chkPackage
			// 
			this.chkPackage.AutoSize = true;
			this.chkPackage.Checked = true;
			this.chkPackage.CheckState = System.Windows.Forms.CheckState.Checked;
			this.chkPackage.Location = new System.Drawing.Point(9, 34);
			this.chkPackage.Name = "chkPackage";
			this.chkPackage.Size = new System.Drawing.Size(49, 17);
			this.chkPackage.TabIndex = 33;
			this.chkPackage.Text = "Build";
			this.chkPackage.UseVisualStyleBackColor = true;
			this.chkPackage.CheckedChanged += new System.EventHandler(this.chkPackage_CheckedChanged);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 13);
			this.label1.TabIndex = 32;
			this.label1.Text = "Version:";
			// 
			// cmdStart
			// 
			this.cmdStart.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.cmdStart.Location = new System.Drawing.Point(9, 381);
			this.cmdStart.Name = "cmdStart";
			this.cmdStart.Size = new System.Drawing.Size(214, 37);
			this.cmdStart.TabIndex = 31;
			this.cmdStart.Text = "&Start";
			this.cmdStart.UseVisualStyleBackColor = true;
			this.cmdStart.Click += new System.EventHandler(this.cmdStart_Click);
			// 
			// txtVersion
			// 
			this.txtVersion.Location = new System.Drawing.Point(57, 6);
			this.txtVersion.Name = "txtVersion";
			this.txtVersion.Size = new System.Drawing.Size(100, 20);
			this.txtVersion.TabIndex = 30;
			this.txtVersion.Text = "1.0";
			// 
			// tabCommands
			// 
			this.tabCommands.Controls.Add(this.cmdDeployExtensionsAnubis);
			this.tabCommands.Controls.Add(this.cmdCommandUpdateLicenseInSources);
			this.tabCommands.Controls.Add(this.cmdGenerateExtensions);
			this.tabCommands.Location = new System.Drawing.Point(4, 22);
			this.tabCommands.Name = "tabCommands";
			this.tabCommands.Size = new System.Drawing.Size(757, 434);
			this.tabCommands.TabIndex = 3;
			this.tabCommands.Text = "Other commands";
			this.tabCommands.UseVisualStyleBackColor = true;
			// 
			// cmdDeployExtensionsAnubis
			// 
			this.cmdDeployExtensionsAnubis.Location = new System.Drawing.Point(8, 159);
			this.cmdDeployExtensionsAnubis.Name = "cmdDeployExtensionsAnubis";
			this.cmdDeployExtensionsAnubis.Size = new System.Drawing.Size(741, 116);
			this.cmdDeployExtensionsAnubis.TabIndex = 2;
			this.cmdDeployExtensionsAnubis.Text = "\r\nDownload some language to the preinstalled extensions directory\r\n(Disabled: Dow" +
    "nload Anubis extensions (language typically) and upload to Isis.)";
			this.cmdDeployExtensionsAnubis.UseVisualStyleBackColor = true;
			this.cmdDeployExtensionsAnubis.Click += new System.EventHandler(this.cmdDeployExtensionsAnubis_Click);
			// 
			// cmdCommandUpdateLicenseInSources
			// 
			this.cmdCommandUpdateLicenseInSources.Location = new System.Drawing.Point(8, 360);
			this.cmdCommandUpdateLicenseInSources.Name = "cmdCommandUpdateLicenseInSources";
			this.cmdCommandUpdateLicenseInSources.Size = new System.Drawing.Size(741, 66);
			this.cmdCommandUpdateLicenseInSources.TabIndex = 1;
			this.cmdCommandUpdateLicenseInSources.Text = "Update licenses in source\r\n( Browse every source files, and check license text in" +
    " top of the file. Use the file \"template_license.txt\" as template )\r\n";
			this.cmdCommandUpdateLicenseInSources.UseVisualStyleBackColor = true;
			this.cmdCommandUpdateLicenseInSources.Click += new System.EventHandler(this.cmdCommandUpdateLicenseInSources_Click);
			// 
			// cmdGenerateExtensions
			// 
			this.cmdGenerateExtensions.Location = new System.Drawing.Point(8, 12);
			this.cmdGenerateExtensions.Name = "cmdGenerateExtensions";
			this.cmdGenerateExtensions.Size = new System.Drawing.Size(741, 116);
			this.cmdGenerateExtensions.TabIndex = 0;
			this.cmdGenerateExtensions.Text = resources.GetString("cmdGenerateExtensions.Text");
			this.cmdGenerateExtensions.UseVisualStyleBackColor = true;
			this.cmdGenerateExtensions.Click += new System.EventHandler(this.cmdGenerateExtensions_Click);
			// 
			// tabLog
			// 
			this.tabLog.Controls.Add(this.splLogMain);
			this.tabLog.Location = new System.Drawing.Point(4, 22);
			this.tabLog.Name = "tabLog";
			this.tabLog.Size = new System.Drawing.Size(757, 434);
			this.tabLog.TabIndex = 2;
			this.tabLog.Text = "Log";
			this.tabLog.UseVisualStyleBackColor = true;
			// 
			// splLogMain
			// 
			this.splLogMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splLogMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
			this.splLogMain.IsSplitterFixed = true;
			this.splLogMain.Location = new System.Drawing.Point(0, 0);
			this.splLogMain.Name = "splLogMain";
			this.splLogMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splLogMain.Panel1
			// 
			this.splLogMain.Panel1.Controls.Add(this.lstLogs);
			// 
			// splLogMain.Panel2
			// 
			this.splLogMain.Panel2.Controls.Add(this.splLogStatus);
			this.splLogMain.Size = new System.Drawing.Size(757, 434);
			this.splLogMain.SplitterDistance = 382;
			this.splLogMain.TabIndex = 0;
			// 
			// lstLogs
			// 
			this.lstLogs.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colLogMessage});
			this.lstLogs.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lstLogs.FullRowSelect = true;
			this.lstLogs.GridLines = true;
			this.lstLogs.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.lstLogs.HideSelection = false;
			this.lstLogs.LargeImageList = this.imgLogs;
			this.lstLogs.Location = new System.Drawing.Point(0, 0);
			this.lstLogs.MultiSelect = false;
			this.lstLogs.Name = "lstLogs";
			this.lstLogs.Size = new System.Drawing.Size(757, 382);
			this.lstLogs.SmallImageList = this.imgLogs;
			this.lstLogs.TabIndex = 28;
			this.lstLogs.UseCompatibleStateImageBehavior = false;
			this.lstLogs.View = System.Windows.Forms.View.Details;
			// 
			// colLogMessage
			// 
			this.colLogMessage.Text = "Message";
			this.colLogMessage.Width = 2000;
			// 
			// splLogStatus
			// 
			this.splLogStatus.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splLogStatus.IsSplitterFixed = true;
			this.splLogStatus.Location = new System.Drawing.Point(0, 0);
			this.splLogStatus.Name = "splLogStatus";
			this.splLogStatus.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splLogStatus.Panel1
			// 
			this.splLogStatus.Panel1.Controls.Add(this.pgrProgress);
			// 
			// splLogStatus.Panel2
			// 
			this.splLogStatus.Panel2.Controls.Add(this.lblLog);
			this.splLogStatus.Size = new System.Drawing.Size(757, 48);
			this.splLogStatus.SplitterDistance = 25;
			this.splLogStatus.TabIndex = 0;
			// 
			// pgrProgress
			// 
			this.pgrProgress.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pgrProgress.Location = new System.Drawing.Point(0, 0);
			this.pgrProgress.Name = "pgrProgress";
			this.pgrProgress.Size = new System.Drawing.Size(757, 25);
			this.pgrProgress.TabIndex = 23;
			// 
			// lblLog
			// 
			this.lblLog.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.lblLog.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lblLog.Location = new System.Drawing.Point(0, 0);
			this.lblLog.Name = "lblLog";
			this.lblLog.Size = new System.Drawing.Size(757, 25);
			this.lblLog.TabIndex = 7;
			this.lblLog.Text = "Log";
			// 
			// frmForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(765, 460);
			this.Controls.Add(this.tabMain);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "frmForm";
			this.Text = "Osiris Management System";
			this.tabMain.ResumeLayout(false);
			this.tabWelcome.ResumeLayout(false);
			this.tabWelcome.PerformLayout();
			this.tabSettings.ResumeLayout(false);
			this.tabSettings.PerformLayout();
			this.grpDeployBlue.ResumeLayout(false);
			this.grpDeployBlue.PerformLayout();
			this.grpCompiler.ResumeLayout(false);
			this.grpCompiler.PerformLayout();
			this.grpPackageFirefox.ResumeLayout(false);
			this.grpPackageFirefox.PerformLayout();
			this.grpPackage.ResumeLayout(false);
			this.grpDeployOrange.ResumeLayout(false);
			this.grpDeployOrange.PerformLayout();
			this.tabCommands.ResumeLayout(false);
			this.tabLog.ResumeLayout(false);
			this.splLogMain.Panel1.ResumeLayout(false);
			this.splLogMain.Panel2.ResumeLayout(false);
			this.splLogMain.ResumeLayout(false);
			this.splLogStatus.Panel1.ResumeLayout(false);
			this.splLogStatus.Panel2.ResumeLayout(false);
			this.splLogStatus.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ImageList imgLogs;
        private System.Windows.Forms.TabControl tabMain;
        private System.Windows.Forms.TabPage tabWelcome;
        private System.Windows.Forms.TabPage tabSettings;
        private System.Windows.Forms.TabPage tabLog;
        private System.Windows.Forms.SplitContainer splLogMain;
        private System.Windows.Forms.ListView lstLogs;
        private System.Windows.Forms.ColumnHeader colLogMessage;
        private System.Windows.Forms.SplitContainer splLogStatus;
        private System.Windows.Forms.TextBox txtReadme;
        private System.Windows.Forms.GroupBox grpDeployOrange;
        private System.Windows.Forms.TextBox txtPasswordOrange;
        private System.Windows.Forms.Label lblPasswordOrange;
        private System.Windows.Forms.TextBox txtUserOrange;
        private System.Windows.Forms.Label lblUserOrange;
        private System.Windows.Forms.TextBox txtPortOrange;
        private System.Windows.Forms.Label lblPortOrange;
        private System.Windows.Forms.TextBox txtHostOrange;
        private System.Windows.Forms.Label lblHostOrange;
        private System.Windows.Forms.CheckBox chkLogsVerbose;
        private System.Windows.Forms.CheckBox chkReport;
        private System.Windows.Forms.CheckBox chkDeploy;
        private System.Windows.Forms.CheckBox chkPackage;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button cmdStart;
        private System.Windows.Forms.TextBox txtVersion;
        private System.Windows.Forms.ProgressBar pgrProgress;
        private System.Windows.Forms.Label lblLog;
        private System.Windows.Forms.GroupBox grpPackage;
        private System.Windows.Forms.ListView lstPackages;
        private System.Windows.Forms.ColumnHeader colProject;
        private System.Windows.Forms.ColumnHeader colPlatform;
        private System.Windows.Forms.ColumnHeader colArchitecture;
        private System.Windows.Forms.ColumnHeader colType;
        private System.Windows.Forms.Label lblVersion3;
        private System.Windows.Forms.TextBox txtVersion3;
        private System.Windows.Forms.GroupBox grpPackageFirefox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label5;
        public System.Windows.Forms.TextBox txtFirefoxVersionMax;
        public System.Windows.Forms.TextBox txtFirefoxVersionMin;
        private System.Windows.Forms.Button cmdPackagesDeselectAll;
        private System.Windows.Forms.TabPage tabCommands;
        private System.Windows.Forms.Button cmdGenerateExtensions;
        private System.Windows.Forms.Button cmdCommandUpdateLicenseInSources;
        private System.Windows.Forms.GroupBox grpCompiler;
        private System.Windows.Forms.ComboBox cboCompilerMacOSX;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox cboCompilerLinux;
        private System.Windows.Forms.ComboBox cboCompilerWindows;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox grpDeployBlue;
        private System.Windows.Forms.TextBox txtPasswordBlue;
        private System.Windows.Forms.Label lblPasswordBlue;
        private System.Windows.Forms.TextBox txtUserBlue;
        private System.Windows.Forms.Label lblUserBlue;
        private System.Windows.Forms.TextBox txtPortBlue;
        private System.Windows.Forms.Label lblPortBlue;
        private System.Windows.Forms.TextBox txtHostBlue;
        private System.Windows.Forms.Label lblHostBlue;
        private System.Windows.Forms.Button cmdDeployExtensionsAnubis;
    }
}

