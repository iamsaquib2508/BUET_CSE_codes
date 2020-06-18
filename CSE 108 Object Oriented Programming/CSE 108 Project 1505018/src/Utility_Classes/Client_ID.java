
package Utility_Classes;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;

public class Client_ID implements Serializable
{
    public InetAddress ip;
    public Integer port;

    public Client_ID(InetAddress ips, int ports)
    {
        ip=ips;
        port=ports;
    }
    
    public Client_ID(Socket s)
    {
        ip = s.getInetAddress();
        port = s.getPort();
    }
    @Override
    public String toString()
    {
        return ip.toString() + " ip with " + port + "port.";
    }

    @Override
    public boolean equals(Object O)
    {
        Client_ID c = (Client_ID) O;
        int k=this.ip.toString().compareTo(c.ip.toString());
        if(k==0 && this.port.compareTo(c.port)==0) return true;

        else return false;

    }
}
