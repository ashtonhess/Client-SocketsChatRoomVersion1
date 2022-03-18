//
// Created by Ashton Hess on 3/17/22. -> Migrating so I can use my Mac. Now using makefiles instead of VS.
// Pawprint: AAHB8F
//
//
// Program description:
// I implemented a simple chat room in c++ with both a server and client that both use
//the socket API. The client program provides the following commands to the user: 'login'(allow users to join the chat room),
// 'newuser'(create a new user account), 'send'(send a message to other clients; actually send the message to the server and
// the server forwards the message to other clients), and 'logout'(quit the chat room).
//
// The server runs the chat room service and echoes messages back to the client.
// Storage of all users that are created is implemented to store a list in users.txt.
//
//
// How to run:
// First, you will need a C++14 compiler installed.
// Second, navigate to each program's folder in terminal.
// Third, run the command "make" in each terminal. This will create the object files and the executables.
// Last, run the command "./server" in the server terminal, and "./client" (appending the server address argument) in the client's terminal.
//
// Note: To clean, run make clean in both project directories.
//
//Client output:
/*
ashtonhess@Bushes-Pro-2 client % ./client 127.0.0.1
Ashton's chat room client. Version One.
> newuser Mike Mike11
> New user account created. Please login.
> newuser Mike Mike11
> Denied. User account already exists.
> send
> Denied. send incorrect arguments.
> login Tom Tom12
> Denied. User name or password incorrect.
> login Tom Tom11
> login confirmed
> send Hello, is anybody out there?
> Tom: Hello, is anybody out there?
> send Bye for now.
> Tom: Bye for now.
> logout
> Tom left.
ashtonhess@Bushes-Pro-2 client %
 */

#include <iostream>
using namespace std;

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <vector>

#define SERVER_PORT 11328
#define MAX_LINE 256

vector<string> split (const string &s, char delim);

int main(int argc, char**argv){

    cout<<"Ashton's chat room client. Version One."<<endl;

    bool status=true;
    char* inputAddy;
    int sock = 0;
    int valread;
    struct sockaddr_in serverAddy;
    char *msgToServer = "";
    char buffer[MAX_LINE] = {};
    char userInput[MAX_LINE] = {};

    //verifying inputs
    if (argc < 2){
        cout<<"> Usage: client serverName"<<endl;
        return 0;
    }else{
        inputAddy=argv[1];
    }
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        cout<<"> Error: Socket initialization."<<endl;
        return -1;
    }
    serverAddy.sin_family = AF_INET;
    serverAddy.sin_port = htons(SERVER_PORT);
    //Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, inputAddy, &serverAddy.sin_addr)<=0){
        cout<<"> Error: Invalid address."<<endl;
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serverAddy, sizeof(serverAddy)) < 0){
        cout<<"> Error: Connection Failed."<<endl;
        return -1;
    }

    bool sendToTheServer;
    bool while1=false;
    while(!while1){
        sendToTheServer=false;
        cout<<"> ";
        cin.getline(userInput, MAX_LINE, '\n');
        string storedUserInput = userInput;
        char* storedChar = userInput;
        //splitting vector into groups by space
        vector<string> delimitVec;
        //see split function below
        delimitVec = split(userInput,' ');
        //checking first word of string against commands.
        if (delimitVec.at(0)=="login") {
            if(delimitVec.size()==3){
                if (delimitVec.at(1).size()>32 || delimitVec.at(1).size()<3){
                    cout<<"Denied. login userID too long."<<endl;
                }else if(delimitVec.at(2).size()<4 || delimitVec.at(2).size()>8){
                    cout<<"Denied. login password must be between 4 and 8 characters."<<endl;
                }else{
                    sendToTheServer=true;
                }
            }else{
                cout<<"Denied. login incorrect arguments."<<endl;
            }
        }else if (delimitVec.at(0)=="newuser"){
            if(delimitVec.size()==3){
                sendToTheServer=true;
            }else{
                cout<<"Denied. login incorrect arguments."<<endl;
            }
        }else if (delimitVec.at(0)=="send"){
            if(delimitVec.size()==1){
                cout<<"> Denied. send incorrect arguments."<<endl;
            }else{
                sendToTheServer=true;
            }
        }else if (delimitVec.at(0)=="logout"){
            if(delimitVec.size()==1){
                while1=true;
                sendToTheServer=true;
            }else{
                cout<<"Denied. logout incorrect arguments."<<endl;
            }
        }else{
            cout<<"Denied. Invalid command."<<endl;
        }
        //using this bool to determine when send should be invoked.
            if (sendToTheServer==true){
                send(sock, userInput, MAX_LINE,0);
                int r = recv(sock, buffer,MAX_LINE,0);
                if (r==-1){
                    cout<<"> Error: receiving."<<endl;
                }
                buffer[r]=0;
                cout<<buffer<<endl;
            }
    }

    close(sock);
    return 0;
}

