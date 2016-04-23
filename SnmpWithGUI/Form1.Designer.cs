namespace SnmpWithGUI
{
    partial class FormForFirst
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormForFirst));
            this.textBoxResult = new System.Windows.Forms.TextBox();
            this.buttonToStart = new System.Windows.Forms.Button();
            this.comboBoxForSnmpOperation = new System.Windows.Forms.ComboBox();
            this.textBoxForIP = new System.Windows.Forms.TextBox();
            this.lableForIP = new System.Windows.Forms.Label();
            this.labelForOperation = new System.Windows.Forms.Label();
            this.textBoxForOID = new System.Windows.Forms.TextBox();
            this.labelForOID = new System.Windows.Forms.Label();
            this.textBoxForInput = new System.Windows.Forms.TextBox();
            this.labelForInput = new System.Windows.Forms.Label();
            this.groupBoxResult = new System.Windows.Forms.GroupBox();
            this.buttonToNext = new System.Windows.Forms.Button();
            this.textBoxForTrap = new System.Windows.Forms.TextBox();
            this.groupBoxTrap = new System.Windows.Forms.GroupBox();
            this.timerForUpdate = new System.Windows.Forms.Timer(this.components);
            this.groupBoxResult.SuspendLayout();
            this.groupBoxTrap.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBoxResult
            // 
            this.textBoxResult.Location = new System.Drawing.Point(5, 28);
            this.textBoxResult.Multiline = true;
            this.textBoxResult.Name = "textBoxResult";
            this.textBoxResult.ReadOnly = true;
            this.textBoxResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResult.Size = new System.Drawing.Size(362, 175);
            this.textBoxResult.TabIndex = 0;
            // 
            // buttonToStart
            // 
            this.buttonToStart.Location = new System.Drawing.Point(12, 153);
            this.buttonToStart.Name = "buttonToStart";
            this.buttonToStart.Size = new System.Drawing.Size(179, 37);
            this.buttonToStart.TabIndex = 1;
            this.buttonToStart.Text = "Start";
            this.buttonToStart.UseVisualStyleBackColor = true;
            this.buttonToStart.Click += new System.EventHandler(this.buttonToStart_Click);
            // 
            // comboBoxForSnmpOperation
            // 
            this.comboBoxForSnmpOperation.FormattingEnabled = true;
            this.comboBoxForSnmpOperation.Items.AddRange(new object[] {
            "get",
            "set",
            "walk"});
            this.comboBoxForSnmpOperation.Location = new System.Drawing.Point(123, 12);
            this.comboBoxForSnmpOperation.Name = "comboBoxForSnmpOperation";
            this.comboBoxForSnmpOperation.Size = new System.Drawing.Size(259, 23);
            this.comboBoxForSnmpOperation.TabIndex = 2;
            this.comboBoxForSnmpOperation.SelectedIndexChanged += new System.EventHandler(this.comboBoxForSnmpOperation_SelectedIndexChanged);
            // 
            // textBoxForIP
            // 
            this.textBoxForIP.Location = new System.Drawing.Point(123, 41);
            this.textBoxForIP.Name = "textBoxForIP";
            this.textBoxForIP.Size = new System.Drawing.Size(259, 25);
            this.textBoxForIP.TabIndex = 3;
            // 
            // lableForIP
            // 
            this.lableForIP.AutoSize = true;
            this.lableForIP.Location = new System.Drawing.Point(12, 51);
            this.lableForIP.Name = "lableForIP";
            this.lableForIP.Size = new System.Drawing.Size(68, 15);
            this.lableForIP.TabIndex = 4;
            this.lableForIP.Text = "IP地址：";
            // 
            // labelForOperation
            // 
            this.labelForOperation.AutoSize = true;
            this.labelForOperation.Location = new System.Drawing.Point(12, 19);
            this.labelForOperation.Name = "labelForOperation";
            this.labelForOperation.Size = new System.Drawing.Size(52, 15);
            this.labelForOperation.TabIndex = 5;
            this.labelForOperation.Text = "行为：";
            // 
            // textBoxForOID
            // 
            this.textBoxForOID.Location = new System.Drawing.Point(123, 73);
            this.textBoxForOID.Name = "textBoxForOID";
            this.textBoxForOID.Size = new System.Drawing.Size(259, 25);
            this.textBoxForOID.TabIndex = 6;
            // 
            // labelForOID
            // 
            this.labelForOID.AutoSize = true;
            this.labelForOID.Location = new System.Drawing.Point(12, 83);
            this.labelForOID.Name = "labelForOID";
            this.labelForOID.Size = new System.Drawing.Size(46, 15);
            this.labelForOID.TabIndex = 7;
            this.labelForOID.Text = "OID：";
            // 
            // textBoxForInput
            // 
            this.textBoxForInput.Enabled = false;
            this.textBoxForInput.Location = new System.Drawing.Point(123, 105);
            this.textBoxForInput.Name = "textBoxForInput";
            this.textBoxForInput.Size = new System.Drawing.Size(259, 25);
            this.textBoxForInput.TabIndex = 8;
            // 
            // labelForInput
            // 
            this.labelForInput.AutoSize = true;
            this.labelForInput.Location = new System.Drawing.Point(12, 115);
            this.labelForInput.Name = "labelForInput";
            this.labelForInput.Size = new System.Drawing.Size(67, 15);
            this.labelForInput.TabIndex = 9;
            this.labelForInput.Text = "置入值：";
            // 
            // groupBoxResult
            // 
            this.groupBoxResult.Controls.Add(this.textBoxResult);
            this.groupBoxResult.Location = new System.Drawing.Point(12, 207);
            this.groupBoxResult.Name = "groupBoxResult";
            this.groupBoxResult.Size = new System.Drawing.Size(375, 211);
            this.groupBoxResult.TabIndex = 10;
            this.groupBoxResult.TabStop = false;
            this.groupBoxResult.Text = "查询结果";
            // 
            // buttonToNext
            // 
            this.buttonToNext.Location = new System.Drawing.Point(208, 153);
            this.buttonToNext.Name = "buttonToNext";
            this.buttonToNext.Size = new System.Drawing.Size(179, 37);
            this.buttonToNext.TabIndex = 11;
            this.buttonToNext.Text = "Get Next";
            this.buttonToNext.UseVisualStyleBackColor = true;
            this.buttonToNext.Click += new System.EventHandler(this.buttonToNext_Click);
            // 
            // textBoxForTrap
            // 
            this.textBoxForTrap.Location = new System.Drawing.Point(12, 24);
            this.textBoxForTrap.Multiline = true;
            this.textBoxForTrap.Name = "textBoxForTrap";
            this.textBoxForTrap.ReadOnly = true;
            this.textBoxForTrap.Size = new System.Drawing.Size(351, 367);
            this.textBoxForTrap.TabIndex = 12;
            // 
            // groupBoxTrap
            // 
            this.groupBoxTrap.Controls.Add(this.textBoxForTrap);
            this.groupBoxTrap.Location = new System.Drawing.Point(392, 12);
            this.groupBoxTrap.Name = "groupBoxTrap";
            this.groupBoxTrap.Size = new System.Drawing.Size(373, 397);
            this.groupBoxTrap.TabIndex = 13;
            this.groupBoxTrap.TabStop = false;
            this.groupBoxTrap.Text = "Trap信息";
            // 
            // timerForUpdate
            // 
            this.timerForUpdate.Interval = 1000;
            this.timerForUpdate.Tick += new System.EventHandler(this.timerForUpdate_Tick);
            // 
            // FormForFirst
            // 
            this.AcceptButton = this.buttonToStart;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(767, 429);
            this.Controls.Add(this.groupBoxTrap);
            this.Controls.Add(this.buttonToNext);
            this.Controls.Add(this.groupBoxResult);
            this.Controls.Add(this.labelForInput);
            this.Controls.Add(this.textBoxForInput);
            this.Controls.Add(this.labelForOID);
            this.Controls.Add(this.textBoxForOID);
            this.Controls.Add(this.labelForOperation);
            this.Controls.Add(this.lableForIP);
            this.Controls.Add(this.textBoxForIP);
            this.Controls.Add(this.comboBoxForSnmpOperation);
            this.Controls.Add(this.buttonToStart);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FormForFirst";
            this.Text = "SnmpManager";
            this.Load += new System.EventHandler(this.FormForFirst_Load);
            this.groupBoxResult.ResumeLayout(false);
            this.groupBoxResult.PerformLayout();
            this.groupBoxTrap.ResumeLayout(false);
            this.groupBoxTrap.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxResult;
        private System.Windows.Forms.Button buttonToStart;
        private System.Windows.Forms.ComboBox comboBoxForSnmpOperation;
        private System.Windows.Forms.TextBox textBoxForIP;
        private System.Windows.Forms.Label lableForIP;
        private System.Windows.Forms.Label labelForOperation;
        private System.Windows.Forms.TextBox textBoxForOID;
        private System.Windows.Forms.Label labelForOID;
        private System.Windows.Forms.TextBox textBoxForInput;
        private System.Windows.Forms.Label labelForInput;
        private System.Windows.Forms.GroupBox groupBoxResult;
        private System.Windows.Forms.Button buttonToNext;
        private System.Windows.Forms.TextBox textBoxForTrap;
        private System.Windows.Forms.GroupBox groupBoxTrap;
        private System.Windows.Forms.Timer timerForUpdate;

    }
}

