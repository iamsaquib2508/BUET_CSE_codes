package User;

import java.io.*;

import javafx.scene.control.Alert;
import javafx.scene.control.ProgressBar;
import User_GUI.Proceedings;
import Utility_Classes.*;


public class DL1_Thread implements Runnable
{
    String filename;
    NCN server;
    List_of_Users users;
    Thread t;
    File directory;
    ProgressBar progressBar;

    public DL1_Thread(String string, File destination, ProgressBar progressBar)
    {
        this.server = Proceedings.SERVER;
        this.directory = destination;
        this.filename = string;
        this.progressBar = progressBar;
        t = new Thread(this, "dloading " + filename);
        t.start();
    }

    @Override
    public void run()
    {
        try 
        {
            int maxx = 10000000;
            users = (List_of_Users) server.read();
            int sz = users.v.size();
            Partition_Thread pt[] = new Partition_Thread[sz];

            File tempf [] = new File[sz];
            FileOutputStream tfos = null;
            long filesize = users.SZ;
            for(int i = 0; i < sz ; i++)
            {
                tempf[i] = new File(directory.toString()+"\\tmp"+i+filename);
                tfos = new FileOutputStream(tempf[i]);
                Client_ID a = users.v.elementAt(i);
                pt[i] = new Partition_Thread(i, sz, filename, a, tfos);
            }
            for(int i=0;i<sz;i++)
            {
                pt[i].t.join();
            }

            String dest = directory.toString()+"\\"+filename;
            File destination = new File(dest);
            FileOutputStream fos = new FileOutputStream(destination);
            byte newf[]= null;
            
            FileInputStream fis = null;
            BufferedOutputStream bos = new BufferedOutputStream(fos);

            long taken=0, pp=0;
            for(int i=0;i<sz;i++) 
            {
                long rem = tempf[i].length();
                fis = new FileInputStream (tempf[i]);
                BufferedInputStream bis = new BufferedInputStream (fis);
                while(rem>0)
                {
                    int kk = rem>maxx? maxx: (int)rem;
                    newf = new byte [kk];
                    int ch = bis.read(newf, 0, kk);
                    pp+=ch;
                    bos.write(newf,0,ch);
                    rem-=ch;
                    taken+=ch;
                    progressBar.setProgress(1.0*pp/filesize);
                    if(taken>59245350)
                    {
                        bos.flush();
                        taken=0;
                    }
                }

                bis.close();
                fis.close();
                tempf[i].delete();

            }
            bos.flush();
            bos.close();
            fos.close();
            destination.createNewFile();

        } catch (InterruptedException | IOException ex) {
            ex.printStackTrace();
        } 
    }
}
