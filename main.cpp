//  Journal
//
//  Created by Samuel Zurowski on 6/28/18.
//  Copyright © 2018 Samuel Zurowski. All rights reserved.

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <limits>
#include <ctime>
#include <stdio.h>
#include "samEncrypt.hpp"

bool create_password(std::string);
bool is_empty();
void readFile(std::string, std::string);
void write_file(std::string, std::string);
void header();
void change_password(std::string);
void top_info();
bool file_exists();
bool check_password(std::string);
std::string getdate();
std::string get_date();

int main(int argc, const char * argv[]) {
    std::string PASS_FILE = "password.txt"; // pass file
    create_password(PASS_FILE);
    while(true) {
        top_info(); // prints top information
        int chooseOption = NULL;
        std::string currentDate = get_date(); // gets date
        std::string FILENAME = "journal.txt";
        std::cout << "What is your choice?: ";
        while (!(std::cin >> chooseOption) || chooseOption < 0) // <<< note use of "short circuit" logical operation here
        {
            std::cout << "Bad input - try again: ";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n'); //to flush cin
        }
        switch (chooseOption) {
                
            case 1: //Journal Entry
                if(check_password(PASS_FILE) == false) break;
                write_file(FILENAME, currentDate);
                break;
            case 2: //Read Journal
                if(check_password(PASS_FILE) == false) break;
                readFile(FILENAME, currentDate);
                break;
            case 3: //Change Password
                change_password(PASS_FILE);
                break;
            case 4:
                return 0;
            default:
                std::cout << "Please choose a valid option.\n";
                break;
        }
    }
    return 0;
}
std::string get_date() {
    time_t t = time(0); // get current time
    struct tm * now = localtime(&t); // struct time
    int day = now->tm_mday;
    int month = now->tm_mon + 1;
    int year = now->tm_year + 1900;
    std::string date = std::to_string(month) + '/' + std::to_string(day) + '/' + std::to_string(year);
    return date; // formats the time month/day/year
}
void write_file(std::string file, std::string date) {
    std::string entree = "";
    std::string line = "";
    std::string description = "";
    std::cout << "Enter Description of Entry: ";
    std::cin >> std::ws;
    getline(std::cin, description); //get line of the description
    while(true) { // used to write to file
        std::cout << "Enter line of text (q to quit): ";
        std::cin >> std::ws; // clear
        getline(std::cin, line); // get the line of cin
        if(line == "q") break; // if only q break.
        
        entree += line + '\n'; // used to concatenate string
    }
    std::string data = encryptDecrypt(std::string(50, '*') + "\nDATE: " + date +
    "\nDESCRIPTION: " + description + "\n" + entree + std::string(50, '*')) + "\n\n"; // sends the full string to file
    
    std::fstream openfile(file, std::ios_base::out | std::ios_base::in | std::ios::app);
    if(openfile.good()) {
        openfile << data; // writes the data to file
        openfile.close();
    }
}

void readFile(std::string filename, std::string date) {
    std::string contents;
    std::string str;
    std::fstream i_file(filename);
    while(std::getline(i_file, str)) { // gets each line of the file
        contents +=encryptDecrypt(str); // decrypts the string.
        contents.push_back('\n'); // new line the contents
    }
    i_file.close(); // close the file
    std::cout << "\nJournal Enteries: \n\n";
    std::cout <<  contents + '\n';
}

void header() {
    std::cout << std::string(50, '*') << std::endl; // put 50 *
}

void top_info() { // used for formating header.
    header();
    std::cout << "SAM'S JOURNAL ENTRIES" <<std::endl;
    header();
    std::cout << "\nChoose Options: \n1. Journal Entry\n2. Read Journal Entries\n3. Change Password\n4. Quit\n\n";
    header();
}

bool create_password(std::string pFile) { // create password
    if(is_empty() == false && file_exists() == true) {
        return true; // used to check if the password exists
    } else {
        std::string phrase;
        std::ofstream passwordFile(pFile);
        if(passwordFile.good()) {
            std::cout << "Set Password: ";
            std::cin >> std::ws;
            getline(std::cin, phrase);
            phrase = encryptDecrypt(phrase);
            passwordFile << phrase; // puts your password encrypted in file
        }
    }
    return false;
}

bool is_empty() {
    std::ifstream in("password.txt");
    if(in.is_open()){
        in.seekg(0,std::ios::end); // puts cursor at the begining file to end
        size_t size = in.tellg();
        if(size == 0) {
            std::cout << "File is empty\n";
            return true;
        }
    }
    return false;
}

bool file_exists() {
    std::ifstream f("password.txt"); // checks if file exists
    return f.good();
}

void change_password(std::string pFile) { // used to change password
    std::string password;
    std::string phraseFile;
    std::fstream passwordfile(pFile);
    while(std::getline(passwordfile, phraseFile)) {
        password = phraseFile; // gets the password
    }
    passwordfile.close();
    std::string guess;
    std::cout << "\n\nPlease Enter Your Old Password: ";
    std:: cin >> guess; // check password
    if(encryptDecrypt(password) == guess) {
        remove(pFile.c_str()); // removes the the file contents
        std::string phrase;
        std::ofstream passwordFile(pFile);
        if(passwordFile.good()) {
            std::cout << "Set Password: ";
            std::cin >> std::ws;
            getline(std::cin, phrase);
            phrase = encryptDecrypt(phrase); // sets password and encrypts
            std::cout << phrase;
            passwordFile << phrase;
            passwordFile.close();
        }
    } else std::cout << "Incorrect Password Given...\n\n"; // if no pass enter
}

bool check_password(std::string pFile) {
    std::string password;
    std::string phraseFile;
    std::fstream passwordfile(pFile);
    while(std::getline(passwordfile, phraseFile)) { // gets content of file.
        password = phraseFile;
    }
    passwordfile.close();
    std::string guess;
    std::cout << "\n\nTo Access this please give the password\nPlease Enter Your Password: ";
    std:: cin >> guess;
    if(encryptDecrypt(password) == guess) { // if password encrypted value of it
        return true;
    }else std::cout << "Incorrect Password Given...\n\n";
    return false;
}
