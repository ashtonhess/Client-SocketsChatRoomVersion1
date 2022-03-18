//
// Created by Ashton Hess on 3/17/22.
//
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

    bool status=true;
    char* inputAddy;
    int sock = 0;
    int valread;
    struct sockaddr_in serverAddy;
    char *msgToServer = "";
    char buffer[MAX_LINE] = {};
    char userInput[MAX_LINE] = {};

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
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, inputAddy, &serverAddy.sin_addr)<=0){
        cout<<">Error: Invalid address."<<endl;
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serverAddy, sizeof(serverAddy)) < 0){
        cout<<"> Error: Connection Failed."<<endl;
        return -1;
    }
    //send(sock , msg , strlen(msg) , 0 );
    //send(sock , msg , strlen(msg) , 0 );

    //cout<<"msg sent to server"<<endl;
    //valread = read( sock , buffer, MAX_LINE);
    int i = recv(sock, buffer, MAX_LINE, 0);
    //cout<<"VALUE OF I: "<<i<<endl;
    if (i==-1){
        cout<<"> Error: receiving."<<endl;
    }
    printf("%s\n",buffer );
    bool sendToTheServer=false;
    while(status){
        cout<<"> ";
        //cin>>buffer;
        cin.getline(buffer, 50, '\n');
        strncpy(userInput, buffer, sizeof(buffer));
        //string userInputUpToASpace = userInput;
        vector<string> delimitVec = split(userInput,' ');
        string firstStr=delimitVec.at(0);
        //cout<<"FIRST STR: "<<firstStr<<endl;
        //cout<<"delimitVec size: "<<delimitVec.size()<<endl;
        //cout<<buffer<<endl;
        if (delimitVec.at(0)=="login"){
            //cout<<"LOGIN TRIGGERED"<<endl;
            sendToTheServer=true;
        }else if(delimitVec.at(0)=="newuser"){
            //cout<<"NEWUSER TRIGGERED"<<endl;

        }else if(delimitVec.at(0)=="send"){
            //cout<<"SEND TRIGGERED"<<endl;
            sendToTheServer=true;

            send(sock , msg , strlen(msg) , 0 );
        }else if (delimitVec.at(0)=="logout"){
            //cout<<"LOGOUT"<<endl;
            sendToTheServer=true;
            status=false;
        }else{
            cout<<"> "<<"Enter a valid command: 'login', 'newuser', 'send', 'logout'"<<endl;
        }
        if(sendToTheServer==true){
            //buffer[MAX_LINE] = "some shit";
            strcpy("something", buffer);
            cout<<"SENDING"<<endl;
            send(sock,buffer, strlen(buffer),0);
            cout<<"SENT"<<endl;
            //int i = recv(sock, buffer, MAX_LINE, 0);
            //buffer[i]=0;
            cout<<"> "<<buffer<<endl;
        }

        //status = false;
    }
    //recv()

    //int closeSocket;
    //closeSocket=close(sock);


    return 0;
}

vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}