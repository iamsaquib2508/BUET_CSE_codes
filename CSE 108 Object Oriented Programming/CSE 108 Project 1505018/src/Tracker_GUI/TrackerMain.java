package Tracker_GUI;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import Tracker.Server_Run;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.URL;
import java.util.ResourceBundle;


public class TrackerMain implements Initializable{

    @FXML
    private TextField serverPort = new TextField();

    @FXML
    private Button Connect;

    static int SERVERPORT;

    @FXML
    void TrackerConnect(ActionEvent event) {
        String a = serverPort.getText();
        int p;
        if(a.equals("")){
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Error!");
            alert.setHeaderText(null);
            alert.setContentText("Please specify server port.");

            alert.showAndWait();
            return;
        }
        try{
            p=Integer.parseInt(a);
        }catch(NumberFormatException ex){
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error!");
            alert.setHeaderText("Not an integer!");
            alert.setContentText("Put an integer in 10000-65530 range.");

            alert.showAndWait();
            return;
        }
        if(p<10000 || p>65530){
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error!");
            alert.setHeaderText("Invalid server port.");
            alert.setContentText("Integer must be in 10000-65530 range.");

            alert.showAndWait();
            return;
        }
        ServerSocket server = null;

        try {
            server = new ServerSocket(p);
        } catch (IOException e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error!");
            alert.setHeaderText("Something went wrong!");
            alert.setContentText("Could not open port at the specified port. Please choose a different port.");

            alert.showAndWait();
            return;
        }
        SERVERPORT=p;
        new Server_Run(server);


        Parent root = null;
        try {
            root = FXMLLoader.load(getClass().getResource("FXMLFilesTracker/TrackerLogPage.fxml"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }


}
