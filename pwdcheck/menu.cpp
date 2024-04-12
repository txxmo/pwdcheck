#include "menu.h"
#include "globals.h"

#include "src/algorithm/algorithm.hpp"
#include "src/window/window.hpp"

using namespace std;
using namespace algorithm;

bool showExtras = false;

bool copyToClipboard( const std::string& text ) {
    if ( !OpenClipboard( nullptr ) ) {
        return false;
    }

    EmptyClipboard( );
    HGLOBAL hMem = GlobalAlloc( GMEM_MOVEABLE, text.size( ) + 1 );
    if ( !hMem ) {
        CloseClipboard( );
        return false;
    }

    memcpy( GlobalLock( hMem ), text.c_str( ), text.size( ) + 1 );
    GlobalUnlock( hMem );
    SetClipboardData( CF_TEXT, hMem );
    CloseClipboard( );
    return true;
}

void ImGuiPasswordChecker( string& password ) {
    static StrengthRequirements requirements;

    // Get password strength and requirements
    PasswordStrength strength = getPasswordStrength( password, requirements );

    // Calculate progress
    float progress = 0.0f;
    switch ( strength ) {
    case PasswordStrength::VeryWeak:
        progress = 0.2f;
        break;
    case PasswordStrength::Weak:
        progress = 0.4f;
        break;
    case PasswordStrength::Moderate:
        progress = 0.6f;
        break;
    case PasswordStrength::Strong:
        progress = 0.8f;
        break;
    case PasswordStrength::VeryStrong:
        progress = 1.0f;
        break;
    case PasswordStrength::Insane:
        progress = 2.0f;
        break;
    }

    // Display password input field
    ImGui::Text( "Input Password: " );
    ImGui::SameLine( 246 );
    if ( ImGui::Checkbox( "Show Extras", &showExtras ) )
    {
        ImGui::SetWindowSize( ImVec2( 356, 420 ) );
    }
    ImGui::SetNextItemWidth( 340 );
    ImGui::InputText( "###PASS", &password );
    if ( ImGui::Button( "Copy", ImVec2( 340, 20 ) ) )
    {
        copyToClipboard( password.c_str( ) );
    }
    if ( ImGui::Button( "Generate", ImVec2( 340, 20 ) ) )
    {
        password = generateStrongPassword( );
    }
    
    if ( showExtras )
    {
        ImGui::Text( "MD5:" );
        ImGui::SetNextItemWidth( 340 );
        ImGui::InputText( "##MD5", const_cast< char* >( algorithm::md5( password ).c_str( ) ), MD5_DIGEST_LENGTH * 2 + 1, ImGuiInputTextFlags_ReadOnly );
        if ( ImGui::Button( "Copy ###COPYMD5", ImVec2( 340, 20 ) ) )
        {
            copyToClipboard( algorithm::md5( password ).c_str( ) );
        }

        ImGui::Text( "SHA-1:" );
        ImGui::SetNextItemWidth( 340 );
        ImGui::InputText( "##SHA1", const_cast< char* >( algorithm::sha1( password ).c_str( ) ), SHA_DIGEST_LENGTH * 2 + 1, ImGuiInputTextFlags_ReadOnly );
        if ( ImGui::Button( "Copy ###COPYSHA", ImVec2( 340, 20 ) ) )
        {
            copyToClipboard( algorithm::sha1( password ).c_str( ) );
        }
    }

    ImGui::Separator( );

    // Display password strength
    const char* strengthText = "";
    ImVec4 strengthColor = ImColor( 255, 255, 255, 255 ); // Default to white
    switch ( strength ) {
    case PasswordStrength::VeryWeak:
        strengthText = "Very Weak";
        strengthColor = ImColor( 255, 59, 59, 255 ); // Red for very weak
        break;
    case PasswordStrength::Weak:
        strengthText = "Weak";
        strengthColor = ImColor( 255, 147, 59, 255 ); // Orange for weak
        break;
    case PasswordStrength::Moderate:
        strengthText = "Moderate";
        strengthColor = ImColor( 255, 226, 59, 255 ); // Yellow for moderate
        break;
    case PasswordStrength::Strong:
        strengthText = "Strong";
        strengthColor = ImColor( 134, 255, 59, 255 ); // Green for strong
        break;
    case PasswordStrength::VeryStrong:
        strengthText = "Very Strong";
        strengthColor = ImColor( 59, 115, 255, 255 ); // Blue for very strong
        break;
    case PasswordStrength::Insane:
        strengthText = "HOLY SMOKES !!!!";
        strengthColor = ImColor( 255, 255, 255, 255 ); // Blue for very strong
        break;
    }
    
    ImGui::TextColored( strengthColor, "Password Strength: %s", strengthText );
    
    // Display progress bar
    ImGui::SetNextItemWidth( 340 );
    ImGui::ProgressBar( progress, ImVec2( 0.0f, 0.0f ) );

    if ( showExtras )
    {
        ImGui::Separator( );

        // Display password requirements
        ImGui::Text( "Password Requirements:" );

        const ImVec4 greenColor( 0.0f, 1.0f, 0.0f, 1.0f );
        const ImVec4 redColor( 1.0f, 0.0f, 0.0f, 1.0f );

        ImGui::BeginChild( "##Requirements", ImVec2( 0, 100 ), true );

        const char* requirementLabels[ ] = {
            "One lowercase character",
            "One uppercase character",
            "One digit",
            "One special character",
            "No palindrome",
            "No sequence",
            "No repeated characters",
            "No keyboard pattern"
        };

        bool requirementMet[ ] = {
            requirements.hasLower,
            requirements.hasUpper,
            requirements.hasDigit,
            requirements.hasSpecialChar,
            !requirements.hasPalindrome,
            !requirements.hasSequence,
            !requirements.hasRepeatedChars,
            !requirements.hasKeyboardPattern
        };

        // Display the requirements in a list box
        for ( int i = 0; i < IM_ARRAYSIZE( requirementLabels ); i++ ) {
            ImGui::Selectable( requirementLabels[ i ], false, ImGuiSelectableFlags_DontClosePopups );
            ImGui::SameLine( 240 );
            ImGui::TextColored( requirementMet[ i ] ? greenColor : redColor, requirementMet[ i ] ? "[X] Met" : "[-] Not met" );
        }

        ImGui::EndChild( );
    }
}

