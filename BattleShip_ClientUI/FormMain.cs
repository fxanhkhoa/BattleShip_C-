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
            GlobalVar.sockC = new SocketClient();
            GlobalVar.dP = new DataProtocol();
            GlobalVar.Gc = new GameController();

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
            //MessageBox.Show(btn_Arr_team2[3, 3].Location.ToString());
        }

        private void btn_team2_click(object sender, EventArgs e)
        {
            Button btn_team2 = sender as Button;
            MessageBox.Show(btn_team2.Location.ToString());
        }

        private void btn_team1_click(object sender, EventArgs e)
        {
            //ship1.Location = new Point(btn_Arr[0, 0].Location.X + size / 2, btn_Arr[0, 0].Location.Y + size / 4);
        }

        private void btn_Get_Click(object sender, EventArgs e)
        {
            //GlobalVar.sockC = new SocketClient();
            GlobalVar.IP = IP.Text;
            GlobalVar.sockC.StartClient();
            //ship1.Location = new Point(btn_Arr[1, 1].Location.X + size / 2, btn_Arr[1, 1].Location.Y + size / 4);
        }



        ////////////////////////////// Variables for set from another form //////////////////

        public Byte _ID, _x, _y, _dim, _typeShip, _value;
        private Byte _startPuting;
        private Byte _fire; // x, y

        public Byte fire
        {
            get { return _fire; }
            set
            {
                _fire = value;
                Fire();
            }
        }

        public Byte startPuting
        {
            get { return _startPuting; }
            set
            {
                _startPuting = value;
                //MessageBox.Show(_ID.ToString());
                switch (_typeShip)
                {
                    case 3:
                        setLocationShip1(_ID, _x, _y, _dim);
                        break;
                    case 4:
                        setLocationShip2(_ID, _x, _y, _dim);
                        break;
                    case 5:
                        setLocationShip3(_ID, _x, _y, _dim);
                        break;
                };
            }
        }

        ////////////////////////////// None Action Functions ////////////////////////////////

        
        
        public void setDataOfShip(Byte ID, Byte x, Byte y, Byte dim, Byte typeShip)
        {
            _ID = ID;
            _x = x;
            _y = y;
            _dim = dim;
            _typeShip = typeShip;

            //
            _value = _dim;
        }

        ////////////////////////////// Action Function with UI /////////////////////////////
        private void Fire()
        {
            if (_ID == 1)
            {
                if (_value == 1) // Put x
                {
                    btn_Arr[_x, _y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\X.png");
                    //btn_Arr[_x, _y].BringToFront();
                }
                else if (_value == 2) // Put Fire
                {
                    btn_Arr[_x, _y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\fire.png");
                    //btn_Arr[_x, _y].BringToFront();
                }
            }
            else
            {
                if (_value == 1) // Put x
                {
                    btn_Arr_team2[_x, _y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\X.png");
                    //btn_Arr[_x, _y].BringToFront();
                }
                else if (_value == 2) // Put Fire
                {
                    btn_Arr_team2[_x, _y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\fire.png");
                    //btn_Arr[_x, _y].BringToFront();
                }
            }
        }

        private void setLocationShip1(Byte ID, Byte x, Byte y, Byte dim)
        {
            Console.WriteLine("Go to location ship 1");
            if (ID == 1)
            {
                //Console.WriteLine("Go in Here");
                //ship1.Location = new Point(btn_Arr[x, y].Location.X + size / 2, btn_Arr[x, y].Location.Y + size / 2);
                btn_Arr[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\1squareship.png");
            }
            else
            {
                //ship4.Location = new Point(btn_Arr_team2[x, y].Location.X + size / 2, btn_Arr_team2[x, y].Location.Y + size / 2 + 255);
                btn_Arr_team2[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\1squareship.png");
            }
            Console.WriteLine("Set Ship 1 OK");
        }

        public void setLocationShip2(Byte ID, Byte x, Byte y, Byte dim)
        {
            if (ID == 1)
            {
                //if (dim == DataProtocol.Vertical) // vertical
                //rotateShip2();
                //ship2.Location = new Point(btn_Arr[x, y].Location.X + size / 2, btn_Arr[x, y].Location.Y + size / 2);
                if (dim == DataProtocol.Vertical)
                {
                    btn_Arr[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                    btn_Arr[x + 1, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                }
                else
                {
                    btn_Arr[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                    btn_Arr[x, y + 1].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                }
            }
            else
            {
                //if (dim == DataProtocol.Vertical)
                //rotateShip5();
                //ship5.Location = new Point(btn_Arr_team2[x, y].Location.X + size / 2, btn_Arr_team2[x, y].Location.Y + size / 2 + 255);
                //MessageBox.Show(btn_Arr_team2[x, y].Location.X.ToString());
                if (dim == DataProtocol.Vertical)
                {
                    btn_Arr_team2[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                    btn_Arr_team2[x + 1, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                }
                else
                {
                    btn_Arr_team2[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                    btn_Arr_team2[x, y + 1].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\2squareship.png");
                }
            }
        }

        public void setLocationShip3(Byte ID, Byte x, Byte y, Byte dim)
        {
            if (ID == 1)
            {
                if (dim == DataProtocol.Vertical) // vertical
                {
                    btn_Arr[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr[x + 1, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr[x + 2, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                }
                else
                {
                    btn_Arr[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr[x, y + 1].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr[x, y + 2].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                }
                //ship3.Location = new Point(btn_Arr[x, y].Location.X + size / 2, btn_Arr[x, y].Location.Y + size / 2);
            }
            else
            {
                if (dim == DataProtocol.Vertical)
                {
                    btn_Arr_team2[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr_team2[x + 1, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr_team2[x + 2, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                }
                else
                {
                    btn_Arr_team2[x, y].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr_team2[x, y + 1].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                    btn_Arr_team2[x, y + 2].BackgroundImage = Image.FromFile(Application.StartupPath + "\\pictures\\3squareship.png");
                }
                    //rotateShip6();
                //ship6.Location = new Point(btn_Arr_team2[x, y].Location.X + size / 2, btn_Arr_team2[x, y].Location.Y + size / 2 + 255);
            }
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

        private void rotateShip5()
        {
            // Rotate picturebox
            int temp = ship5.Width;
            ship5.Width = ship5.Height;
            ship5.Height = temp;

            Image rotatePic = ship5.Image;
            rotatePic.RotateFlip(RotateFlipType.Rotate90FlipNone);
        }

        private void rotateShip6()
        {
            // Rotate picturebox
            int temp = ship6.Width;
            ship6.Width = ship6.Height;
            ship6.Height = temp;

            Image rotatePic = ship6.Image;
            rotatePic.RotateFlip(RotateFlipType.Rotate90FlipNone);
        }
    }
}
