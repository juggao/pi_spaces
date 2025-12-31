#include <iostream>
#include <fstream>
#include <string>
#include <gmp.h>
#include <mpfr.h>
#include <algorithm>

// ANSI color codes
const std::string COLOR_RED = "\033[91m";
const std::string COLOR_YELLOW = "\033[93m";
const std::string COLOR_GREEN = "\033[92m";
const std::string COLOR_BLUE = "\033[94m";
const std::string COLOR_MAGENTA = "\033[95m";
const std::string COLOR_CYAN = "\033[96m";
const std::string COLOR_WHITE = "\033[97m";
const std::string COLOR_RESET = "\033[0m";

std::string getColor(char digit) {
    switch(digit) {
        case '2': return COLOR_RED;
        case '3': return COLOR_YELLOW;
        case '4': return COLOR_GREEN;
        case '5': return COLOR_BLUE;
        case '6': return COLOR_MAGENTA;
        case '7': return COLOR_CYAN;
        case '8': return COLOR_WHITE;
        default: return "";
    }
}

std::string computePiDigits(int n) {
    mpfr_t pi;
    mpfr_init2(pi, (n + 10) * 4);  // Extra precision
    mpfr_const_pi(pi, MPFR_RNDN);
    
    // Convert to string
    mpfr_exp_t exp;
    char* str = mpfr_get_str(NULL, &exp, 10, n + 10, pi, MPFR_RNDN);
    
    std::string result(str);
    mpfr_free_str(str);
    mpfr_clear(pi);
    
    // Remove leading '3' and get n digits
    if (result.length() > 0 && result[0] == '3') {
        result = result.substr(1);
    }
    
    // Ensure we have exactly n digits
    if (result.length() > n) {
        result = result.substr(0, n);
    }
    
    return "3" + result;
}

int findMaxConsecutiveSpaces(const std::string& text) {
    int maxSpaces = 0;
    int currentSpaces = 0;
    
    for (char c : text) {
        if (c == ' ') {
            currentSpaces++;
            maxSpaces = std::max(maxSpaces, currentSpaces);
        } else {
            currentSpaces = 0;
        }
    }
    
    return maxSpaces;
}

struct ProcessResult {
    int digitsProcessed;
    std::string piDigits;
    std::string replacedText;
    int position;
    int maxSpacesFound;
};

ProcessResult processPiUntilTarget(int targetSpaces, int chunkSize, int maxDigits) {
    std::cout << "Searching for " << targetSpaces << " consecutive spaces (9s or 0s)..." << std::endl;
    std::cout << "Processing in chunks of " << chunkSize << " digits...\n" << std::endl;
    
    ProcessResult result = {0, "", "", -1, 0};
    int n = chunkSize;
    bool found = false;
    
    while (n <= maxDigits && !found) {
        std::cout << "Computing " << n << " digits of PI..." << std::flush;
        std::string piStr = computePiDigits(n);
        std::cout << " done." << std::endl;
        
        // Replace 9 and 0 with spaces
        std::string replaced = piStr;
        std::replace(replaced.begin(), replaced.end(), '9', ' ');
        std::replace(replaced.begin(), replaced.end(), '0', ' ');
        
        // Check for consecutive spaces
        int currentSpaces = 0;
        for (size_t i = 0; i < replaced.length(); i++) {
            if (replaced[i] == ' ') {
                currentSpaces++;
                if (currentSpaces >= targetSpaces) {
                    found = true;
                    result.position = i - targetSpaces + 2;  // 1-indexed
                    result.maxSpacesFound = currentSpaces;
                    result.digitsProcessed = n;
                    result.piDigits = piStr;
                    result.replacedText = replaced;
                    break;
                }
            } else {
                if (currentSpaces > result.maxSpacesFound) {
                    result.maxSpacesFound = currentSpaces;
                }
                currentSpaces = 0;
            }
        }
        
        if (found) {
            std::cout << "\n✓ Found " << targetSpaces << "+ consecutive spaces at position " 
                      << result.position << "!" << std::endl;
            std::cout << "  Total digits processed: " << n << std::endl;
            std::cout << "  Maximum consecutive spaces found: " << result.maxSpacesFound << std::endl;
            return result;
        } else {
            std::cout << "  Processed " << n << " digits... max consecutive spaces so far: " 
                      << result.maxSpacesFound << std::endl;
            n += chunkSize;
        }
    }
    
    if (!found) {
        std::cout << "\n✗ Did not find " << targetSpaces << " consecutive spaces in first " 
                  << maxDigits << " digits" << std::endl;
        std::cout << "  Maximum consecutive spaces found: " << result.maxSpacesFound << std::endl;
        
        // Return what we have
        result.digitsProcessed = n - chunkSize;
        result.piDigits = computePiDigits(result.digitsProcessed);
        result.replacedText = result.piDigits;
        std::replace(result.replacedText.begin(), result.replacedText.end(), '9', ' ');
        std::replace(result.replacedText.begin(), result.replacedText.end(), '0', ' ');
    }
    
    return result;
}

void colorizeAndSave(const std::string& text, const std::string& outputFile) {
    std::ofstream file(outputFile);
    
    std::string coloredText;
    
    for (char c : text) {
        std::string color = getColor(c);
        if (!color.empty()) {
            coloredText += color + c + COLOR_RESET;
            file << c;
        } else {
            coloredText += c;
            file << c;
        }
    }
    
    file.close();
    
    std::cout << "\nColorized display (first 500 characters):" << std::endl;
    if (coloredText.length() > 500) {
        std::cout << coloredText.substr(0, 500) << "..." << std::endl;
    } else {
        std::cout << coloredText << std::endl;
    }
    
    std::cout << "\nSaved colored digits (with spaces) to " << outputFile << std::endl;
}

int main() {
    std::cout << "============================================================" << std::endl;
    std::cout << "PI DIGITS PROCESSOR - SEARCH FOR 10 CONSECUTIVE SPACES" << std::endl;
    std::cout << "============================================================\n" << std::endl;
    
    // Process PI until we find 10 consecutive spaces
    ProcessResult result = processPiUntilTarget(10, 5000, 10000000);
    
    // Save original PI digits
    std::ofstream piFile("pi_digits.txt");
    piFile << result.piDigits;
    piFile.close();
    std::cout << "\nStep 1: Saved " << result.digitsProcessed << " digits of PI to pi_digits.txt" << std::endl;
    
    // Save replaced text
    std::ofstream replacedFile("pi_replaced.txt");
    replacedFile << result.replacedText;
    replacedFile.close();
    std::cout << "Step 2: Saved replaced text to pi_replaced.txt" << std::endl;
    
    // Colorize, display and save
    std::cout << "\nStep 3: Colorizing and saving..." << std::endl;
    colorizeAndSave(result.replacedText, "pi_colored.txt");
    
    std::cout << "\n============================================================" << std::endl;
    std::cout << "Processing complete!" << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "\nFiles created:" << std::endl;
    std::cout << "- pi_digits.txt (original " << result.digitsProcessed << " PI digits)" << std::endl;
    std::cout << "- pi_replaced.txt (with 9 and 0 replaced by spaces)" << std::endl;
    std::cout << "- pi_colored.txt (colored digits with spaces)" << std::endl;
    
    if (result.position > 0) {
        std::cout << "\n★ 10 consecutive spaces found at position " << result.position << "!" << std::endl;
    }
    
    return 0;
}
