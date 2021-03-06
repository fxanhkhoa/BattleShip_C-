﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BattleShip_TestCommand
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GlobalVar.dP = new DataProtocol();
            GlobalVar.sockC = new SocketClient();
        }

        private void btn_Send_Click(object sender, EventArgs e)
        {
            GlobalVar.dP.OwnID = Convert.ToByte(ID.Text);
            GlobalVar.dP.Command = Convert.ToByte(Command.Text);
            GlobalVar.dP.X = Convert.ToByte(X.Text);
            GlobalVar.dP.Y = Convert.ToByte(Y.Text);
            GlobalVar.dP.Dim = Convert.ToByte(Dim.Text);
            GlobalVar.dP.BuildBuffer();
            GlobalVar.dP.Send();
        }

        private void btn_connect_Click(object sender, EventArgs e)
        {
            GlobalVar.IP = IP.Text;
            GlobalVar.sockC.StartClient();
        }

        private void SignIn_Click(object sender, EventArgs e)
        {
            GlobalVar.dP.OwnID = Convert.ToByte(ID.Text);
            GlobalVar.dP.Command = 6;
            GlobalVar.dP.Username = username.Text;
            GlobalVar.dP.Passwd = passwd.Text;
            if ((username.Text.Length != 6) || (passwd.Text.Length !=6))
            {
                MessageBox.Show("Username and password must be 6 in length");
                return;
            }
            GlobalVar.dP.SignIn();
        }

        private void SignUp_Click(object sender, EventArgs e)
        {
            GlobalVar.dP.OwnID = Convert.ToByte(ID.Text);
            GlobalVar.dP.Command = 5;
            GlobalVar.dP.Username = username.Text;
            GlobalVar.dP.Passwd = passwd.Text;
            if ((username.Text.Length != 6) || (passwd.Text.Length != 6))
            {
                MessageBox.Show("Username and password must be 6 in length");
                return;
            }
            GlobalVar.dP.SignUp();
        }
    }
}
