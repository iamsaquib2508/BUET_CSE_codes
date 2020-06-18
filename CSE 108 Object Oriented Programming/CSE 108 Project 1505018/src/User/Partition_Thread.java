
package User;

import java.io.*;
import java.net.*;
import Utility_Classes.*;


class Partition_Thread implements Runnable
{
    int part, total;
    String file;
    Client_ID clinfo;
    Thread t;
    FileOutputStream fos;
    long sofar;

    Partition_Thread(int p, int tot, String filename, Client_ID h, FileOutputStream f)
    {
        part = p;
        file = filename;
        clinfo = h;
        total = tot;
        fos=f;
        sofar=0;
        t = new Thread(this);
        t.start();
    }
    @Override
    public void run() 
    {
        try {
            Socket s = new Socket(clinfo.ip,clinfo.port);
            NCN seeder = new NCN(s);
            seeder.write(file + "\n" + part + "\n" + total );
            byte [] filebyte = null;
            BufferedOutputStream bos = new BufferedOutputStream(fos);
            while (true)
            {
                int piecesize = (Integer) seeder.read();
                if(piecesize==-1) break;
                filebyte = new byte[piecesize];
                filebyte =(byte []) seeder.read();
                bos.write(filebyte);
                sofar+=piecesize;
            }
            bos.flush();
            bos.close();
            fos.close();
            
        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }
    
}

/*class sofar
{
    long val;
    sofar(long k)
    {val=k;}
    synchronized public void setsofar(long k)
    {

    }
}*/
