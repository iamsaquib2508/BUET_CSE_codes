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

public class LogSeed implements Initializable {

    @FXML
    TableView <SQSeed> SeedTable;

    @FXML
    private TableColumn <SQSeed, String> seedFileName;

    @FXML
    private TableColumn <SQSeed, String> seedIP;

    @FXML
    private TableColumn <SQSeed, Integer> seedPort;

    @FXML
    private TableColumn <SQSeed,String> status;

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
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/SeedLog.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        seedFileName.setCellValueFactory(new PropertyValueFactory<SQSeed, String>("filename"));
        seedIP.setCellValueFactory(new PropertyValueFactory<SQSeed, String>("ip"));
        seedPort.setCellValueFactory(new PropertyValueFactory<SQSeed, Integer>("port"));
        status.setCellValueFactory(new PropertyValueFactory<SQSeed, String>("status"));

        ObservableList <SQSeed> observableList = FXCollections.observableArrayList(TrackerLogPage.SeedList);

        SeedTable.setItems(observableList);
    }
}
