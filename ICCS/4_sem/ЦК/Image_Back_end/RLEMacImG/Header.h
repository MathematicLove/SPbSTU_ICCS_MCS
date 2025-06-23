#pragma once
#include <iostream>
#include <string>


std::string runLengthEncode(const std::string& input) {
    std::string encoded;
    int count = 1;

    for (int i = 1; i <= input.length(); ++i) {
        if (i == input.length() || input[i] != input[i - 1]) {
            encoded += input[i - 1] + std::to_string(count);
            count = 1;
        }
        else {
            ++count;
        }
    }

    return encoded;
}


std::string runLengthDecode(const std::string& input) {
    std::string decoded;

    for (int i = 0; i < input.length(); i += 2) {
        char character = input[i];
        int count = input[i + 1] - '0';

        for (int j = 0; j < count; ++j) {
            decoded += character;
        }
    }

    return decoded;
}



