using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BattleShip_ServerBackEnd
{
    public partial class FormMain : Form
    {
        private string _data;

        public string data
        {
            get { return _data; }
            set
            {
                _data = value;
                Message.AppendText(value + "\n");
                Message.AppendText(GlobalVar.dP.translate() + "\n");
                Message.AppendText("------------ \n");
            }
        }
        public FormMain()
        {
            InitializeComponent();
        }

        private void btn_Start_Click(object sender, EventArgs e)
        {
            GlobalVar.dP = new DataProtocol();
            GlobalVar.gC = new GameController();
            GlobalVar.sS = new SocketServer();
            GlobalVar.sS.StartListening();
        }

        private void btn_Reset_Click(object sender, EventArgs e)
        {
                GlobalVar.sS.SendAllInString("a b c d e");   
        }
    }
}
