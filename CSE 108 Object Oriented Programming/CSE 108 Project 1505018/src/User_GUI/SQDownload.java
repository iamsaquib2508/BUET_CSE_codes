package User_GUI;

import javafx.scene.control.ProgressBar;

import java.io.File;
import java.text.DecimalFormat;


public class SQDownload {
    private String filename;
    private long filesize;
    private ProgressBar progressBar;
    private String destination;

    public SQDownload(String filename, long size, ProgressBar progressBar, File destination){
        this.filename = filename;
        this.filesize = size;
        this.progressBar = progressBar;
        this.progressBar.setMinWidth(170);
        this.progressBar.setMaxWidth(190);
        this.progressBar.setPrefWidth(130);
        this.progressBar.setVisible(true);
        this.destination = destination.toPath().toString();
    }

    public String getDestination() {
        return destination;
    }

    public void setDestination(String destination) {
        this.destination = destination;
    }

    public String getFilename() {
        return filename;
    }

    public void setFilename(String filename) {
        this.filename = filename;
    }

    public ProgressBar getProgressBar() {
        return progressBar;
    }

    public void setProgressBar(double progress) {
        this.progressBar.setProgress(1.0 * progress); // change kora hoiche
    }

    public String getFilesize(){
        String temp ;
        DecimalFormat df = new DecimalFormat("#.##");
        if(filesize < 1024){
            temp = filesize + " B";
        }
        else if(filesize <= 1024*1024){
            temp = df.format(1.0 * filesize/1024) + " KB";
        }
        else if(filesize <= 1024*1024*1024){
            temp = df.format(1.0 * filesize/ (1024*1024 ) ) + " MB";
        }
        else if(filesize <= 1024*1024*1024){
            temp = df.format(1.0 * filesize/ (1024*1024 ) ) + " MB";
        }
        else{
            temp = df.format(1.0 * filesize/ (1024*1024*1024 ) ) + " GB";
        }

        return temp;
    }

    public void setFilesize(long filesize) {
        this.filesize = filesize;
    }
}
