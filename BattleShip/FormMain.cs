using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BattleShip
{
    public partial class FormMain : Form
    {
        // Array contains all button
        Button[,] btn_Arr = new Button[20,20];
        Button[,] btn_Arr_team2 = new Button[20, 20];
        int row = 8, col = 8;
        const int size = 30;

        int[] ship_stt = new int[4];

        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            flowPanelTeam1.Margin = new Padding(0, 0, 0, 0);
            label_choose.Visible = false;
        }

        private void flow_pannel_square_Paint(object sender, PaintEventArgs e)
        {

        }

        private void flowLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void create_BTN_Click(object sender, EventArgs e)
        {
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
                    flowPanelTeam1.Controls.Add(btn_Arr[i,j]);

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

        private void Setbtn_Click(object sender, EventArgs e)
        {
            ship1.Location = new Point(btn_Arr[0, 0].Location.X + size / 2, btn_Arr[0, 0].Location.Y + size / 4);
        }

        private void ship1_Click(object sender, EventArgs e)
        {
            setShipStatus(1, 1);
            label_choose.Visible = true;
            label_choose.Top = ship1.Top + 10;
        }

        private void ship2_Click(object sender, EventArgs e)
        {
            setShipStatus(2, 1);
            label_choose.Visible = true;
            label_choose.Top = ship2.Top + 10;
        }

        private void ship3_Click(object sender, EventArgs e)
        {
            setShipStatus(3, 1);
            label_choose.Visible = true;
            label_choose.Top = ship3.Top + 10;
        }


        private void btn_rotateLeft_Click(object sender, EventArgs e)
        {
            int selectedShip = getShipIsClicked();

            if ((selectedShip == 0) || (selectedShip == 1)) return;

            switch (selectedShip)
            {
                case 2:
                    rotateShip2();
                    break;
                case 3:
                    rotateShip3();
                    break;
                default:
                    break;
            };
        }

        private void btn_team2_click(object sender, EventArgs e)
        {
            Button btn_team2 = sender as Button;

            btn_team2.BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\fire.png");
            //throw new NotImplementedException();
        }

        private void btn_team1_click(object sender, EventArgs e)
        {
            Button btn_team1 = sender as Button; // get which button is click

            int shipIndex = getShipIsClicked();
            if (shipIndex == 0) return;
            switch (shipIndex)
            {
                case 1:
                    setLocation_1squareShip(btn_team1.Location.X, btn_team1.Location.Y);
                    setShipStatus(shipIndex, 0);
                    break;
                default:
                    break;
            };

            //btn_team1.BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\fire.png");
            //throw new NotImplementedException();
        }









        ///////////////////////////// User Function
        private int getShipIsClicked()
        {
            for (int i = 0; i < 4; i++)
            {
                if (ship_stt[i] == 1)
                    return i;
            }
            
            return 0;
        }

        private void setShipStatus(int index, int value)
        {
            ship_stt[index] = value;
        }

        

        private void setLocation_1squareShip(int x, int y)
        {
            ship1.Location = new Point(x + size / 2, y + size / 4);
        }

        private void rotateShip2()
        {
            // Rotate picturebox
            int temp = ship2.Width;
            ship2.Width = ship2.Height;
            ship2.Height = temp;

            Image rotatePic = ship2.Image;
            rotatePic.RotateFlip(RotateFlipType.Rotate90FlipNone);
        }

        private void rotateShip3()
        {
            // Rotate picturebox
            int temp = ship3.Width;
            ship3.Width = ship3.Height;
            ship3.Height = temp;

            Image rotatePic = ship3.Image;
            rotatePic.RotateFlip(RotateFlipType.Rotate90FlipNone);
        }
    }
}
