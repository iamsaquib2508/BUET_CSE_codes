package User_GUI;

import java.text.DecimalFormat;


public class SQFile {
    private String filename;
    private long filesize;
    private int seeder;

    public SQFile(String filename, String filesize, String seed){
        this.filename = filename;
        this.filesize = Long.parseLong(filesize);
        this.seeder = Integer.parseInt(seed);
    }

    public long getFilesizel(){
        return filesize;
    }

    public String getFilesize() {
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


    public int getSeeder() {
        return seeder;
    }

    public void setSeeder(int seeder) {
        this.seeder = seeder;
    }

    public void setFilesize(long filesize) {
        this.filesize = filesize;
    }

    public String getFilename() {
        return filename;
    }

    public void setFilename(String filename) {
        this.filename = filename;
    }

}
