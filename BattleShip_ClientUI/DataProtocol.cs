using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BattleShip_ClientUI
{
    class DataProtocol
    {
        // 1 byte ID -- ID = 110 => Client UI
        // 1 byte Command
        // 1 byte X
        // 1 byte Y
        // 10 bytes backup

        const Byte
            Fire = 1,
            PutShip1 = 2,
            PutShip2 = 3,
            PutShip3 = 4;

        public const Byte Vertical = 2, // dọc
                    Horizontal = 1; // ngang

        public const Byte
            Ship1Square = 3,
            Ship2Squares = 4,
            Ship3Squares = 5;

        public Byte OwnID;
        Byte Command;
        Byte X;
        Byte Y;
        Byte Dim;

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
            Console.Write(OwnID.ToString(), Command.ToString(), X.ToString(), Y.ToString(), Dim.ToString());
        }

        public void Send()
        {
            //GlobalVar.sS.SendAllInBuffer(_buffer);
        }

        public String translate()
        {
            String temp = "";
            temp = "ID = " + OwnID + " Command = " + Command + " x,y = " + X + "," + Y
                + " Dim = " + Dim;
            return temp;
        }

        public void setBuffer(Byte[] data)
        {
            _buffer = data;
        }

        public void processData()
        {
            switch (Command)
            {
                case Fire:
                    GlobalVar.Gc.fire(OwnID, X, Y, Dim);
                    break;
                case PutShip1:
                    GlobalVar.Gc.put(OwnID, X, Y, Dim, Ship1Square);
                    break;
                case PutShip2:
                    GlobalVar.Gc.put(OwnID, X, Y, Dim, Ship2Squares);
                    break;
                case PutShip3:
                    GlobalVar.Gc.put(OwnID, X, Y, Dim, Ship3Squares);
                    break;
                default:
                    Console.WriteLine("Command is not in list");
                    break;
            };
        }
    }
}
