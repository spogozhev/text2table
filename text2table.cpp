#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Use:\n"
                  << "    text2table source destination\n";
        return 1;
    }

    std::ifstream inFile(argv[1]);
    if (!inFile) {
        std::cout << "Can not open file '" << argv[1] << "'\n";
        return 2;
    }

    // 1. Пройти по файлу и подсчитать ширины столбцов.
    int lenwords[200] = { 0 };
    int wordlen = 0;
    int wordnum = 0;
    int ch;
    while (  EOF != (ch = inFile.get()) ) {
        if (' '==ch || '\t'==ch) {
            if (wordlen != 0) {
                if (lenwords[wordnum] < wordlen) {
                    lenwords[wordnum] = wordlen;
                }
                wordlen = 0;
                ++wordnum;
            }
        }
        else if ('\n' == ch) {
            if (lenwords[wordnum] < wordlen) {
                lenwords[wordnum] = wordlen;
            }
            wordlen = 0;
            wordnum = 0;
        }
        else {
            ++wordlen;
        }
    }
    if (wordlen != 0) {
        if (lenwords[wordnum] < wordlen) {
            lenwords[wordnum] = wordlen;
        }
    }

#if 0
    for (int i = 0; i < 200; ++i)
        std::cout << lenwords[i] << " ";
#endif

    // 2. Пройти по файлу и добавлять пробелы к словам так,
    //    чтобы ширина слова с пробелами стала равна ширине столбца 
    inFile.clear();
    inFile.seekg(0);
    
    std::ofstream outFile(argv[2]);
    if (!outFile.is_open()) {
        std::cout << "Can not write to file '" << argv[2] << "'\n";
        inFile.close();
        return 3;
    }

    wordnum = 0;
    wordlen = 0;
    int charcounter = 0;
    while (EOF != (ch = inFile.get())) {
        if (' ' == ch || '\t' == ch) {
            if (wordlen != 0) {
                ++wordnum;
                wordlen = 0;
            }
        }
        else if ('\n' == ch) {
            outFile.put('\n');
            wordnum = 0;
            wordlen = 0;
            charcounter = 0;
        }
        else {
            if (wordlen == 0 && wordnum > 0) { // Вставить пробелы перед словом
                while (charcounter <= lenwords[wordnum-1]) {
                    outFile.put(' ');
                    ++charcounter;
                }
                charcounter = 0;
            }
            outFile.put(ch);
            ++wordlen;
            ++charcounter;
        }
    }
    inFile.close();
    outFile.close();
    return 0;
}
