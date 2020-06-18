package Tracker_GUI;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Label;

import java.io.IOException;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Optional;
import java.util.ResourceBundle;


public class TrackerLogPage implements Initializable {

    @FXML
    private Label serverIP;

    @FXML
    private Button userLog;

    @FXML
    private Button seedLog;

    @FXML
    private Button downloadLog;

    @FXML
    private Button disconnect;

    @FXML
    private Label serverport;

    public static ArrayList <SQSeed> SeedList = new ArrayList<>();
    public static ArrayList <SQUser> UserList = new ArrayList<>();
    public static ArrayList <SQDownload> DownloadList = new ArrayList<>();

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        try {
            serverIP.setText(InetAddress.getLocalHost().getHostAddress());
            serverport.setText(String.valueOf(TrackerMain.SERVERPORT));
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
    }


    @FXML
    void Disconnect(ActionEvent event) {
        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Exit Confirmation");
        alert.setHeaderText(null);
        alert.setContentText("Do you really want to discard the server? Press OK to proceed.");

        Optional<ButtonType> result = alert.showAndWait();
        if (result.get() == ButtonType.OK){

            System.exit(0);
        } else {
            return;
        }
    }

    @FXML
    void Download(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/DownloadLog.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @FXML
    void Seed(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/SeedLog.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @FXML
    void User(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/UserLog.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }
}
