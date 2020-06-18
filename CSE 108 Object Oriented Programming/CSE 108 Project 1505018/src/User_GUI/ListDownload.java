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
import Utility_Classes.NCN;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;


public class ListDownload implements Initializable{
    @FXML
    private Button DownloadListBack;

    @FXML
    private Button seedButton;

    @FXML
    private TableView <SQDownload> DownloadStatusTable;

    @FXML
    private TableColumn<SQDownload, String> filename;

    @FXML
    private TableColumn<SQDownload, Long> filesize;

    @FXML
    private TableColumn<SQDownload, ProgressBar> status;

    NCN server;


    @Override
    public void initialize(URL location, ResourceBundle resources) {
        server = Proceedings.SERVER;
        filename.setCellValueFactory(new PropertyValueFactory<SQDownload, String>("filename"));
        filesize.setCellValueFactory(new PropertyValueFactory<SQDownload,Long>("filesize"));
        status.setCellValueFactory(new PropertyValueFactory<SQDownload, ProgressBar>("progressBar"));

        ObservableList <SQDownload> observableList = FXCollections.observableList(ControlPage.downloadLists);

        DownloadStatusTable.setItems(observableList);
    }

    @FXML
    void downloadSeed(ActionEvent event) {

        SQDownload sqdownload = DownloadStatusTable.getSelectionModel().getSelectedItem();
        if(sqdownload ==null)
        {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Error!");
            alert.setHeaderText(null);
            alert.setContentText("You forgot to choose a file");

            alert.showAndWait();
            return;
        }
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Done!");
        alert.setHeaderText("Seed complete.");
        alert.setContentText("File " + sqdownload.getFilename()+ " has been seeded." );

        alert.showAndWait();
        String temp = "1505018";
        server.write("Seed " + sqdownload.getFilename() + "\n" + Proceedings.CLIENTUPPORT + "\n" + temp);
        Proceedings.hashuser.put(sqdownload.getFilename(), sqdownload.getDestination());
    }

    @FXML
    void Download_List_Back(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesUSER/ButtonPage.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }
}
