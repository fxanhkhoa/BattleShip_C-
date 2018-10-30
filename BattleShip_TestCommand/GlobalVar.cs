using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BattleShip_TestCommand
{
    class GlobalVar
    {
        private static DataProtocol _dP;
        private static SocketClient _sockC;
        private static string _IP;

        public static string IP
        {
            get { return _IP; }
            set { _IP = value; }
        }

        public static SocketClient sockC
        {
            get { return _sockC; }
            set { _sockC = value; }
        }

        public static DataProtocol dP
        {
            get { return _dP; }
            set { _dP = value; }
        }
    }
}
