package pk;
import java.net.*;
import java.util.*;
import java.util.StringTokenizer;

class active implements Runnable 
{
    DatagramPacket pack;
    DatagramSocket sock;
    Hashtable<String, clientinfo> table;
    String servername;
    int PORT;
    Thread t;
    active(String servername, int p) {
        this.servername=servername;
        PORT=p;
        table = new Hashtable<>();
	t=new Thread(this);
        t.start();
    }
    @Override
    public void run() 
    {
        try{
            sock=new DatagramSocket(PORT);
        }catch(SocketException x){
        }
        while(true) 
        {
            try {
                receive_and_send();
            } catch(Exception e) {
                System.out.println(e);
            }
        }
    }
    public void receive_and_send() throws Exception 
    {
        byte data[]=new byte[3000];
        pack=new DatagramPacket(data,data.length); 
        sock.receive(pack);
        String s=new String(pack.getData());
        s=s.trim();
        StringTokenizer a=new StringTokenizer(s,"\n");
        int i=0, ccase=0;
        String next, posclient=new String(""), tbs="";
        clientinfo client = null;
        while (a.hasMoreTokens()) 
        {
            next=a.nextToken();
            if(i==0)
            {
                if (!next.startsWith("Via: ")) break;
                else if( !next.equals("Via: "+servername)) 
                {
                    System.out.println("Warning: Server name mismatch. Message dropped.");
                    break;
                }
                tbs=tbs+next+"\n";
            }
            else if(i==1)
            {
                if (!next.startsWith("To: ")) break;
                else if( next.equals("To: "+servername)) ccase=1;
                else 
                {
                    tbs=tbs+next+"\n";
                    next=next.replace("To: ", "");
                    if(table.containsKey(next))
                    {
                        s=(next);
                        client=table.get(next);
                    }
                    else
                    {
                        System.out.println("Warning: Unknown recipient. Message dropped.");
                        break;
                    }
                    ccase=2;
                }
            }
            else if(i==2)
            {
                posclient=next.replace("From: ", "");
                if(ccase==1){
                
                if(posclient.isEmpty()) break;
                }
                else
                {
                    if(table.containsKey(posclient))
                    {
                        if(!posclient.equals(s))
                        System.out.println(posclient+" --> "+s);
                    }
                    //else break; //this will stop getting message from intruder
                    tbs=tbs+next+"\n";
                    
                }
            }
            else if(i==3)
            {
                if(ccase==1)
                {
                next=next.replace("Port: ", "");
                if(next.isEmpty()) break;
                next=next.trim();
                System.out.println(posclient+" logged in with "+ pack.getAddress() +" and port "+  next);
                table.put(posclient,new clientinfo(pack.getAddress(),Integer.parseInt(next)));
                    
                }
                else if(ccase==2)
                {
                    tbs=tbs+next.replace("Body:  ", "Body: ");
                    //System.out.println(tbs);
                    byte udpdata[]=tbs.getBytes();
                    pack=new DatagramPacket(udpdata,udpdata.length);
                    pack.setAddress(client.ip);
                    pack.setPort(client.port);
                    sock.send(pack);
                }
            }
            i++;
        }
    }
}

class clientinfo
{
    InetAddress ip;
    int port;
    clientinfo(InetAddress a, int pp)
    {
        ip=a; port=pp;
    }
}


public class Server 
{
    static int port;
    static String servername; 
    public static void main(String args[])
    {
        //System.out.println(InetAddress.getLocalHost());
        servername=args[0];
        if(args.length==2) 
        port= Integer.parseInt(args[1]);
        
        
        active activethread = new active(servername, port);
        try{
            activethread.t.join();
        }catch(InterruptedException x){
            System.out.println("Join Error.");
        }
        System.out.println("Server Quits.");
    }
}
