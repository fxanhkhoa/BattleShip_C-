using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BattleShip_ServerBackEnd
{
    class GameController
    {
        // 0 Nothing
        // 1 Fired but missed (x)
        // 2 Fired and hitted (burn)
        // 3 ship 1 square
        // 4 ship 2 squares
        // 5 ship 3 squares
        Byte[,] Team1 = new Byte[8, 8];
        Byte[,] Team2 = new Byte[8, 8];

        public GameController()
        {
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                {
                    Team1[i, j] = Team2[i, j] = 0;
                }
        }

        public void fire(Byte ID, Byte x, Byte y)
        {
            if (ID == 1)
            {
                switch (Team1[x, y])
                {
                    case 0:
                        Team1[x, y] = 1;
                        break;
                    case 3:
                    case 4:
                    case 5:
                        Team1[x, y] = 2;
                        break;
                    default:
                        break;
                };
            }
            else
            {
                switch (Team2[x, y])
                {
                    case 0:
                        Team2[x, y] = 1;
                        break;
                    case 3:
                    case 4:
                    case 5:
                        Team2[x, y] = 2;
                        break;
                    default:
                        break;
                };
            }
        }

        public void put(Byte ID, Byte x, Byte y, Byte dim, Byte shipType)
        {
            if (ID == 1)
            {
                switch (shipType)
                {
                    case 3: // 1 square
                        Team1[x, y] = 3;
                        break;
                    case 4: // 2 square
                        if (dim == DataProtocol.Vertical)
                        {
                            Team1[x, y] = 4;
                            Team1[x, y + 1] = 4;
                        }
                        else
                        {
                            Team1[x, y] = 4;
                            Team1[x + 1, y] = 4;
                        }
                        break;
                    case 5:
                        if (dim == DataProtocol.Vertical)
                        {
                            Team1[x, y] = 5;
                            Team1[x, y + 1] = 5;
                            Team1[x, y + 2] = 5;
                        }
                        else
                        {
                            Team1[x, y] = 5;
                            Team1[x + 1, y] = 5;
                            Team1[x + 2, y] = 5;
                        }
                        break;
                };
            }
            else
            {
                switch (shipType)
                {
                    case 3: // 1 square
                        Team2[x, y] = 3;
                        break;
                    case 4: // 2 square
                        if (dim == DataProtocol.Vertical)
                        {
                            Team2[x, y] = 4;
                            Team2[x, y + 1] = 4;
                        }
                        else
                        {
                            Team2[x, y] = 4;
                            Team2[x + 1, y] = 4;
                        }
                        break;
                    case 5:
                        if (dim == DataProtocol.Vertical)
                        {
                            Team2[x, y] = 5;
                            Team2[x, y + 1] = 5;
                            Team2[x, y + 2] = 5;
                        }
                        else
                        {
                            Team2[x, y] = 5;
                            Team2[x + 1, y] = 5;
                            Team2[x + 2, y] = 5;
                        }
                        break;
                };
            }
        }

        private void setSquare(Byte ID, Byte x, Byte y, Byte value)
        {
            if (ID == 1)
            {
                Team1[x, y] = value;
            }
            else
                Team2[x, y] = value;
        }
    }
}
