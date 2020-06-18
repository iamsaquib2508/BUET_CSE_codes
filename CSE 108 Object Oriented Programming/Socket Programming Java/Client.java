package pk;
import java.net.*;
import java.util.Scanner;
import java.util.StringTokenizer;

class sender implements Runnable
{
    DatagramPacket pack;
    DatagramSocket sock;
    Thread t;
    int cport;
    String servername, username;
    InetAddress server;
    sender(String servername,String username,InetAddress server, int cport) {
	this.servername= servername;
        this.username= username;
        this.server= server;
        this.cport= cport;
        t=new Thread(this);
        t.start();
    }
    @Override
    public void run() 
    {
        try{
            String port_info="Via: " + servername+ "\nTo: "+ servername+ "\nFrom: "+ 
                    username+"\nPort: "+cport+"\n";
            send(port_info);
        }catch(Exception x){ //System.out.println(x);
        }
        while(true) 
        {
            try {
                pass_to_send();
            } catch(Exception e) {
                //System.out.println(e);
            }
        }
    }
    void pass_to_send() throws Exception 
    {
        Scanner scan=new Scanner(System.in);
        String temp=scan.nextLine();
        StringTokenizer s= new StringTokenizer(temp,"$");
        String data="Via: " + servername+ "\nTo: ", name=s.nextToken();
        data+=name;
        temp=temp.replace(name+"$ ","");
        data+="\nFrom: "+username+"\nBody: "+temp+"\n";
        send(data);
    }
    void send(String data) throws Exception
    {
        byte udpdata[]=data.getBytes();
        pack=new DatagramPacket(udpdata,udpdata.length);
        pack.setAddress(server);
	pack.setPort(Client.sport);
        sock=new DatagramSocket();
	sock.send(pack);
        sock.close();
    }
}

class receiver implements Runnable 
{
    DatagramPacket pack;
    DatagramSocket sock;
    Thread t;
    int cport;
    String servername, username;
    InetAddress server;
    receiver(String servername,String username,InetAddress server, int cport) {
	this.servername= servername;
        this.username= username;
        this.server= server;
        this.cport= cport;
	t=new Thread(this);
        t.start();
    }
    @Override
    public void run() 
    {
        try{
            sock=new DatagramSocket(cport);
        }catch(SocketException x){
        }
        while(true) 
        {
            try {
                receive();
            } catch(Exception e) {
                System.out.println(e);
            }
        }
    }
    public void receive() throws Exception 
    {
        byte data[]=new byte[3000];
        pack=new DatagramPacket(data,data.length); 
        sock.receive(pack);
        String s=new String(pack.getData());
        StringTokenizer a=new StringTokenizer(s,"\n");
        int i=0;
        String next;
        boolean ok = false;
        while (a.hasMoreTokens()) 
        {
            next=a.nextToken();
            if(i==0)
            {
                if (!next.startsWith("Via: ") || !next.equals("Via: "+servername)) 
                    break;
            }
            else if(i==1)
            {
                if (!next.startsWith("To: ") || !next.equals("To: "+username)) 
                    break;
            }
            else if(i==2)
            {
                next=next.replace("From: ", "");
                if(!next.equals(username))
                {
                    System.out.print(next+" says: ");
                    ok=true;
                }
                
            }
            else if(i==3)
            {
                if(!ok) break;
                next=next.replace("Body: ", "");
                System.out.println(next.trim());
            }
            i++;
        }
    }
}

public class Client 
{
    static Integer cport, sport;
    static String servername, servr, username;
    static InetAddress server;
    public static void main(String args[])
    {
        username=args[0]; cport=Integer.parseInt(args[1]); servr=args[2]; servername=args[3]; 
        sport=Integer.parseInt(args[4]);
        String tofill[]=new String[4];
        StringTokenizer a=new StringTokenizer(servr,".");
        int i=0;
        while(a.hasMoreTokens()) tofill[i++]=a.nextToken();
        byte tosend[]= new byte[4];
        for(i=0;i<4;i++) tosend[i]=(byte)Integer.parseInt(tofill[i]);
        try{
            server=InetAddress.getByAddress(tosend);
        }
        catch(UnknownHostException e){
            System.out.println("Couldn't find Server");
        }
        sender senderthread = new sender(servername,username,server,cport);
        receiver receiverthread = new receiver(servername,username,server,cport);
        try
        {
            senderthread.t.join();
            receiverthread.t.join();
        }catch(InterruptedException e){
            System.out.println("Join Error.");
        }
        System.out.println(username+" quits.");
    }

}