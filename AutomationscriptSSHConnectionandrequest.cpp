#include <stdlib.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h> 
#include <iostream>

using namespace std;

int downloadFile(ssh_session session, const char* remotePath, const char* localPath);



int main(){
    ssh_session session;
    int port = 22;
    char* password;
    char* username;
    int rc;


    //creating session
    username="emerson";
    session = ssh_new();
    ssh_options_set(session, SSH_OPTIONS_HOST, "raspberrypi.local");
    ssh_options_set(session, SSH_OPTIONS_USER, &username);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);

    //Attempting to connect to session
    rc=ssh_connect(session);
    if (rc!=SSH_OK){
        cout<<"Error connecting to session: "<< ssh_get_error(session);
        ssh_free(session);
        exit(-1);
    }



    ssh_session_update_known_hosts(session);
    
    password="emerson";

    //username MAY be username here, if this doesnt work, try null
    rc=ssh_userauth_password(session,NULL,password);
    if (rc !=SSH_AUTH_SUCCESS){

        cout<<"Error authenticating User: "<<ssh_get_error(session);
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }



    //attempting to copy file
    char* remotePath = "scripts\\sensor_data.csv";
    char* localPath = "D:\\Emerson Data\\sensor_data.csv";
    downloadFile(session,remotePath,localPath);


}

int downloadFile(ssh_session session, const char* remotePath, const char* localPath) {
    sftp_session sftp;
    sftp_file file;
    // Start an SFTP session
    sftp = sftp_new(session);
    if (sftp == nullptr) {
        cout << "Error creating SFTP session." << std::endl;
        return 1;
    }

    // Initialize the SFTP session
    int rc = sftp_init(sftp);
    if (rc != SSH_OK) {
        cout<< "Error initializing SFTP session: " << ssh_get_error(session) << std::endl;
        sftp_free(sftp);
        return 1;
    }

    // Open the remote file for reading
    // The OFN_READONLY is supposed to be O_RDONLY which causes errors, if cause issues, try 00
    file = sftp_open(sftp, remotePath, OFN_READONLY, 0);
    
    if (file == nullptr) {
        cout << "Error opening remote file for reading: " << ssh_get_error(session) << std::endl;
        sftp_free(sftp);
        return 1;
    }

    // Open the local file for writing
    FILE* localFile = fopen(localPath, "wb");
    if (localFile == nullptr) {
        cout << "Error opening local file for writing." << std::endl;
        sftp_close(file);
        sftp_free(sftp);
        return 1;
    }

    // Read data from the remote file and write to the local file
    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = sftp_read(file, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytesRead, localFile);
    }

    // Close the local and remote files
    fclose(localFile);
    sftp_close(file);

    // Free the SFTP session
    sftp_free(sftp);

    cout << "File downloaded successfully." << endl;

    return 0;
}