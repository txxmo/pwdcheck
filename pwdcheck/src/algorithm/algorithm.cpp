#include "algorithm.hpp"

#include <iomanip>
#include <sstream>

namespace algorithm
{
    // Function to check character types in the password
    void checkCharacterTypes( const string& password, StrengthRequirements& requirements ) 
    {
        for ( char c : password ) 
        {
            if ( islower( c ) )
                requirements.hasLower = true;
            else if ( isupper( c ) )
                requirements.hasUpper = true;
            else if ( isdigit( c ) )
                requirements.hasDigit = true;
            else
                requirements.hasSpecialChar = true;
        }
    }

    // Function to check additional criteria in the password
    void checkAdditionalCriteria( const string& password, int length, bool& hasRepeatedChars, bool& hasSequence, bool& hasPalindrome, bool& hasKeyboardPattern ) 
    {
        const string keyboardPatterns[ ] = { "qwerty", "asdfgh", "zxcvbn", "123456", "654321", "987654", "password", "admin", "123123" };
        for ( int i = 0; i < length - 1; ++i ) 
        {
            if ( password[ i ] == password[ i + 1 ] )
                hasRepeatedChars = true;

            if ( i < length - 2 && ( password[ i ] + 1 == password[ i + 1 ] && password[ i ] + 2 == password[ i + 2 ] ) )
                hasSequence = true;

            if ( i < length - 1 && password[ i ] == password[ i + 1 ] )
                hasPalindrome = true;

            for ( const string& pattern : keyboardPatterns ) 
            {
                if ( password.find( pattern ) != string::npos ) 
                {
                    hasKeyboardPattern = true;
                    break;
                }
            }
        }
    }

    PasswordStrength getPasswordStrength( const string& password, StrengthRequirements& requirements ) {
        int length = password.length( );

        // Check if all character types are present
        bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecialChar = false;
        for ( char c : password ) 
        {
            if ( islower( c ) ) hasLower = true;
            else if ( isupper( c ) ) hasUpper = true;
            else if ( isdigit( c ) ) hasDigit = true;
            else if ( !isspace( c ) ) hasSpecialChar = true; // Exclude whitespace from special characters
        }

        // Update requirements
        requirements.hasLower = hasLower;
        requirements.hasUpper = hasUpper;
        requirements.hasDigit = hasDigit;
        requirements.hasSpecialChar = hasSpecialChar;

        // Check if password meets additional criteria
        bool hasRepeatedChars = false, hasSequence = false, hasPalindrome = false, hasKeyboardPattern = false;
        checkAdditionalCriteria( password, length, hasRepeatedChars, hasSequence, hasPalindrome, hasKeyboardPattern );

        requirements.hasRepeatedChars = hasRepeatedChars;
        requirements.hasSequence = hasSequence;
        requirements.hasPalindrome = hasPalindrome;
        requirements.hasKeyboardPattern = hasKeyboardPattern;

        // Determine password strength based on requirements and additional criteria
        if ( hasLower && hasUpper && hasDigit && hasSpecialChar &&
            !hasRepeatedChars && !hasSequence && !hasPalindrome && !hasKeyboardPattern && length >= 24 )
            return PasswordStrength::Insane;
        else if ( hasLower && hasUpper && hasDigit && hasSpecialChar &&
            !hasRepeatedChars && !hasSequence && !hasPalindrome && !hasKeyboardPattern && length >= 20 )
            return PasswordStrength::Insane;
        else if ( hasLower && hasUpper && hasDigit && hasSpecialChar &&
            !hasRepeatedChars && !hasSequence && !hasPalindrome && length >= 16 )
            return PasswordStrength::VeryStrong;
        else if ( hasLower && hasUpper && hasDigit && hasSpecialChar &&
            !hasRepeatedChars && !hasKeyboardPattern && length >= 12 )
            return PasswordStrength::Strong;
        else if ( ( hasLower || hasUpper ) && hasDigit && hasSpecialChar &&
            !hasRepeatedChars && !hasKeyboardPattern && length >= 10 )
            return PasswordStrength::Moderate;
        else if ( ( hasLower || hasUpper ) && hasSpecialChar && !hasRepeatedChars && length >= 8 )
            return PasswordStrength::Weak;
        else
            return PasswordStrength::VeryWeak;
    }

