package Tracker;

import Tracker_GUI.SQDownload;
import Tracker_GUI.SQSeed;
import Tracker_GUI.SQUser;
import Utility_Classes.Client_ID;
import Utility_Classes.List_of_Users;
import Utility_Classes.NCN;

import java.net.Socket;
import java.util.Iterator;
import java.util.Vector;

import static Tracker_GUI.TrackerLogPage.DownloadList;
import static Tracker_GUI.TrackerLogPage.SeedList;
import static Tracker_GUI.TrackerLogPage.UserList;

public class One_to_One_response implements Runnable {
    Thread t;
    NCN user;
    Client_ID CC;
    Socket s;
    String IP;
    int PORT;
    Vector<String> var;
    public One_to_One_response(NCN a) {
        user = a;
        s = user.socket;

        String str = s.getInetAddress().toString();
        IP = str.substring(str.lastIndexOf("/") + 1);
        PORT = s.getPort();
        var = new Vector<>();

        t = new Thread(this);
        t.start();
    }

    @Override
    public void run()
    {
        while (true)
        {

            int k = 0;
            String msg = (String) user.read(), filename;
            if (msg == null || msg.equals("EXIT")) break;
            if (msg.startsWith("Get "))
            {
                filename = msg.replace("Get ", "");

                List_of_Users arr = Server_Run.hashserver_f2c.get(filename);
                user.write(arr);

                SQDownload sqdownload = new SQDownload(filename, IP , PORT);

                DownloadList.add(sqdownload);
            }
            else if (msg.startsWith("Seed "))
            {

                String ss[] = msg.split("\n");
                k = Integer.parseInt(ss[1].trim());
                filename = ss[0].replace("Seed ", "");
                long sz = Long.parseLong(ss[2]);
                CC = new Client_ID(s.getInetAddress(), k);

                if (Server_Run.hashserver_f2c.containsKey(filename)) {
                    List_of_Users ls = Server_Run.hashserver_f2c.get(filename);
                    ls.v.add(CC);
                }
                else
                {
                    List_of_Users a = new List_of_Users();
                    a.v.add(new Client_ID(s.getInetAddress(), k));
                    Server_Run.hashserver_f2c.put(filename, a);
                    if (a.SZ == -1) a.SZ = sz;
                }
                var.add(filename);

                SQSeed sqseed = new SQSeed(filename, IP , PORT, "Seeded");
                SeedList.add(sqseed);

            }
            else if (msg.equals("Show"))
            {
                Iterator it = Server_Run.hashserver_f2c.keySet().iterator();
                int kk = Server_Run.hashserver_f2c.keySet().size();
                String[] sar = new String[kk];
                int i = 0;
                while (it.hasNext()) {
                    sar[i] = (String) it.next();
                    long sizeoffile = Server_Run.hashserver_f2c.get(sar[i]).SZ;
                    int seed = Server_Run.hashserver_f2c.get(sar[i]).v.size();
                    sar[i] += "\n" + sizeoffile + "\n" + seed;
                    i++;
                }
                user.write(sar);
            }

        }

        if (CC!=null && var.size()>0)
        {
            for (int kk = 0; kk < var.size(); kk++)
            {
                String file = var.elementAt(kk);
                Server_Run.hashserver_f2c.get(file).v.remove(CC);

                SQSeed sqseed = new SQSeed(file, IP , PORT, "Removed");
                SeedList.add(sqseed);
            }
        }
        var.clear();

        SQUser squser = new SQUser(IP, PORT , "Disconnected");
        UserList.add(squser);
    }
}