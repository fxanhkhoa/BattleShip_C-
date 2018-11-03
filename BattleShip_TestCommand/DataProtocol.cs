using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BattleShip_TestCommand
{
    class DataProtocol
    {
        // 1 byte ID -- ID = 110 => Client UI
        // 1 byte Command
        // 1 byte X
        // 1 byte Y
        // 1 byte dim
        // 10 bytes backup

        const Byte
            Fire = 1,
            PutShip1 = 2,
            PutShip2 = 3,
            PutShip3 = 4;

        public const Byte Vertical = 1, // dọc
                    Horizontal = 2; // ngang

        public Byte OwnID;
        public Byte Command;
        public Byte X;
        public Byte Y;
        public Byte Dim;
        public String Username;
        public String Passwd;

        private Byte[] _buffer = new Byte[1024];

        public Byte[] buffer
        {
            get { return _buffer; }
            set { _buffer = value; Seperate(); }
        }

        public Byte[] BuildBuffer()
        {
            _buffer[0] = OwnID;
            _buffer[1] = Command;
            _buffer[2] = X;
            _buffer[3] = Y;
            _buffer[4] = Dim;
            return _buffer;
        }

        public void Seperate()
        {
            OwnID = _buffer[0];
            Command = _buffer[1];
            X = _buffer[2];
            Y = _buffer[3];
            Dim = _buffer[4];
        }

        public void Send()
        {
            GlobalVar.sockC._Send(_buffer);
        }

        public String translate()
        {
            String temp = "";
            temp = "ID = " + OwnID + " Command = " + Command + " x,y = " + X + "," + Y
                + " Dim = " + Dim;
            return temp;
        }

        public void SignIn()
        {
            _buffer[0] = OwnID;
            _buffer[1] = Command;

            _buffer[2] = Convert.ToByte(Username[0]);
            _buffer[3] = Convert.ToByte(Username[1]);
            _buffer[4] = Convert.ToByte(Username[2]);
            _buffer[5] = Convert.ToByte(Username[3]);
            _buffer[6] = Convert.ToByte(Username[4]);
            _buffer[7] = Convert.ToByte(Username[5]);

            _buffer[8] = Convert.ToByte(Passwd[0]);
            _buffer[9] = Convert.ToByte(Passwd[1]);
            _buffer[10] = Convert.ToByte(Passwd[2]);
            _buffer[11] = Convert.ToByte(Passwd[3]);
            _buffer[12] = Convert.ToByte(Passwd[4]);
            _buffer[13] = Convert.ToByte(Passwd[5]);

            Send();
        }

        public void SignUp()
        {
            _buffer[0] = OwnID;
            _buffer[1] = Command;

            _buffer[2] = Convert.ToByte(Username[0]);
            _buffer[3] = Convert.ToByte(Username[1]);
            _buffer[4] = Convert.ToByte(Username[2]);
            _buffer[5] = Convert.ToByte(Username[3]);
            _buffer[6] = Convert.ToByte(Username[4]);
            _buffer[7] = Convert.ToByte(Username[5]);

            _buffer[8] = Convert.ToByte(Passwd[0]);
            _buffer[9] = Convert.ToByte(Passwd[1]);
            _buffer[10] = Convert.ToByte(Passwd[2]);
            _buffer[11] = Convert.ToByte(Passwd[3]);
            _buffer[12] = Convert.ToByte(Passwd[4]);
            _buffer[13] = Convert.ToByte(Passwd[5]);

            Send();
        }
    }
}
