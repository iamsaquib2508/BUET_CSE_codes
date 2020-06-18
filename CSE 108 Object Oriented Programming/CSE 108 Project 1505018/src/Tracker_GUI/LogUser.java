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

public class LogUser implements Initializable{

    @FXML
    private TableView<SQUser> UserTable;

    @FXML
    private TableColumn<SQUser, String> userIp;

    @FXML
    private TableColumn<SQUser, Integer> userPort;

    @FXML
    private TableColumn<SQUser, String> userStatus;

    @FXML
    private Button userBack;

    @FXML
    private Button userRefresh;

    @FXML
    void Back(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/TrackerLogPage.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @FXML
    void Refresh(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/UserLog.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        userIp.setCellValueFactory(new PropertyValueFactory<SQUser, String>("ip"));
        userPort.setCellValueFactory(new PropertyValueFactory<SQUser, Integer>("port"));
        userStatus.setCellValueFactory(new PropertyValueFactory<SQUser, String>("status"));

        ObservableList <SQUser> observableList = FXCollections.observableArrayList(TrackerLogPage.UserList);

        UserTable.setItems(observableList);
    }
}