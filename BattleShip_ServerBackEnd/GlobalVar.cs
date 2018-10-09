using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BattleShip_ServerBackEnd
{
    class GlobalVar
    {
        private static SocketServer _sS;
        private static DataProtocol _dP;
        private static GameController _gC;

        public static SocketServer sS
        {
            get { return _sS; }
            set { _sS = value; }
        }

        public static DataProtocol dP
        {
            get { return _dP; }
            set { _dP = value; }
        }

        public static GameController gC
        {
            get { return _gC; }
            set { _gC = value; }
        }
    }
}