string password;

void PositionImGuiWindowAboveTaskbar( )
{
    // Get the taskbar information
    APPBARDATA appBarData;
    appBarData.cbSize = sizeof( APPBARDATA );
    SHAppBarMessage( ABM_GETTASKBARPOS, &appBarData );

    // Calculate window position above the taskbar
    int screenWidth = GetSystemMetrics( SM_CXSCREEN );
    int screenHeight = GetSystemMetrics( SM_CYSCREEN );
    int taskbarHeight = appBarData.rc.bottom - appBarData.rc.top;
    int windowWidth = 356; // Set your desired window width
    int windowHeight = showExtras ? 425 : 170; // Set your desired window height
    int offsetX = 10; // Set the horizontal offset from the right edge of the screen
    int offsetY = 10; // Set the vertical offset from the bottom edge of the screen
    ImVec2 windowPos( screenWidth - windowWidth - offsetX, screenHeight - taskbarHeight - windowHeight - offsetY );

    // Set the ImGui window position
    ImGui::SetNextWindowPos( windowPos, ImGuiCond_Always );
}

void menu::render()
{
    // Close program upon pressing X.
    // console window is hidden upon start so this is required to exit
    if ( !global::active )
        exit( EXIT_SUCCESS );

    if ( !global::open )
        return;

    float windowHeight = showExtras ? 425 : 170;

    PositionImGuiWindowAboveTaskbar( );
    ImGui::SetNextWindowSize( ImVec2{ 356, windowHeight } );
    ImGui::Begin( "PWDCHECK", &global::active, 
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize );
    {
        // Begin password checking.
        ImGuiPasswordChecker( password );
    }
    ImGui::End( );
}