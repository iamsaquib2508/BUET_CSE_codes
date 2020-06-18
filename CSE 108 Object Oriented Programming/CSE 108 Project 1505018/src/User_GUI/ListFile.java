package User_GUI;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.DirectoryChooser;
import Utility_Classes.NCN;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;

import User.DL1_Thread;


public class ListFile implements Initializable{
    @FXML
    private Button Download;

    @FXML
    private Button Back;

    @FXML
    private TableView <SQFile> FListTable;

    @FXML
    private TableColumn <SQFile,String> filename;

    @FXML
    private TableColumn<SQFile, Long> filesize;

    @FXML
    private TableColumn<SQFile, Integer> seeder;

    @FXML
    static ObservableList <SQFile> list;

    @FXML
    private Button refresh;

    NCN server;

    @FXML
    void file_list_refresh(ActionEvent event) throws IOException {
        server.write("Show");
        String [] ss;
        ss = (String[]) server.read();

        ControlPage.fileLists = null;
        ControlPage.fileLists = new ArrayList<SQFile>();

        for(int i = 0;i < ss.length ; i++) {
            String [] a = ss[i].split("\n");
            SQFile sqfile = new SQFile(a[0],a[1],a[2]);
            ControlPage.fileLists.add(sqfile);
        }

        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesUSER/FileList.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @FXML
    void File_List_Back(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesUSER/ButtonPage.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @FXML
    void File_List_Download(ActionEvent event) {
        SQFile sqfile = FListTable.getSelectionModel().getSelectedItem();
        if(sqfile ==null) {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Error!");
            alert.setHeaderText(null);
            alert.setContentText("You forgot to choose a file.");

            alert.showAndWait();
            return;
        }

        if(sqfile.getSeeder() == 0) {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Error!");
            alert.setHeaderText("Sorry");
            alert.setContentText("Currently no seeder is available for this file!");

            alert.showAndWait();
            return;
        }

        DirectoryChooser directoryChooser = new DirectoryChooser();
        directoryChooser.setTitle("Choose Destination");
        File destination = directoryChooser.showDialog(Download.getScene().getWindow());
        if(destination==null) {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Error!");
            alert.setHeaderText(null);
            alert.setContentText("You haven't chosen the directory.");

            alert.showAndWait();
            return;
        }

        String filename = sqfile.getFilename();
        long filesize = sqfile.getFilesizel();
        ProgressBar progressBar = new ProgressBar(0.0);

        SQDownload sqdownload = new SQDownload(filename,filesize,progressBar,destination);

        ControlPage.downloadLists.add(sqdownload);

        NCN server = Proceedings.SERVER;
        server.write("Get " + filename);
        new DL1_Thread(filename,destination,progressBar);

    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        server = Proceedings.SERVER;
        filename.setCellValueFactory(new PropertyValueFactory<>("filename"));
        filesize.setCellValueFactory(new PropertyValueFactory<SQFile, Long>("filesize"));
        seeder.setCellValueFactory(new PropertyValueFactory<SQFile, Integer>("seeder"));
        list = FXCollections.observableArrayList(ControlPage.fileLists);
        FListTable.setItems(list);
    }
}
