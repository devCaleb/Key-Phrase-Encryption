
/*

KEY PHRASE ENCRYPTION by Caleb Watson

*/


#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>

using namespace std;
using namespace std::filesystem;

vector<char> alphabet = {'0', '1',
'2', '3', '4', '5', '6', '7', '8', '9',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 
'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
'y', 'z', ',', '.', '/', '`', '"', '&',
'(', ')', '%', '#', '-'};

char unknownCharacterSymbol = '$';


map<char, string> createKey(string* phrase) {
    map<char, string> key;

    int x = 0; //place in the phrase
    int y = 0; //number of times looped through the phrase
    for (int i = 0; i < alphabet.size() - 1; i++) {
        string h = phrase->substr(x, 1);
        char letter = alphabet[i];

        if (y > 0) {
            int offset = 4;
            for (int z = 0; z < y; z++) { //For the number of times we've gone through the phrase
                for (int p = 0; p < phrase->length() - 1; p++) { //Loop through the phrase
                    if (p == z) {
                        if (z == 0) {
                            h = h + phrase->substr(p, 1);
                        }
                        else {
                            if (p + offset > phrase->length() - 1) {
                                int diff = (p + offset) - (phrase->length() - 1);

                                h = h + phrase->substr(diff, 1);
                            }
                            else {
                                h = h + phrase->substr(p + offset, 1);
                            }
                        }

                        break;
                    }
                }
            }
        }

        key.insert(pair<char, string>(letter, h));

        if (x >= phrase->length() - 1) {
            x = 0;
            y++;
        }
        else {
            x++;
        }
    }

    return key;
}

string hashChar(char c, map<char, string> key) {
    string character;

    auto it = key.find(c);
    if (it != key.end()) {
        character = it->second;
    }
    else {
        cout << "ERROR: `" << c << "` not found in alphabet." << endl;
    }

    return character;
}

string unHashString(string t, map<char, string> key) {
    char character = unknownCharacterSymbol; //If it cannot find the character, you will be left with this.

    for (const auto& pair : key) {
        if (pair.second == t) {
            character = pair.first;
            break;
        }
    }

    return string(1, character);
}

void writeFile(string location, vector<string> lines) {
    ofstream file(location, std::ofstream::out | std::ofstream::trunc);

    for (string l : lines) {
        file << l << endl;
    }

    file.close();
}

void EncryptFile(string location, string phrase) {
    map<char, string> key = createKey(&phrase);
    ifstream file(location);

    vector<string> lines;
    while (file) {
        string l;
        string hashedLine;

        getline(file, l);

        //Make the whole line lowercase
        transform(l.begin(), l.end(), l.begin(),
            [](unsigned char c) { return tolower(c); });

        for (char c : l) {
            isspace(c) ? hashedLine.append("[*] ") : hashedLine.append(hashChar(c, key) + " ");
        }

        lines.push_back(hashedLine);
    }

    file.close();

    writeFile(location, lines);

    cout << "Encryption Complete!" << endl;
}

vector<string> DecryptFile(string location, string phrase) {
    map<char, string> key = createKey(&phrase);
    ifstream file(location);

    vector<string> lines;
    while (file) {
        string l;
        string unhashedLine;

        getline(file, l);

        string token;
        for (int i = 0; i < l.length() - 1; i++) {
            if (i + 1 >= l.length())
                break;

            if (l.substr(i, 1) == " ") {
                unhashedLine += unHashString(token, key);
                token = "";
                continue;
            }

            if (i == l.length() - 2) { //end of line
                token += l.substr(i, 1);
                unhashedLine += unHashString(token, key);
                token = "";
                continue;
            }

            if (l.substr(i, 3) == "[*]") {
                unhashedLine = unhashedLine + " ";
                i += 3;
                continue;
            }

            token += l.substr(i, 1);
        }

        lines.push_back(unhashedLine);
    }

    file.close();

    return lines;
}

int main()
{
    string fileLocation;
    string keyPhrase;
    string input;
    
    cout << "Key Phrase Encryption" << endl;

    cout << "Path of file:" << endl;
    std::getline(cin, fileLocation);

    cout << "What would you like to do with the file? (encrypt/decrypt/read)" << endl;
    cin >> input;

    transform(input.begin(), input.end(), input.begin(),
        [](unsigned char c) { return tolower(c); });

    cout << "Key Phrase (minimum 8 letters, maximum " << alphabet.size() << " letters):" << endl;
    cin >> keyPhrase;

    if (keyPhrase.length() < 8 || keyPhrase.length() > alphabet.size()) {
        cout << "The phrase you entered was not the correct length." << endl;
        cout << "Key phrase must be between 8 and " << alphabet.size() << " letters" << endl;
        cout << "Please try again: " << endl;

        while (cin >> input) {
            if (input.length() < 8 || input.length() > alphabet.size()) {
                cout << "The phrase you entered was not the correct length." << endl;
                cout << "Key phrase must be between 8 and " << alphabet.size() << " letters" << endl;
                cout << "Please try again: " << endl;
            }
            else {
                keyPhrase = input;
                break;
            }
        }
    }

    if (input == "encrypt") {
        EncryptFile(fileLocation, keyPhrase);
    } else if (input == "decrypt") {
        writeFile(fileLocation, DecryptFile(fileLocation, keyPhrase));
        cout << "Decryption Complete!" << endl;
    } else if (input == "read") {
        vector<string> lines = DecryptFile(fileLocation, keyPhrase);

        cout << "\nFILE DECRYPTED:\n" << endl;

        for (string l : lines) {
            cout << l << endl;
        }
    }

    //Keep the window open until the user closes it
    while (cin >> input)
        break;

    return 0;
}

