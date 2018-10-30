using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BattleShip_ClientUI
{
    class GameController
    {
        //Get MainForm
        public FormMain fMain;
        // 0 Nothing
        // 1 Fired but missed (x)
        // 2 Fired and hitted (burn)
        // 3 ship 1 square
        // 4 ship 2 squares
        // 5 ship 3 squares

        public GameController()
        {
            //Get form main
            fMain = (FormMain)Application.OpenForms["FormMain"];
        }

        public void fire(Byte ID, Byte x, Byte y, Byte value)
        {
            fMain.setDataOfShip(ID, x, y, value, 0);
            fMain.Invoke(new Action(() => fMain.fire = 1));
        }

        public void put(Byte ID, Byte x, Byte y, Byte dim, Byte shipType)
        {
            fMain.setDataOfShip(ID, x, y, dim, shipType);
            fMain.Invoke(new Action(() => fMain.startPuting = 1));
        }
    }
}
