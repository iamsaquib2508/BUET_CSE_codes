package User_GUI;

import Utility_Classes.NCN;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import User.UPG_Thread;
import javafx.stage.WindowEvent;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Hashtable;

public class Proceedings {

    @FXML
    private Button NextPage;

    @FXML
    public TextField clientport = new TextField() ;

    @FXML
    public TextField serverport = new TextField() ;

    @FXML
    public TextField serverip = new TextField();


    public static Hashtable<String, String> hashuser ; // <file,directory>
    public static int SEPORT;
    public static int CLIENTUPPORT;
    public static String SERVIP;
    public static NCN SERVER;

    @FXML
    void Next_Page(ActionEvent event) {
        hashuser = new Hashtable<>();
        if(clientport.getText().equals("") || serverport.getText().equals("") || serverip.getText().equals(""))
        {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Error");
            alert.setHeaderText("Oops!");
            alert.setContentText("At least one of the fields is empty.");

            alert.showAndWait();
            return;
        }
        try {
            CLIENTUPPORT = Integer.parseInt(clientport.getText());
            SEPORT = Integer.parseInt(serverport.getText());
        }catch (NumberFormatException ex){
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error!");
            alert.setHeaderText("Integer not found.");
            alert.setContentText("Put integers in 10000-65530 range as ports.");

            alert.showAndWait();
            return;
        }
        if(CLIENTUPPORT<10000 || CLIENTUPPORT>65530 || SEPORT<10000 || SEPORT>65530){
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error!");
            alert.setHeaderText("Invalid port(s).");
            alert.setContentText("Both the ports must be in 10000-65530 range.");

            alert.showAndWait();
            return;
        }
        SERVIP = serverip.getText();
        Socket s = null;
        ServerSocket sock = null;
        try {
            s = new Socket(SERVIP,SEPORT);
        } catch (IOException e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("ERROR!");
            alert.setHeaderText("Server not found!");
            alert.setContentText("Check Server IP and Server Port or Internet connection.");

            alert.showAndWait();
            return;

        }


        try {
            sock= new ServerSocket(Proceedings.CLIENTUPPORT);
        } catch (IOException ex) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("ERROR!");
            alert.setHeaderText("Something went wrong!");
            alert.setContentText("Couldn't create seeding facility. Please try again with different client port.");

            alert.showAndWait();
            return;
        }
        SERVER = new NCN(s);
        new UPG_Thread(sock);

        Parent root = null;
        try {
            root = FXMLLoader.load(getClass().getResource("FXMLFilesUSER/ButtonPage.fxml"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        Scene scene = new Scene(root);
        Main.stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            public void handle(WindowEvent we) {

                /*Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
                alert.setTitle("Confirmation");
                alert.setHeaderText(null);
                alert.setContentText("Are you sure to exit?");

                Optional<ButtonType> result = alert.showAndWait();
                if (result.get() == ButtonType.OK){
                    SERVER.write("EXIT");
                    System.exit(0);
                } else {
                    Main.stage.
                }*/
                SERVER.write("EXIT");
                System.exit(0);
            }
        });
        Main.stage.setScene(scene);
    }
}
