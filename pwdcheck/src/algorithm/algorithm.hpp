#pragma once
#include <string>
#include <iostream>
#include <windows.h>
#include <random>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>


using namespace std;

enum class PasswordStrength { VeryWeak, Weak, Moderate, Strong, VeryStrong, Insane };

struct StrengthRequirements {
    bool hasLower = false;
    bool hasUpper = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;
    bool hasPalindrome = false;
    bool hasSequence = false;
    bool hasRepeatedChars = false;
    bool hasKeyboardPattern = false;
};

namespace algorithm
{
    // Function to check character types in the password
    void checkCharacterTypes( const string& password, StrengthRequirements& requirements );

    // Function to check additional criteria in the password
    void checkAdditionalCriteria( const string& password, int length, StrengthRequirements& requirements );

    // Function to determine password strength
    PasswordStrength getPasswordStrength( const string& password, StrengthRequirements& requirements );

    string generateStrongPassword( );

    // Function to generate MD5 hash of a string
    string md5( const string& password );

    // Function to generate SHA-1 hash of a string
    string sha1( const string& password );
}