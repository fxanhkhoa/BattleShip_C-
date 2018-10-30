using System;

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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.flowPanelTeam2 = new System.Windows.Forms.FlowLayoutPanel();
            this.flowPanelTeam1 = new System.Windows.Forms.FlowLayoutPanel();
            this.btn_Get = new System.Windows.Forms.Button();
            this.ship3 = new System.Windows.Forms.PictureBox();
            this.ship2 = new System.Windows.Forms.PictureBox();
            this.ship1 = new System.Windows.Forms.PictureBox();
            this.ship6 = new System.Windows.Forms.PictureBox();
            this.ship5 = new System.Windows.Forms.PictureBox();
            this.ship4 = new System.Windows.Forms.PictureBox();
            this.IP = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.ship3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship4)).BeginInit();
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
            this.btn_Get.Text = "Connect";
            this.btn_Get.UseVisualStyleBackColor = true;
            this.btn_Get.Click += new System.EventHandler(this.btn_Get_Click);
            // 
            // ship3
            // 
            this.ship3.BackColor = System.Drawing.Color.Transparent;
            this.ship3.Image = ((System.Drawing.Image)(resources.GetObject("ship3.Image")));
            this.ship3.Location = new System.Drawing.Point(288, 153);
            this.ship3.Name = "ship3";
            this.ship3.Size = new System.Drawing.Size(84, 24);
            this.ship3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship3.TabIndex = 5;
            this.ship3.TabStop = false;
            // 
            // ship2
            // 
            this.ship2.BackColor = System.Drawing.Color.Transparent;
            this.ship2.Image = ((System.Drawing.Image)(resources.GetObject("ship2.Image")));
            this.ship2.Location = new System.Drawing.Point(288, 103);
            this.ship2.Name = "ship2";
            this.ship2.Size = new System.Drawing.Size(54, 24);
            this.ship2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship2.TabIndex = 6;
            this.ship2.TabStop = false;
            // 
            // ship1
            // 
            this.ship1.BackColor = System.Drawing.Color.Transparent;
            this.ship1.Image = ((System.Drawing.Image)(resources.GetObject("ship1.Image")));
            this.ship1.Location = new System.Drawing.Point(288, 58);
            this.ship1.Name = "ship1";
            this.ship1.Size = new System.Drawing.Size(24, 24);
            this.ship1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship1.TabIndex = 7;
            this.ship1.TabStop = false;
            // 
            // ship6
            // 
            this.ship6.Image = ((System.Drawing.Image)(resources.GetObject("ship6.Image")));
            this.ship6.Location = new System.Drawing.Point(288, 394);
            this.ship6.Name = "ship6";
            this.ship6.Size = new System.Drawing.Size(84, 24);
            this.ship6.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship6.TabIndex = 8;
            this.ship6.TabStop = false;
            // 
            // ship5
            // 
            this.ship5.Image = ((System.Drawing.Image)(resources.GetObject("ship5.Image")));
            this.ship5.Location = new System.Drawing.Point(288, 344);
            this.ship5.Name = "ship5";
            this.ship5.Size = new System.Drawing.Size(54, 24);
            this.ship5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship5.TabIndex = 9;
            this.ship5.TabStop = false;
            // 
            // ship4
            // 
            this.ship4.Image = ((System.Drawing.Image)(resources.GetObject("ship4.Image")));
            this.ship4.Location = new System.Drawing.Point(288, 298);
            this.ship4.Name = "ship4";
            this.ship4.Size = new System.Drawing.Size(24, 24);
            this.ship4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship4.TabIndex = 10;
            this.ship4.TabStop = false;
            // 
            // IP
            // 
            this.IP.Location = new System.Drawing.Point(378, 15);
            this.IP.Name = "IP";
            this.IP.Size = new System.Drawing.Size(132, 20);
            this.IP.TabIndex = 11;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.ClientSize = new System.Drawing.Size(522, 543);
            this.Controls.Add(this.IP);
            this.Controls.Add(this.ship6);
            this.Controls.Add(this.ship5);
            this.Controls.Add(this.ship4);
            this.Controls.Add(this.ship3);
            this.Controls.Add(this.ship2);
            this.Controls.Add(this.ship1);
            this.Controls.Add(this.btn_Get);
            this.Controls.Add(this.flowPanelTeam2);
            this.Controls.Add(this.flowPanelTeam1);
            this.Name = "FormMain";
            this.Text = "FormMain";
            this.Load += new System.EventHandler(this.FormMain_Load);
            ((System.ComponentModel.ISupportInitialize)(this.ship3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship4)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel flowPanelTeam2;
        private System.Windows.Forms.FlowLayoutPanel flowPanelTeam1;
        private System.Windows.Forms.Button btn_Get;
        private System.Windows.Forms.PictureBox ship3;
        private System.Windows.Forms.PictureBox ship2;
        private System.Windows.Forms.PictureBox ship1;
        private System.Windows.Forms.PictureBox ship6;
        private System.Windows.Forms.PictureBox ship5;
        private System.Windows.Forms.PictureBox ship4;
        private System.Windows.Forms.TextBox IP;
    }
}

