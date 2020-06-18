package User_GUI;

import Utility_Classes.NCN;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.stage.FileChooser;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Optional;
import java.util.ResourceBundle;

public class ControlPage implements Initializable{

    @FXML
    private Button down;

    @FXML
    private Button progress;

    @FXML
    private Button file;

    @FXML
    private Button FileSeed;

    @FXML
    private Button exit;

    static ArrayList <SQFile> fileLists = new ArrayList<>();

    static ArrayList <SQDownload> downloadLists = new ArrayList<>();

    NCN server;

    @FXML
    void Exit(ActionEvent event) {
        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Exit Confirmation");
        alert.setHeaderText(null);
        alert.setContentText("Are you sure to exit? Press OK to proceed.");

        Optional<ButtonType> result = alert.showAndWait();
        if (result.get() == ButtonType.OK){
            server.write("EXIT");
            System.exit(0);
        } else {
            return;
        }

    }

    @FXML
    void Seed(ActionEvent event) {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Choose File");
        File file = fileChooser.showOpenDialog(FileSeed.getScene().getWindow());
        if(file == null){
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Wait");
            alert.setHeaderText(null);
            alert.setContentText("Please choose a file");

            alert.showAndWait();
            return;
        }
        server.write("Seed " + file.getName() + "\n" + Proceedings.CLIENTUPPORT + "\n" + file.length());
        Proceedings.hashuser.put(file.getName(), file.getParent());

    }

    @FXML
    void file_list(ActionEvent event) throws IOException {
        server = Proceedings.SERVER;
        server.write("Show");
        String [] ss;
        ss = (String[]) server.read();

        fileLists = null;
        fileLists = new ArrayList<SQFile>();

        for(int i = 0;i < ss.length ; i++) {
            String [] a = ss[i].split("\n");
            SQFile sqfile = new SQFile(a[0],a[1],a[2]);
            fileLists.add(sqfile);
        }

        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesUSER/FileList.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @FXML
    void progress_list(ActionEvent event) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLFilesUSER/DownloadList.fxml"));
        Scene scene = new Scene(root);
        Main.stage.setScene(scene);
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        server = Proceedings.SERVER;
    }
}
