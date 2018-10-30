using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BattleShip_ServerBackEnd
{
    // Client Socket Data
    public class SocketClientData
    {
        public Socket _socket;
        
        //Constructor
        public SocketClientData(Socket s)
        {
            _socket = s;
        }
    }

    class SocketServer
    {
        Socket listener;

        public FormMain fMain;
        public List<SocketClientData> listClient = new List<SocketClientData>();
        // Size of receive buffer.  
        public const int BufferSize = 1024;
        // Receive buffer.  
        public byte[] buffer = new byte[BufferSize];

        public void StartListening()
        {
            //Get form main
            fMain = (FormMain)Application.OpenForms["FormMain"];

            // Establish the local endpoint for the socket.  
            // The DNS name of the computer  
            // running the listener is "host.contoso.com".  
            IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = ipHostInfo.AddressList[0];
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 9998);
            fMain.Invoke(new Action(() => fMain.data = ipAddress.ToString()));

            // Create a TCP/IP socket.
            listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and listen for incoming connections.  
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(100);
                listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);
                fMain.Invoke(new Action(() => fMain.data = "started"));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }

        private void AcceptCallback(IAsyncResult ar)
        {
            Socket handler = listener.EndAccept(ar);
            listClient.Add(new SocketClientData(handler));
            fMain.Invoke(new Action(() => fMain.data = handler.RemoteEndPoint.ToString()));
            handler.BeginReceive(buffer, 0, BufferSize, SocketFlags.None, new AsyncCallback(ReadCallback), handler);
            listener.BeginAccept(new AsyncCallback(AcceptCallback), null);
        }

        private void ReadCallback(IAsyncResult ar)
        {
            Socket handler = (Socket)ar.AsyncState;
            if (handler.Connected)
            {
                int bytesRead;
                try
                {
                    // Read data from the client socket.   
                    bytesRead = handler.EndReceive(ar);
                    //fMain.Invoke(new Action(() => fMain.data = bytesRead.ToString() + handler.RemoteEndPoint.ToString()));
                }
                catch (Exception ex)
                {
                    // Client closed
                    //foreach (SocketClientData scd in listClient)
                    //{
                    //    if (scd._socket.RemoteEndPoint.ToString().Equals(handler.RemoteEndPoint.ToString()))
                    //    {
                    //        listClient.Remove(scd);
                    //        break;
                    //    }
                    //}
                    return;
                }
                if (bytesRead > 0)
                {
                    Byte[] dataBuf = new Byte[BufferSize];
                    Array.Copy(buffer, dataBuf, BufferSize); // backup
                    string text = Encoding.ASCII.GetString(dataBuf);

                    //
                    GlobalVar.dP.buffer = dataBuf; // Separate in this function too
                    fMain.Invoke(new Action(() => fMain.data = text));

                    String reponse = "received";
                    Send(handler, reponse);

                    // Process data received
                    GlobalVar.dP.processData();

                    // Send to Client UI
                    GlobalVar.dP.SetOwnID(69);
                    //GlobalVar.dP.BuildBuffer();
                    GlobalVar.dP.Send();
                }
                else
                {
                    foreach (SocketClientData scd in listClient)
                    {
                        if (scd._socket.RemoteEndPoint.ToString().Equals(handler.RemoteEndPoint.ToString()))
                        {
                            listClient.Remove(scd);
                            break;
                        }
                    }
                }
            }
            handler.BeginReceive(buffer, 0, BufferSize, SocketFlags.None, 
                new AsyncCallback(ReadCallback), handler);
        }

        public void Send(Socket handler, String data)
        {
            // Convert the string data to byte data using ASCII encoding.  
            byte[] byteData = Encoding.ASCII.GetBytes(data);
            //fMain.Invoke(new Action(() => fMain.data = handler.RemoteEndPoint.ToString()));
            // Begin sending the data to the remote device.  
            handler.BeginSend(byteData, 0, byteData.Length, 0,
                new AsyncCallback(SendCallback), handler);

            listener.BeginAccept(new AsyncCallback(AcceptCallback), null);
        }

        public void SendInByte(Socket handler, Byte[] byteData)
        {
            try
            {
                // Begin sending the data to the remote device. 
                handler.BeginSend(byteData, 0, byteData.Length, 0,
                    new AsyncCallback(SendCallback), handler);
            }
            catch (Exception ex)
            {

            }
            

            listener.BeginAccept(new AsyncCallback(AcceptCallback), null);
        }

        private void SendCallback(IAsyncResult ar)
        {
            Socket handler = (Socket)ar.AsyncState;
            int bytesSent = handler.EndSend(ar);
            Console.WriteLine("Sent {0} bytes to client.", bytesSent);
        }

        public void SendAllInBuffer(Byte[] dataBuf)
        {
            foreach (SocketClientData scd in listClient)
            {
                SendInByte(scd._socket, dataBuf);
            }
        }

        public void SendAllInString(String dataBuf)
        {
            foreach (SocketClientData scd in listClient)
            {
                Send(scd._socket, dataBuf);
            }
        }
    }
}
