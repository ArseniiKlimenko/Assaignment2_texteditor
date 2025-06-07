#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cctype>

#define Maxrows 100
#define MaxLenght_Line 1000

struct State {
    int AmountLines;
    char text[Maxrows][MaxLenght_Line];
};

class TexEditor {
    private:
    char text[Maxrows][MaxLenght_Line];
    int AmountLines;
    char copiedText[MaxLenght_Line];

    std::vector<State> undoStack;
    std::vector<State> redoStack;

    void SaveState() {
        State s;
        s.AmountLines = AmountLines;
        for (int i = 0; i < AmountLines; i++) {
            strcpy(s.text[i], text[i]);
        }
        undoStack.push_back(s);
        redoStack.clear();
    }

    void RestoreState(const State &s) {
        AmountLines = s.AmountLines;
        for (int i = 0; i < AmountLines; i++) {
            strcpy(text[i], s.text[i]);
        }
    }

    void deleteText(int line, int index, int num) {
        if (line < 0 || line >= AmountLines) return;
        int len = strlen(text[line]);
        if (index < 0 || index >= len) return;
        if (index + num > len) {
            num = len - index;
        }
        for (int i = index; i <= len - num; i++) {
            text[line][i] = text[line][i + num];
        }
    }

    void InsertReplacement(int line, int index, const char *NewText) {
        if (line < 0 || line >= AmountLines) return;
        int LenText = strlen(NewText);
        int LenLine = strlen(text[line]);
        if (index < 0 || index >= LenLine) return;
        char trans[MaxLenght_Line];
        strncpy(trans, text[line], index);
        trans[index] = '\0';
        strcat(trans, NewText);
        if (index + LenText < LenLine) {
            strcat(trans, text[line] + index + LenText);
        }
        if (strlen(trans) < MaxLenght_Line) {
            strcpy(text[line], trans);
        }else {
            std::cout << "Not enough space!" << std::endl;
        }
    }

    void Copy(int line, int index, int num) {
        if (line < 0 || line >= AmountLines) return;
        int len = strlen(text[line]);
        if (index < 0 || index >= len) return;
        if (index + num > len)
            num = len - index;
        strncpy(copiedText, text[line] + index, num);
        copiedText[num] = '\0';
    }

    void Paste(int line, int index) {
        if (line < 0 || line >= AmountLines) return;
        int len = strlen(text[line]);
        if (index < 0 || index > len) return;
        char trans[MaxLenght_Line];
        strncpy(trans, text[line], index);
        trans[index] = '\0';
        strcat(trans, copiedText);
        strcat(trans, text[line] + index);
        if (strlen(trans) < MaxLenght_Line) {
            strcpy(text[line], trans);
        } else {
            std::cout << "Not enough space for paste" << std::endl;
        }
    }

    char* encrypt(char* rawText, int key) {
        int len = strlen(rawText);
        for (int i = 0; i < len; i++) {
            char c = rawText[i];
            if(isalpha(c)) {
                if(isupper(c)) {
                    rawText[i] = char((c - 'A' + key) % 26 + 'A');
                }else {
                    rawText[i] = char((c - 'a' + key) % 26 + 'a');
                }
            }
        }
        return rawText;
    }

    char* decrypt(char* encryptedText, int key) {
        int len = strlen(encryptedText);
        for (int i = 0; i < len; i++) {
            char c = encryptedText[i];
            if(isalpha(c)) {
                if(isupper(c)) {
                    encryptedText[i] = char((c - 'A' - key + 26) % 26 + 'A');
                }else {
                    encryptedText[i] = char((c - 'a' - key +26) % 26 + 'a');
                }
            }
        }
        return encryptedText;
    }

public:
    TexEditor() {

        for (int i = 0; i < Maxrows; i++) {
            text[i][0] = '\0';
        }
        AmountLines = 1;
    }

