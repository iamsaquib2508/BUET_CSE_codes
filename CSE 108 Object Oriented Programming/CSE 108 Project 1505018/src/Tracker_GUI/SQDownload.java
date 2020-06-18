package Tracker_GUI;


public class SQDownload {
    private String filename;
    private String ip;
    private int port;

    public SQDownload(String string, String str, int p){
        this.filename = string;
        this.ip = str;
        this.port = p;
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
