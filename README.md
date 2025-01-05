This is a simple one file program I wrote to encrypt a text file using the key phrase method.

Make sure you are using C++17+ when compiling. I compiled it with C++20 when testing, so if you're having problems try that.

The key phrase for the example.txt file is "lonestar." Give ./example.txt for the path, then tell it to "read," then enter "lonestar."

NEVER use the same character twice.

The way it's set up out of the box, it will accept all 26 letters of the English alphabet, the numbers 0-9, and the special characters ,./`"&()%#-

If you want to add more, just add them to the alphabet. Just keep in mind that unknown characters will show up as "$", so if you use that character in your text, it may get confusing. If you want to change the unknown character symbol, you can change the variable near the top of the file. Also keep in mind that if you encrypt a file with one alphabet, and then add letters to the front of the alphabet, it won't decrypt the file correctly.