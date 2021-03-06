﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BattleShip_ClientUI
{
    // State object for receiving data from remote device.  
    public class StateObject
    {
        // Client socket.  
        public Socket workSocket = null;
        // Size of receive buffer.  
        public const int BufferSize = 1024;
        // Receive buffer.  
        public byte[] buffer = new byte[BufferSize];
        // Received data string.  
        public StringBuilder sb = new StringBuilder();
    }
    class SocketClient
    {
        //Socket client;
        const int bufferSize = 1024;
        byte[] buffer = new byte[bufferSize];
        // The port number for the remote device.  
        private const int port = 8000;
        IPAddress ipAddress;
        IPEndPoint remoteEP;

        // ManualResetEvent instances signal completion.  
        private static ManualResetEvent connectDone =
            new ManualResetEvent(false);
        private static ManualResetEvent sendDone =
            new ManualResetEvent(false);
        private static ManualResetEvent receiveDone =
            new ManualResetEvent(false);

        // The response from the remote device.  
        private static String response = String.Empty;

        public void StartClient()
        {
            // Connect to a remote device.  
            try
            {
                // Establish the remote endpoint for the socket.  
                // The name of the   
                // remote device is "host.contoso.com".  
                //IPHostEntry ipHostInfo = Dns.GetHostEntry("host.contoso.com");
                ipAddress = IPAddress.Parse(GlobalVar.IP);
                remoteEP = new IPEndPoint(ipAddress, port);

                // Create a TCP/IP socket.  
                Socket client = new Socket(ipAddress.AddressFamily,
                    SocketType.Stream, ProtocolType.Tcp);

                // Connect to the remote endpoint.  
                client.BeginConnect(remoteEP,
                    new AsyncCallback(ConnectCallback), client);
                //connectDone.WaitOne();
                Thread.Sleep(250);

                // Send test data to the remote device.  
                //Send(client, "This is a test<EOF>");
                //sendDone.WaitOne();

                // Receive the response from the remote device.  
                Receive(client);
                //receiveDone.WaitOne();

                // Write the response to the console.  
                //Console.WriteLine("Response received : {0}", response);

                // Release the socket.  
                //client.Shutdown(SocketShutdown.Both);
                //client.Close();

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                Socket client = (Socket)ar.AsyncState;

                if (!client.Connected)
                {
                    // Complete the connection.  
                    client.EndConnect(ar);

                    Console.WriteLine("Socket connected to {0}",
                        client.RemoteEndPoint.ToString());

                    // Signal that the connection has been made.  
                    //connectDone.Set();
                }

                // Connect to the remote endpoint.  
                //client.BeginConnect(remoteEP,
                //    new AsyncCallback(ConnectCallback), client);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private void Receive(Socket client)
        {
            try
            {
                // Create the state object.  
                //StateObject state = new StateObject();
                //state.workSocket = client;

                // Begin receiving the data from the remote device.  
                client.BeginReceive(buffer, 0, bufferSize, 0,
                    new AsyncCallback(ReceiveCallback), client);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the state object and the client socket   
                // from the asynchronous state object.  
                //StateObject state = (StateObject)ar.AsyncState;
                //Socket client = state.workSocket;
                Socket client = (Socket)ar.AsyncState;
                // Read data from the remote device.  
                int bytesRead = client.EndReceive(ar);
                //MessageBox.Show(bytesRead.ToString());

                if (bytesRead > 0)
                {
                    // There might be more data, so store the data received so far.  
                    //state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                    //Console.WriteLine(Encoding.ASCII.GetString(buffer, 0, bytesRead));
                    //MessageBox.Show(buffer[0].ToString());
                    //Process Data
                    GlobalVar.dP.setBuffer(buffer);
                    GlobalVar.dP.Seperate();
                    GlobalVar.dP.processData();
                    //if (GlobalVar.dP.OwnID == 69)
                    //{
                    //    GlobalVar.dP.processData();
                    //}

                    // Get the rest of the data.  
                    client.BeginReceive(buffer, 0, bufferSize, 0,
                        new AsyncCallback(ReceiveCallback), client);
                }
                else
                {
                    GlobalVar.dP.setBuffer(buffer);
                    GlobalVar.dP.Seperate();
                    GlobalVar.dP.processData();
                    //MessageBox.Show(buffer[0].ToString());
                    // All the data has arrived; put it in response.  
                    //if (state.sb.Length > 1)
                    //{
                    //    response = state.sb.ToString();
                    //}
                    // Signal that all bytes have been received.  
                    //receiveDone.Set();
                    client.BeginReceive(buffer, 0, bufferSize, 0,
                        new AsyncCallback(ReceiveCallback), client);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private void Send(Socket client, String data)
        {
            // Convert the string data to byte data using ASCII encoding.  
            byte[] byteData = Encoding.ASCII.GetBytes(data);

            // Begin sending the data to the remote device.  
            client.BeginSend(byteData, 0, byteData.Length, 0,
                new AsyncCallback(SendCallback), client);
        }

        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                Socket client = (Socket)ar.AsyncState;

                // Complete sending the data to the remote device.  
                int bytesSent = client.EndSend(ar);
                Console.WriteLine("Sent {0} bytes to server.", bytesSent);

                // Signal that all bytes have been sent.  
                //sendDone.Set();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
