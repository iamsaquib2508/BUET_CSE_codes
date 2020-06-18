
package User;

import java.io.*;
import java.net.*;

import User_GUI.Proceedings;
import Utility_Classes.*;

class Up1_Thread implements Runnable
{
    Thread t;
    NCN dloader;
    String filename, directory;
    int part, total;
    
    Up1_Thread(Socket s)
    {
        dloader= new NCN(s);
        t= new Thread(this);
        t.start();
    }

    @Override
    public void run()
    {
        try 
        {
        String msg= (String) dloader.read();
        String[] a= new String[3];
        a= msg.split("\n", 3);
        filename=a[0];
        part = Integer.parseInt(a[1])+1;
        total = Integer.parseInt(a[2]);
        directory = Proceedings.hashuser.get(filename);
        
        String sss = directory+"\\"+filename;
        File f = new File(sss);

        long len = f.length();
        FileInputStream fis ;
        
        fis = new FileInputStream(f);
        BufferedInputStream bis = new BufferedInputStream (fis);
        int max = 10000000;
        long piecesize=len/total;
        if(len%total!=0) piecesize++;
        byte [] sending = null;
        int ch=0;
        bis.skip(piecesize*(part-1));
        
        long left = piecesize;
        while(left>0)
        {
            int kk = left>max? max: (int)left;
            sending = new byte [kk];
            ch = bis.read(sending, 0, kk);
            if(ch<=0) break;
            dloader.write(ch);
            dloader.write(sending);
            left-=ch;
        }
        dloader.write(-1);
        //dloader.closeConnection();
        bis.close();
        fis.close();
        
        } catch (IOException | NumberFormatException ex) {
            ex.printStackTrace();
        }
        
    }
}
