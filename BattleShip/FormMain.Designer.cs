namespace BattleShip
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
            this.flowPanelTeam1 = new System.Windows.Forms.FlowLayoutPanel();
            this.create_BTN = new System.Windows.Forms.Button();
            this.flowPanelTeam2 = new System.Windows.Forms.FlowLayoutPanel();
            this.btn_rotateLeft = new System.Windows.Forms.Button();
            this.ship1 = new System.Windows.Forms.PictureBox();
            this.ship2 = new System.Windows.Forms.PictureBox();
            this.ship3 = new System.Windows.Forms.PictureBox();
            this.Setbtn = new System.Windows.Forms.Button();
            this.label_choose = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.ship1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship3)).BeginInit();
            this.SuspendLayout();
            // 
            // flowPanelTeam1
            // 
            this.flowPanelTeam1.Location = new System.Drawing.Point(12, 12);
            this.flowPanelTeam1.Name = "flowPanelTeam1";
            this.flowPanelTeam1.Size = new System.Drawing.Size(250, 250);
            this.flowPanelTeam1.TabIndex = 0;
            this.flowPanelTeam1.Paint += new System.Windows.Forms.PaintEventHandler(this.flow_pannel_square_Paint);
            // 
            // create_BTN
            // 
            this.create_BTN.Location = new System.Drawing.Point(283, 12);
            this.create_BTN.Name = "create_BTN";
            this.create_BTN.Size = new System.Drawing.Size(75, 23);
            this.create_BTN.TabIndex = 1;
            this.create_BTN.Text = "Create";
            this.create_BTN.UseVisualStyleBackColor = true;
            this.create_BTN.Click += new System.EventHandler(this.create_BTN_Click);
            // 
            // flowPanelTeam2
            // 
            this.flowPanelTeam2.Location = new System.Drawing.Point(12, 268);
            this.flowPanelTeam2.Name = "flowPanelTeam2";
            this.flowPanelTeam2.Size = new System.Drawing.Size(250, 250);
            this.flowPanelTeam2.TabIndex = 1;
            this.flowPanelTeam2.Paint += new System.Windows.Forms.PaintEventHandler(this.flowLayoutPanel1_Paint);
            // 
            // btn_rotateLeft
            // 
            this.btn_rotateLeft.Location = new System.Drawing.Point(283, 41);
            this.btn_rotateLeft.Name = "btn_rotateLeft";
            this.btn_rotateLeft.Size = new System.Drawing.Size(25, 27);
            this.btn_rotateLeft.TabIndex = 2;
            this.btn_rotateLeft.Text = "<";
            this.btn_rotateLeft.UseVisualStyleBackColor = true;
            this.btn_rotateLeft.Click += new System.EventHandler(this.btn_rotateLeft_Click);
            // 
            // ship1
            // 
            this.ship1.Image = ((System.Drawing.Image)(resources.GetObject("ship1.Image")));
            this.ship1.Location = new System.Drawing.Point(283, 86);
            this.ship1.Name = "ship1";
            this.ship1.Size = new System.Drawing.Size(30, 30);
            this.ship1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship1.TabIndex = 3;
            this.ship1.TabStop = false;
            this.ship1.Click += new System.EventHandler(this.ship1_Click);
            // 
            // ship2
            // 
            this.ship2.Image = ((System.Drawing.Image)(resources.GetObject("ship2.Image")));
            this.ship2.Location = new System.Drawing.Point(283, 132);
            this.ship2.Name = "ship2";
            this.ship2.Size = new System.Drawing.Size(60, 30);
            this.ship2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship2.TabIndex = 3;
            this.ship2.TabStop = false;
            this.ship2.Click += new System.EventHandler(this.ship2_Click);
            // 
            // ship3
            // 
            this.ship3.Image = ((System.Drawing.Image)(resources.GetObject("ship3.Image")));
            this.ship3.Location = new System.Drawing.Point(283, 182);
            this.ship3.Name = "ship3";
            this.ship3.Size = new System.Drawing.Size(90, 30);
            this.ship3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ship3.TabIndex = 3;
            this.ship3.TabStop = false;
            this.ship3.Click += new System.EventHandler(this.ship3_Click);
            // 
            // Setbtn
            // 
            this.Setbtn.Location = new System.Drawing.Point(283, 239);
            this.Setbtn.Name = "Setbtn";
            this.Setbtn.Size = new System.Drawing.Size(75, 23);
            this.Setbtn.TabIndex = 4;
            this.Setbtn.Text = "Set";
            this.Setbtn.UseVisualStyleBackColor = true;
            this.Setbtn.Click += new System.EventHandler(this.Setbtn_Click);
            // 
            // label_choose
            // 
            this.label_choose.AutoSize = true;
            this.label_choose.ForeColor = System.Drawing.Color.Green;
            this.label_choose.Location = new System.Drawing.Point(415, 55);
            this.label_choose.Name = "label_choose";
            this.label_choose.Size = new System.Drawing.Size(51, 13);
            this.label_choose.TabIndex = 5;
            this.label_choose.Text = "Choosing";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.ClientSize = new System.Drawing.Size(522, 543);
            this.Controls.Add(this.label_choose);
            this.Controls.Add(this.Setbtn);
            this.Controls.Add(this.ship3);
            this.Controls.Add(this.ship2);
            this.Controls.Add(this.ship1);
            this.Controls.Add(this.btn_rotateLeft);
            this.Controls.Add(this.flowPanelTeam2);
            this.Controls.Add(this.create_BTN);
            this.Controls.Add(this.flowPanelTeam1);
            this.Name = "FormMain";
            this.Text = "ClientUI";
            this.Load += new System.EventHandler(this.FormMain_Load);
            ((System.ComponentModel.ISupportInitialize)(this.ship1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ship3)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel flowPanelTeam1;
        private System.Windows.Forms.Button create_BTN;
        private System.Windows.Forms.FlowLayoutPanel flowPanelTeam2;
        private System.Windows.Forms.Button btn_rotateLeft;
        private System.Windows.Forms.PictureBox ship1;
        private System.Windows.Forms.PictureBox ship2;
        private System.Windows.Forms.PictureBox ship3;
        private System.Windows.Forms.Button Setbtn;
        private System.Windows.Forms.Label label_choose;
    }
}

