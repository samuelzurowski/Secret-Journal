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
    std::string PASS_FILE = "password.txt";
    create_password(PASS_FILE);
    while(true) {
        top_info();
        int chooseOption = NULL;
        std::string currentDate = get_date();
        std::string FILENAME = "journal.txt";
        std::cout << "What is your choice?: ";
        while (!(std::cin >> chooseOption) || chooseOption < 0) // <<< note use of "short circuit" logical operation here
        {
            std::cout << "Bad input - try again: ";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n'); // NB: preferred method for flushing cin
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
    time_t t = time(0);
    struct tm * now = localtime(&t);
    int day = now->tm_mday;
    int month = now->tm_mon + 1;
    int year = now->tm_year + 1900;
    std::string date = std::to_string(month) + '/' + std::to_string(day) + '/' + std::to_string(year);
    return date;
}
void write_file(std::string file, std::string date) {
    std::string entree = "";
    std::string line = "";
    std::string description = "";
    std::cout << "Enter Description of Entry: ";
    std::cin >> std::ws;
    getline(std::cin, description);
    while(true) {
        std::cout << "Enter line of text (q to quit): ";
        std::cin >> std::ws;
        getline(std::cin, line);
        if(line == "q") break;
        
        entree += line + '\n';
    }
    std::string data = encryptDecrypt(std::string(50, '*') + "\nDATE: " + date + "\nDESCRIPTION: " + description + "\n" + entree + std::string(50, '*')) + "\n\n";
    std::fstream openfile(file, std::ios_base::out | std::ios_base::in | std::ios::app);
    if(openfile.good()) {
        openfile << data;
        openfile.close();
    }
}

void readFile(std::string filename, std::string date) {
    std::string contents;
    std::string str;
    std::fstream i_file(filename);
    while(std::getline(i_file, str)) {
        contents +=encryptDecrypt(str);
        contents.push_back('\n');
    }
    i_file.close();
    std::cout << "\nJournal Enteries: \n\n";
    std::cout <<  contents + '\n';
}

void header() {
    std::cout << std::string(50, '*') << std::endl;
}

void top_info() {
    header();
    std::cout << "SAM'S JOURNAL ENTRIES" <<std::endl;
    header();
    std::cout << "\nChoose Options: \n1. Journal Entry\n2. Read Journal Entries\n3. Change Password\n4. Quit\n\n";
    header();
}

bool create_password(std::string pFile) {
    if(is_empty() == false && file_exists() == true) {
        return true;
    } else {
        std::string phrase;
        std::ofstream passwordFile(pFile);
        if(passwordFile.good()) {
            std::cout << "Set Password: ";
            std::cin >> std::ws;
            getline(std::cin, phrase);
            phrase = encryptDecrypt(phrase);
            passwordFile << phrase;
        }
    }
    return false;
}

bool is_empty() {
    std::ifstream in("password.txt");
    if(in.is_open()){
        in.seekg(0,std::ios::end);
        size_t size = in.tellg();
        if( size == 0) {
            std::cout << "File is empty\n";
            return true;
        }
    }
    return false;
}

bool file_exists() {
    std::ifstream f("password.txt");
    return f.good();
}

void change_password(std::string pFile) {
    std::string password;
    std::string phraseFile;
    std::fstream passwordfile(pFile);
    while(std::getline(passwordfile, phraseFile)) {
        password = phraseFile;
    }
    passwordfile.close();
    std::string guess;
    std::cout << "\n\nPlease Enter Your Old Password: ";
    std:: cin >> guess;
    if(encryptDecrypt(password) == guess) {
        remove(pFile.c_str());
        std::string phrase;
        std::ofstream passwordFile(pFile);
        if(passwordFile.good()) {
            std::cout << "Set Password: ";
            std::cin >> std::ws;
            getline(std::cin, phrase);
            phrase = encryptDecrypt(phrase);
            std::cout << phrase;
            passwordFile << phrase;
            passwordFile.close();
        }
    } else std::cout << "Incorrect Password Given...\n\n";
}

bool check_password(std::string pFile) {
    std::string password;
    std::string phraseFile;
    std::fstream passwordfile(pFile);
    while(std::getline(passwordfile, phraseFile)) {
        password = phraseFile;
    }
    passwordfile.close();
    std::string guess;
    std::cout << "\n\nTo Access this please give the password\nPlease Enter Your Password: ";
    std:: cin >> guess;
    if(encryptDecrypt(password) == guess) {
        return true;
    }else std::cout << "Incorrect Password Given...\n\n";
    return false;
}
