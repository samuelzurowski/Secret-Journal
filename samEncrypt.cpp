//
//  samEncrypt.cpp
//  Journal
//
//  Created by Samuel Zurowski on 6/29/18.
//  Copyright © 2018 Samuel Zurowski. All rights reserved.
//

#include "samEncrypt.hpp"
#include <iostream>

std::string encryptDecrypt(std::string toEncrypt) {
    char key[5] = {'Z', 'A', 'B','X', 'F'};
    std::string result = toEncrypt;
    
    for(int i = 0; i < toEncrypt.size(); i++) {
        result[i] = toEncrypt[i] ^ key[i % (sizeof(key)/sizeof(char))];
    }
    return result;
}
