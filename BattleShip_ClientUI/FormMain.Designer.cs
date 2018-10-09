namespace BattleShip_ClientUI
{
    partial class FormMain
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
            this.flowPanelTeam2 = new System.Windows.Forms.FlowLayoutPanel();
            this.flowPanelTeam1 = new System.Windows.Forms.FlowLayoutPanel();
            this.btn_Get = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // flowPanelTeam2
            // 
            this.flowPanelTeam2.Location = new System.Drawing.Point(12, 268);
            this.flowPanelTeam2.Name = "flowPanelTeam2";
            this.flowPanelTeam2.Size = new System.Drawing.Size(250, 250);
            this.flowPanelTeam2.TabIndex = 3;
            // 
            // flowPanelTeam1
            // 
            this.flowPanelTeam1.Location = new System.Drawing.Point(12, 12);
            this.flowPanelTeam1.Name = "flowPanelTeam1";
            this.flowPanelTeam1.Size = new System.Drawing.Size(250, 250);
            this.flowPanelTeam1.TabIndex = 2;
            // 
            // btn_Get
            // 
            this.btn_Get.Location = new System.Drawing.Point(288, 12);
            this.btn_Get.Name = "btn_Get";
            this.btn_Get.Size = new System.Drawing.Size(75, 23);
            this.btn_Get.TabIndex = 4;
            this.btn_Get.Text = "Start";
            this.btn_Get.UseVisualStyleBackColor = true;
            this.btn_Get.Click += new System.EventHandler(this.btn_Get_Click);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.ClientSize = new System.Drawing.Size(522, 543);
            this.Controls.Add(this.btn_Get);
            this.Controls.Add(this.flowPanelTeam2);
            this.Controls.Add(this.flowPanelTeam1);
            this.Name = "FormMain";
            this.Text = "FormMain";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel flowPanelTeam2;
        private System.Windows.Forms.FlowLayoutPanel flowPanelTeam1;
        private System.Windows.Forms.Button btn_Get;
    }
}

