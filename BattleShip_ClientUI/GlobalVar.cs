using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BattleShip_ClientUI
{
    class GlobalVar
    {
        private static SocketClient _sockC;

        public static SocketClient sockC
        {
            get { return _sockC; }
            set { _sockC = value; } 
        }
    }
}
