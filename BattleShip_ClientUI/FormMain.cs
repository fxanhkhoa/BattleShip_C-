using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BattleShip_ClientUI
{
    public partial class FormMain : Form
    {
        Button[,] btn_Arr = new Button[20, 20];
        Button[,] btn_Arr_team2 = new Button[20, 20];
        int row = 8, col = 8;
        const int size = 30;

        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            flowPanelTeam1.Margin = new Padding(0, 0, 0, 0);

            //Add button
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                {
                    //Team1
                    btn_Arr[i, j] = new Button()
                    {
                        Width = size,
                        Height = size,
                        Margin = new Padding(0, 0, 0, 0),
                        BackgroundImageLayout = ImageLayout.Stretch
                    };

                    btn_Arr[i, j].Click += btn_team1_click;
                    flowPanelTeam1.Controls.Add(btn_Arr[i, j]);

                    btn_Arr_team2[i, j] = new Button()
                    {
                        Width = size,
                        Height = size,
                        Margin = new Padding(0, 0, 0, 0),
                        BackgroundImageLayout = ImageLayout.Stretch
                    };

                    btn_Arr_team2[i, j].Click += btn_team2_click;
                    flowPanelTeam2.Controls.Add(btn_Arr_team2[i, j]);
                }
        }

        private void btn_team2_click(object sender, EventArgs e)
        {
            
        }

        private void btn_team1_click(object sender, EventArgs e)
        {
            
        }

        private void btn_Get_Click(object sender, EventArgs e)
        {
            GlobalVar.sockC = new SocketClient();
            GlobalVar.sockC.StartClient();
        }
    }
}
