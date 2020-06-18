package Tracker_GUI;


public class SQSeed {
    private String filename;
    private String ip;
    private int port;
    private String status;

    public SQSeed(String string, String str, int p, String status){
        this.filename = string;
        this.ip = str;
        this.port = p;
        this.status = status;

    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getFilename() {
        return filename;
    }

    public void setFilename(String filename) {
        this.filename = filename;
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
}