//This function splits a string by a delimiter and places each piece in a string vector.
vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}








//strncpy(userInput, buffer, sizeof(buffer));

//send(sock, userInput, strlen(buffer), 0);


//cout<<"msg sent to server"<<endl;
//valread = read( sock , buffer, MAX_LINE);
//int i = recv(sock, buffer, MAX_LINE, 0);
//cout<<"VALUE OF I: "<<i<<endl;
//if (i==-1){
//    cout<<"> Error: receiving."<<endl;
//}
//printf("%s\n",buffer );

//        if (delimitVec.at(0)=="login"){
//            int loginResult= login(delimitVec.at(1),delimitVec.at(2));
//            //return 0 for user does not exist; return 1 for login success; return 2 for wrong password.
//            if (loginResult!=1){
//                cout<<"> Denied. User name or password incorrect."<<endl;
//            }else{
//                logInStatus=true;
//                const char *logInSuccessMsg= "> login confirmed";
//                strcpy(buffer, logInSuccessMsg);
//                send(currentSocket, buffer, strlen(buffer), 0);
//                userID = delimitVec.at(1);
//            }
//
//        }else if (delimitVec.at(0)=="newuser"){
//
//            //Create a new user account; returns 0 if userID account already exists; returns 1 if user successfully added.
//            int newUserResult = newUser(delimitVec.at(1), delimitVec.at(2));
//            if (newUserResult==1){
//                cout<<"New user account created."<<endl;//---------------------------------------------------------------NEEDED--------------------------------------------------------------------------
//                const char *userAcctCreatedSuccess= "> New user account created. Please login.";
//                strcpy(buffer, userAcctCreatedSuccess);
//                send(currentSocket, buffer, strlen(buffer), 0);
//            }else{
//                //cout<<"> Denied. User account already exists."<<endl;
//                const char *userAcctExists= "> Denied. User account already exists.";
//                strcpy(buffer, userAcctExists);
//                send(currentSocket, buffer, strlen(buffer), 0);
//            }
//
//        }else if (delimitVec.at(0)=="send"){
//            if (logInStatus==true){
//
//                const char *userAcctExists= "> login confirmed";
//                strcpy(buffer, userAcctExists);
//                send(currentSocket, buffer, strlen(buffer), 0);
//
//            }else{
//                //cout<<"> Denied. Please login first."<<endl;
//                const char *loginBefore= "> Denied. Please login first.";
//                strcpy(buffer, loginBefore);
//                send(currentSocket, buffer, strlen(buffer), 0);
//            }
//        }else if (delimitVec.at(0)=="logout") {
//            if (logInStatus == true) {
//                userID = "";
//                logout();
//                const char *logoutMsg= "> You left.";
//                strcpy(buffer, logoutMsg);
//                send(currentSocket, buffer, strlen(buffer), 0);
//
//            }
//        }



//        cout<<"> ";
//        cin.getline(buffer, MAX_LINE, '\n');
//        strncpy(userInput, buffer, sizeof(buffer));
//        //string userInputUpToASpace = userInput;
//        vector<string> delimitVec = split(userInput,' ');
//        string firstStr=delimitVec.at(0);
//        //cout<<"FIRST STR: "<<firstStr<<endl;
//        //cout<<"delimitVec size: "<<delimitVec.size()<<endl;
//        //cout<<buffer<<endl;
//        if (delimitVec.at(0)=="login"){
//            //cout<<"LOGIN TRIGGERED"<<endl;
//            sendToTheServer=true;
//        }else if(delimitVec.at(0)=="newuser"){
//            //cout<<"NEWUSER TRIGGERED"<<endl;
//
//        }else if(delimitVec.at(0)=="send"){
//            //cout<<"SEND TRIGGERED"<<endl;
//            sendToTheServer=true;
//
//            send(sock , msgToServer , strlen(msgToServer) , 0 );
//        }else if (delimitVec.at(0)=="logout"){
//            //cout<<"LOGOUT"<<endl;
//            sendToTheServer=true;
//            status=false;
//        }else{
//            cout<<"> "<<"Enter a valid command: 'login', 'newuser', 'send', 'logout'"<<endl;
//        }
//        if(sendToTheServer==true){
//            //buffer[MAX_LINE] = "some shit";
////            strcpy("something", buffer);
////            cout<<"SENDING"<<endl;
////            send(sock,buffer, strlen(buffer),0);
////            cout<<"SENT"<<endl;
////            //int i = recv(sock, buffer, MAX_LINE, 0);
////            //buffer[i]=0;
////            cout<<"> "<<buffer<<endl;
//            //send(sock , msg , strlen(msg) , 0 );
//        }
//
//        //status = false;

    //recv()
    //int closeSocket;
    //closeSocket=close(sock);

