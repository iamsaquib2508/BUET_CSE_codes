
package Utility_Classes;

import java.io.*;
import java.net.*;

public class NCN
{

    public Socket socket;
    private ObjectOutputStream oos;
    private ObjectInputStream ois;

    public NCN(String s, int port)
    {
        try 
        {
            this.socket = new Socket(s, port);
            oos = new ObjectOutputStream(socket.getOutputStream());
            ois = new ObjectInputStream(socket.getInputStream());
        } catch (IOException e) {
            System.out.println("In NCN : " + e.toString());
        }
    }

    public NCN(Socket s)
    {
        try 
        {
            this.socket = s;
            oos = new ObjectOutputStream(socket.getOutputStream());
            ois = new ObjectInputStream(socket.getInputStream());
        } catch (IOException e) {

        }
    }

    public Object read() 
    {
        Object o = null;
        try 
        {
            o = ois.readObject();
        } catch (IOException | ClassNotFoundException e) {
        }
        return o;
    }

    public void write(Object o) 
    {
        try 
        {
            oos.writeObject(o);
        } catch (IOException e) {
        }
    }

}
