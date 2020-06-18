
package Tracker;

import java.io.*;
import java.net.*;
import java.util.*;

import Tracker_GUI.SQUser;
import Utility_Classes.*;
import javafx.scene.control.Alert;

import static Tracker_GUI.TrackerLogPage.UserList;


public class Server_Run implements Runnable
{
    public static Hashtable<String, List_of_Users> hashserver_f2c; //<file, Client_ID[]>
    public static int SERVERPORT;
    ServerSocket server;
    Thread t;

    public Server_Run(ServerSocket s){
        server = s;
        hashserver_f2c = new Hashtable<>();
        t = new Thread(this);
        t.start();
    }

    @Override
    public void run() {
        try
        {
            while(true)
            {
                Socket client = server.accept();
                String string = client.getInetAddress().toString();
                String str = string.substring(string.lastIndexOf("/") + 1);
                SQUser squser = new SQUser(str,client.getPort(),"Connected");
                UserList.add(squser);
                new One_to_One_response( new NCN(client) );
            }
        } catch (IOException ex) {

        }
    }
}
