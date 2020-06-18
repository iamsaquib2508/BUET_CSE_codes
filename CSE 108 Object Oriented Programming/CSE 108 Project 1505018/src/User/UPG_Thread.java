package User;

import User_GUI.Proceedings;
import javafx.scene.control.Alert;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class UPG_Thread implements Runnable
{
    ServerSocket up;
    Thread t;

    public UPG_Thread(ServerSocket s)
    {
        up = s;
        t = new Thread(this);
        t.start();
    }

    @Override
    public void run()
    {
        while(true)
        {
            try
            {
                Socket receiver = up.accept();
                new Up1_Thread(receiver);
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }

}