    void run() {
        int comand;
        while (true) {
            printMenu();
            std::cout << "Enter the command: ";
            if (scanf("%d", &comand) != 1) {
                std::cout << "Invalid input!" << std::endl;
                while (getchar() != '\n');
                continue;
            }
            getchar();

            switch (comand) {
                case 0:
                    return;

                case 1: {
                    char line[MaxLenght_Line];
                    std::cout << "Write your text: ";
                    if (fgets(line, sizeof(line), stdin) == NULL) {
                        break;
                    }
                    line[strcspn(line, "\n")] = '\0';
                    SaveState();
                    if ((strlen(text[AmountLines - 1]) + strlen(line)) < MaxLenght_Line) {
                        strcat(text[AmountLines - 1], line);
                        std::cout << "Text added." << std::endl;
                    }
                    else {
                        std::cout << "Not enough space for text." << std::endl;
                    }
                    break;
                }

                case 2:
                    if (AmountLines < Maxrows) {
                        strcpy(text[AmountLines], "");
                        AmountLines++;
                        std::cout << "New line was started." << std::endl;
                    }
                    else {
                        std::cout << "Maximum of lines was reached." << std::endl;
                    }
                    break;

                case 3: {
                    char filename[250];
                    std::cout << "Write file name to save text in it: ";
                    if (fgets(filename, sizeof(filename), stdin) == NULL)
                        break;
                    filename[strcspn(filename, "\n")] = '\0';
                    FILE* fp = fopen(filename, "w");
                    if (fp == NULL) {
                        std::cout << "Error: can't save to the file." << std::endl;
                    }
                    else {
                        for (int i = 0; i < AmountLines; i++) {
                            fprintf(fp, "%s\n", text[i]);
                        }
                        fclose(fp);
                        std::cout << "Text was successfully saved." << std::endl;
                    }
                    break;
                }

                case 4: {
                    char filename[250];
                    std::cout << "Write file name for loading: ";
                    if (fgets(filename, sizeof(filename), stdin) == NULL)
                        break;
                    filename[strcspn(filename, "\n")] = '\0';
                    FILE* fp = fopen(filename, "r");
                    if (fp == NULL) {
                        std::cout << "Error: can't load from file." << std::endl;
                    }
                    else {
                        AmountLines = 0;
                        while (AmountLines < Maxrows &&
                               fgets(text[AmountLines], MaxLenght_Line, fp) != NULL) {
                            text[AmountLines][strcspn(text[AmountLines], "\n")] = '\0';
                            AmountLines++;
                               }
                        fclose(fp);
                        std::cout << "Text was loaded successfully." << std::endl;
                    }
                    break;
                }

                case 5:
                    std::cout << "\n-----CURRENT TEXT-----\n";
                    for (int i = 0; i < AmountLines; i++) {
                        std::cout << text[i] << std::endl;
                    }
                    std::cout << std::endl;
                    break;

                case 6: {
                    int line, index;
                    std::cout << "Choose line and index of the word (like: 0 6): ";
                    if (scanf("%d %d", &line, &index) != 2) {
                        std::cout << "Invalid input, try again!" << std::endl;
                        while (getchar() != '\n');
                        break;
                    }
                    getchar();
                    if (line < 0 || line >= AmountLines) {
                        std::cout << "Incorrect line number, try again!" << std::endl;
                        break;
                    }
                    char insrText[MaxLenght_Line];
                    std::cout << "Write text you want to insert: ";
                    if (fgets(insrText, sizeof(insrText), stdin) == NULL)
                        break;
                    insrText[strcspn(insrText, "\n")] = '\0';
                    if (strlen(text[line]) + strlen(insrText) >= MaxLenght_Line) {
                        std::cout << "Not enough space in the line!" << std::endl;
                        break;
                    }
                    SaveState();
                    char trans[MaxLenght_Line];
                    strncpy(trans, text[line], index);
                    trans[index] = '\0';
                    strcat(trans, insrText);
                    strcat(trans, text[line] + index);
                    strcpy(text[line], trans);
                    std::cout << "Text inserted successfully." << std::endl;
                    break;
                }

                case 7: {
                    char search[MaxLenght_Line];
                    std::cout << "Enter text or word to search: ";
                    if (fgets(search, sizeof(search), stdin) == NULL)
                        break;
                    search[strcspn(search, "\n")] = '\0';
                    if (strlen(search) == 0) {
                        std::cout << "No text entered. Please write at least one character!" << std::endl;
                        break;
                    }
                    int found = 0;
                    for (int i = 0; i < AmountLines; i++) {
                        char* p = text[i];
                        while ((p = strstr(p, search)) != NULL) {
                            int pos = p - text[i];
                            std::cout << "Text is present in this position: " << i << " " << pos << std::endl;
                            found = 1;
                            p++;
                        }
                    }
                    if (!found)
                        std::cout << "Text not found." << std::endl;
                    break;
                }

                case 8: {
                    int line, index, num;
                    std::cout << "Choose line, index and number of symbols (like: 0 0 1) you want to delete: "<<std::endl;
                    if (scanf("%d %d %d", &line, &index, &num) != 3) {
                        std::cout << "Invalid input, try again" << std::endl;
                        while (getchar() != '\n');
                        break;
                    }
                    getchar();
                    if (line < 0 || line >= AmountLines) {
                        std::cout << "Incorrect line number, try again" << std::endl;
                        break;
                    }
                    if (index < 0 || index >= (int)(strlen(text[index]))) {
                        std::cout << "Incorrect index number, try again" << std::endl;
                        break;
                    }
                    SaveState();
                    deleteText(line, index, num);
                    std::cout << "Text deleted." << std::endl;
                    break;
                }

                case 9: {
                    int line, index;
                    std::cout << "Write line and index for replacement(like: 0 1): ";
                    if (scanf("%d %d", &line, &index) != 2) {
                        std::cout << "Invalid input, try again" << std::endl;
                        while (getchar() != '\n');
                        break;
                    }
                    getchar();
                    if (line < 0 || line >= AmountLines) {
                        std::cout << "Incorrect line number, try again" << std::endl;
                        break;
                    }
                    char NewText[MaxLenght_Line];
                    std::cout << "Write text for replace: ";
                    if (fgets(NewText, sizeof(NewText), stdin) == NULL) {
                        std::cout << "No text entered." << std::endl;
                        break;
                    }
                    NewText[strcspn(NewText, "\n")] = '\0';
                    SaveState();
                    InsertReplacement(line, index, NewText);
                    std::cout << "Text inserted." << std::endl;
                    break;
                }

                case 10: {
                    int line, index, num;
                    std::cout << "Choose text you want to copy(like: 0 0 1 line, index, number: ";
                    if (scanf("%d %d %d", &line, &index, &num) != 3) {
                        std::cout << "Invalid input, try again" << std::endl;
                        while (getchar() != '\n');
                        break;
                    }
                    getchar();
                    if (line < 0 || line >= AmountLines) {
                        std::cout << "Incorrect line number, try again" << std::endl;
                        break;
                    }
                    Copy(line, index, num);
                    std::cout << "Text copied" << std::endl;
                    break;
                }

                case 11: {
                    int line, index;
                    std::cout << "Choose line and index to paste (like: 0 5): ";
                    if (scanf("%d %d", &line, &index) != 2) {
                        std::cout << "Invalid input, try again" << std::endl;
                        while (getchar() != '\n');
                        break;
                    }
                    getchar();
                    if (line < 0 || line >= AmountLines) {
                        std::cout << "Incorrect line number, try again" << std::endl;
                        break;
                    }
                    if (index < 0 || index > (int)strlen(text[line])) {
                        std::cout << "Incorrect index number, try again" << std::endl;
                        break;
                    }
                    SaveState();
                    Paste(line, index);
                    std::cout << "Text pasted." << std::endl;
                    break;
                }


                case 12: {
                    int line, index, num;
                    std::cout << "Choose text you want to cut(like: 0 0 1 line index number of symbols):   ";
                    if (scanf("%d %d %d", &line, &index, &num) != 3) {
                        std::cout << "Invalid input, try again" << std::endl;
                        while (getchar() != '\n');
                        break;
                    }
                    getchar();
                    if (line < 0 || line >= AmountLines) {
                        std::cout << "Incorrect line number, try again" << std::endl;
                        break;
                    }
                    Copy(line, index, num);
                    SaveState();
                    deleteText(line, index, num);
                    std::cout << "Text was cutted" << std::endl;
                    break;
                }

                case 13: {
                    if (undoStack.empty()) {
                        std::cout << "Nothing to undo" << std::endl;
                    } else {
                        State current;
                        current.AmountLines = AmountLines;
                        for (int i = 0; i < AmountLines; i++) {
                            strcpy(current.text[i], text[i]);
                        }
                        redoStack.push_back(current);
                        State prev = undoStack.back();
                        undoStack.pop_back();
                        RestoreState(prev);
                        std::cout << "Undo success." << std::endl;
                    }
                    break;
                }

                case 14: {
                    if (redoStack.empty()) {
                        std::cout << "Nothing to redo." << std::endl;
                    } else {
                        State current;
                        current.AmountLines = AmountLines;
                        for (int i = 0; i < AmountLines; i++) {
                            strcpy(current.text[i], text[i]);
                        }
                        undoStack.push_back(current);
                        State next = redoStack.back();
                        redoStack.pop_back();
                        RestoreState(next);
                        std::cout << "Redo success." << std::endl;
                    }
                    break;
                }

                case 15: {
                    char input[MaxLenght_Line];
                    std::cout << "Enter text to encrypt: ";
                    if (fgets(input, sizeof(input), stdin) == NULL) {
                        std::cout << "No text entered." << std::endl;
                        break;
                    }
                    std::cout << "Enter encrypt key." << std::endl;
                    int key;
                    if (scanf("%d", &key) != 1) {
                        std::cout << "Invalid key, try again" << std::endl;
                        break;
                    }
                    getchar();
                    std::cout << "Encrypted text: " << encrypt(input, key) << std::endl;
                    break;
                }

                case 16: {
                    char input[MaxLenght_Line];
                    std::cout << "Enter text to decrypt: ";
                    if (fgets(input, sizeof(input), stdin) == NULL) {
                        std::cout << "No text entered." << std::endl;
                        break;
                    }
                }




                default:
                    std::cout << "Command is not implemented, try again!" << std::endl;
                    break;
            }
        }
    }


    void printMenu() {
        std::cout << "\nChoose the command:" << std::endl;
        std::cout << "1. Append text symbols to the end" << std::endl;
        std::cout << "2. Start a new line" << std::endl;
        std::cout << "3. Save text to file" << std::endl;
        std::cout << "4. Load text from file" << std::endl;
        std::cout << "5. Print the current text to console" << std::endl;
        std::cout << "6. Insert text by line and symbol index" << std::endl;
        std::cout << "7. Search substring in text" << std::endl;
        std::cout << "8. Delete text" << std::endl;
        std::cout << "9. Insert with replacement" << std::endl;
        std::cout << "10. Copy" << std::endl;
        std::cout << "11. Paste" << std::endl;
        std::cout << "12. Cut" << std::endl;
        std::cout << "13. Undo" << std::endl;
        std::cout << "14. Redo" << std::endl;
        std::cout << "15. Encrypt" << std::endl;
        std::cout << "16. Decrypt" << std::endl;
        std::cout << "0. Exit" << std::endl;
    }
};

int main() {
    TexEditor editor;
    editor.run();
    return 0;
}