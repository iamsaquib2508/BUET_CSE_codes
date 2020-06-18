package Tracker_GUI;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;


public class LogDL implements Initializable {
    @FXML
    private TableView<SQDownload> DownloadTable;

    @FXML
    private TableColumn<SQDownload, String> filename;

    @FXML
    private TableColumn<SQDownload, String> ip;

    @FXML
    private TableColumn<SQDownload, Integer> port;

    @FXML
    private Button back;

    @FXML
    private Button refresh;

    @FXML
    void Back(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/TrackerLogPage.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @FXML
    void Refresh(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/DownloadLog.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        filename.setCellValueFactory(new PropertyValueFactory<SQDownload, String>("filename"));
        ip.setCellValueFactory(new PropertyValueFactory<SQDownload, String>("ip"));
        port.setCellValueFactory(new PropertyValueFactory<SQDownload, Integer>("port"));

        ObservableList <SQDownload> observableList = FXCollections.observableArrayList(TrackerLogPage.DownloadList);

        DownloadTable.setItems(observableList);
    }
}
