package Tracker_GUI;


public class SQUser {
    String ip;
    int port;
    String status;

    public SQUser(String ip, int port, String status){
        this.ip = ip;
        this.port = port;
        this.status = status;
    }

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}