    string generateStrongPassword( ) {
        const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+";
        const int minLength = 24; // Increase minimum length
        const int minLowercase = 2; // Increase minimum lowercase characters
        const int minUppercase = 2; // Increase minimum uppercase characters
        const int minDigit = 2; // Increase minimum digit characters
        const int minSpecialChar = 2; // Increase minimum special characters

        random_device rd;
        mt19937 gen( rd( ) );
        uniform_int_distribution<int> distribution( 0, charset.length( ) - 1 );

        string password;
        PasswordStrength strength;
        StrengthRequirements requirements;

        int attempts = 0; // Add a counter for attempts

        // Generate password until it meets the requirements
        do 
        {
            password.clear( );
            int lowercaseCount = 0, uppercaseCount = 0, digitCount = 0, specialCharCount = 0;

            // Generate random password
            for ( int i = 0; i < minLength; ++i ) 
            {
                char c = charset[ distribution( gen ) ];
                password += c;
                if ( islower( c ) )
                    lowercaseCount++;
                else if ( isupper( c ) )
                    uppercaseCount++;
                else if ( isdigit( c ) )
                    digitCount++;
                else
                    specialCharCount++;
            }

            // Check the strength of the generated password
            strength = getPasswordStrength( password, requirements );

            // Debug
            cout << "Attempt " << attempts << ": Password = " << password << ", Strength = " << static_cast< int >( strength ) << endl;
            cout << "Requirements: Lower=" << requirements.hasLower << ", Upper=" << requirements.hasUpper
                << ", Digit=" << requirements.hasDigit << ", Special=" << requirements.hasSpecialChar << endl;

            attempts++; // Increment attempt counter
            if ( attempts > 1000 ) 
            {
                cout << "Exceeded maximum attempts" << endl;
                break; // Break loop if too many attempts
            }

        } while ( strength != PasswordStrength::Insane ); // Repeat until "Insane" strength is achieved

        return password;
    }

    // Function to generate MD5 hash of a string
    std::string md5( const std::string& password ) {
        EVP_MD_CTX* mdctx;
        const EVP_MD* md;
        unsigned char md_value[ EVP_MAX_MD_SIZE ];
        unsigned int md_len;

        md = EVP_get_digestbyname( "MD5" );
        if ( !md ) {
            // Handle error
            return "";
        }

        mdctx = EVP_MD_CTX_new( );
        if ( !mdctx ) {
            // Handle error
            return "";
        }

        EVP_DigestInit_ex( mdctx, md, nullptr );
        EVP_DigestUpdate( mdctx, password.c_str( ), password.length( ) );
        EVP_DigestFinal_ex( mdctx, md_value, &md_len );
        EVP_MD_CTX_free( mdctx );

        // Convert the binary MD5 hash to a hexadecimal string
        std::stringstream ss;
        for ( int i = 0; i < md_len; ++i ) {
            ss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << static_cast< unsigned int >( md_value[ i ] );
        }
        return ss.str( );
    }

    // Function to generate SHA-1 hash of a string
    string sha1( const string& password ) 
    {
        unsigned char digest[ SHA_DIGEST_LENGTH ];
        SHA1( ( const unsigned char* )password.c_str( ), password.length( ), digest );

        char shaString[ SHA_DIGEST_LENGTH * 2 + 1 ];
        for ( int i = 0; i < SHA_DIGEST_LENGTH; i++ )
            sprintf( &shaString[ i * 2 ], "%02x", ( unsigned int )digest[ i ] );

        return shaString;
    }
}