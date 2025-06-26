
#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include "Text.h"
#include "Line.h"
#include "Caesarload.h"

class TextEditor {
    int Key = 0;
    Text text;
    std::vector<std::vector<std::string>> undoStack;
    std::vector<std::vector<std::string>> redoStack;
    std::string temp;

    void saveState() {
        undoStack.push_back(text.state());
        redoStack.clear();
    }
    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }

    void Undo() {
        if (!canUndo())
            return;
        redoStack.push_back(text.state());
        auto prev = undoStack.back();
        undoStack.pop_back();
        text.restore(prev);
    }

    void Redo() {
        if (!canRedo())
            return;
        undoStack.push_back(text.state());
        auto next = redoStack.back();
        redoStack.pop_back();
        text.restore(next);
    }

    void Delete(int ln, int idx, int cnt) {
        if (ln<0||ln>=int(text.size()))
            return;
        auto t = dynamic_cast<TextLine*>(text.get(ln));
        if (!t)
            return;
        std::string& s = t->text;
        if (idx<0||idx>int(s.size()))
            return;
        s.erase(idx, cnt);
    }

    void Insert(int ln, int idx, const std::string& ins) {
        if (ln<0||ln>=int(text.size()))
            return;
        auto t = dynamic_cast<TextLine*>(text.get(ln));
        if (!t)
            return;
        std::string& s = t->text;
        if (idx<0||idx>int(s.size()))
            return;
        s.insert(idx, ins);
    }

    void Copy(int ln, int idx, int cnt) {
        temp.clear();
        if (ln<0||ln>=int(text.size()))
            return;
        auto t = dynamic_cast<TextLine*>(text.get(ln));
        if (!t) return;
        const std::string& s = t->text;
        if (idx<0||idx>int(s.size())) return;
        temp = s.substr(idx, cnt);
    }
    void Paste(int ln, int idx) {
        if (ln<0||ln>=int(text.size()))
            return;
        auto t = dynamic_cast<TextLine*>(text.get(ln));
        if (!t) return;
        std::string& s = t->text;
        if (idx<0||idx>int(s.size()))
            return;
        s.insert(idx, temp);
    }
    void Search(const std::string& pat) {
        for (int i = 0; i < int(text.size()); ++i) {
            auto t = dynamic_cast<TextLine*>(text.get(i));
            if (!t) continue;
            const std::string& s = t->text;
            size_t pos = 0;
            while ((pos = s.find(pat, pos)) != std::string::npos) {
                std::cout << "Found at line " << i << ", index " << pos << "\n";
                ++pos;
            }
        }
    }

    void Cut(int ln, int idx, int cnt) {
        temp.clear();
        if (ln < 0 || ln >= int(text.size()))
            return;
        auto t = dynamic_cast<TextLine*>(text.get(ln));
        if (!t) return;
        std::string& s = t->text;
        if (idx < 0 || idx + cnt > int(s.size())) return;
        temp = s.substr(idx, cnt);
        s.erase(idx, cnt);
    }

    void InsertwithReplacment(int ln, int idx, int cnt, const std::string& ins) {
        if (ln < 0 || ln >= int(text.size()))
            return;
        auto t = dynamic_cast<TextLine*>(text.get(ln));
        if (!t) return;
        std::string& s = t->text;
        if (idx < 0 || idx + cnt > int(s.size())) return;
        s.replace(idx, cnt, ins);
    }



    void PrintMenu() const {
        std::cout << "\n=== Text Editor ===\n"
                  << " 1) Load file\n"
                  << " 2) Save file\n"
                  << " 3) Print text\n"
                  << " 4) Append text line\n"
                  << " 5) Append checklist line\n"
                  << " 6) Append contact line\n"
                  << " 7) New empty text line\n"
                  << " 8) Insert text\n"
                  << " 9) Delete text\n"
                  << "10) Copy text\n"
                  << "11) Paste text\n"
                  << "12) Cut text\n"
                  << "13) Insert with replacement\n"
                  << "14) Search substring\n"
                  << "15) Undo\n"
                  << "16) Redo\n"
                  << "17) Encrypt whole text\n"
                  << "18) Decrypt whole text\n"
                  << "19) Save encrypted\n"
                  << "20) Load encrypted\n"
                  << " 0) Exit\n"
                  << "Choice: ";
    }


public:
    void run() {
        while (true) {
            PrintMenu();
            int cmd;
            if (!(std::cin >> cmd)) {
                std::cin.clear();
                std::cin.ignore(1e6, '\n');
                continue;
            }
            std::cin.ignore(1e6, '\n');

            if (cmd == 0) break;

            switch (cmd) {

                case 1: {
                    std::string f;
                    std::cout << "File to load: ";
                    std::getline(std::cin, f);
                    if (text.LoadFromFile(f))
                        std::cout << "Loaded\n";
                    else
                        std::cout << "Load failed\n";
                    break;
                }
                case 2: {
                    std::string f;
                    std::cout << "File to save: ";
                    std::getline(std::cin, f);
                    if (text.SaveToFile(f))
                        std::cout << "Saved\n";
                    else
                        std::cout << "Save failed\n";
                    break;
                }

                case 3: {
                    std::cout << "\n--- Text ---\n";
                    text.printAll();
                    break;
                }

                case 4: {
                    std::string s;
                    std::cout << "Enter text: ";
                    std::getline(std::cin, s);
                    saveState();
                    text.add(new TextLine(s));
                    break;
                }

                case 5: {
                    std::string itm; int fl;
                    std::cout << "Item: ";
                    std::getline(std::cin, itm);
                    std::cout << "Checked? ";
                    std::cin >> fl;
                    std::cin.ignore(1e6,'\n');
                    saveState();
                    text.add(new CheckLine(itm, fl!=0));
                    break;
                }

                case 6: {
                    std::string n,s,e;
                    std::cout<<"Name: ";
                    std::getline(std::cin,n);
                    std::cout<<"Surname: ";
                    std::getline(std::cin,s);
                    std::cout<<"Email: ";
                    std::getline(std::cin,e);
                    saveState();
                    text.add(new ContactLine(n,s,e));
                    break;
                }

                case 7: {
                    saveState();
                    text.add(new TextLine(""));
                    break;
                }

                case 8: {
                    int ln,idx; std::cout<<"Line, idx: ";
                    std::cin>>ln>>idx;
                    std::cin.ignore(1e6,'\n');
                    std::string ins;
                    std::cout<<"Text to insert: ";
                    std::getline(std::cin, ins);
                    saveState();
                    Insert(ln, idx, ins);
                    break;
                }

                case 9: {
                    int ln,idx,cnt;
                    std::cout<<"Line, idx, count: ";
                    std::cin>>ln>>idx>>cnt;
                    std::cin.ignore(1e6,'\n');
                    saveState();
                    Delete(ln, idx, cnt);
                    break;
                }

                case 10: {
                    int ln,idx,cnt;
                    std::cout<<"Line, idx, count: ";
                    std::cin>>ln>>idx>>cnt;
                    std::cin.ignore(1e6,'\n');
                    Copy(ln, idx, cnt);
                    break;
                }

                case 11: {
                    int ln,idx;
                    std::cout<<"Line, idx: ";
                    std::cin>>ln>>idx;
                    std::cin.ignore(1e6,'\n');
                    saveState();
                    Paste(ln, idx);
                    break;
                }

                case 12: {
                    int ln, idx, cnt;
                    std::cout << "Line, idx, count: ";
                    std::cin >> ln >> idx >> cnt;
                    std::cin.ignore(1e6, '\n');
                    saveState();
                    Cut(ln, idx, cnt);
                    break;
                }

                case 13: {
                    int ln, idx, cnt;
                    std::cout << "Line, idx, count to replace: ";
                    std::cin >> ln >> idx >> cnt;
                    std::cin.ignore(1e6, '\n');
                    std::string ins;
                    std::cout << "Text to insert: ";
                    std::getline(std::cin, ins);
                    saveState();
                    InsertwithReplacment(ln, idx, cnt, ins);
                    break;
                }

                case 14: {
                    std::string pat;
                    std::cout << "Text to search: ";
                    std::getline(std::cin, pat);
                    Search(pat);
                    break;
                }

                case 15:
                    if (canUndo()) {
                        Undo();
                        std::cout << "Undone\n";
                    } else {
                        std::cout << "Nothing to undo\n";
                    }
                    break;

                case 16:
                    if (canRedo()) {
                        Redo();
                        std::cout << "Redone\n";
                    } else {
                        std::cout << "Nothing to redo\n";
                    }
                    break;

                case 17: {
                    std::cout << "Enter key: ";
                    if (!(std::cin >> Key)) {
                        std::cin.clear();
                        std::cin.ignore(1e6, '\n');
                        std::cout << "Invalid key\n";
                        break;
                    }
                    std::cin.ignore(1e6, '\n');
                    Caesarload load("C:/Users/User/CLionProjects/Assaignment2_texteditor/cmake-build-debug/caesar1.dll", Key);
                    if (!load.valid()) break;
                    saveState();
                    auto stat = text.state();
                    text.clear();

                    for (auto& full : stat) {
                        size_t sep = full.find(':');
                        if (sep == std::string::npos) {
                            text.add(new TextLine(full));
                            continue;
                        }
                        std::string prefix = full.substr(0, sep + 1);
                        std::string data   = full.substr(sep + 1);
                        if (prefix == "CHECK:") {
                            size_t sub = data.find(':');
                            if (sub != std::string::npos) {
                                std::string flag = data.substr(0, sub + 1);
                                std::string item = data.substr(sub + 1);
                                std::string enc  = load.encrypt(item);
                                text.add(Line::deserialize(prefix + flag + enc));
                                continue;
                            }
                        }
                        std::string enc = load.encrypt(data);
                        text.add(Line::deserialize(prefix + enc));
                    }

                    std::cout << "Encrypted with key = " << Key << "\n";
                    break;
                }


                case 18: {
                    if (Key == 0) {
                        std::cout << "No key set. First encrypt to set key.\n";
                        break;
                    }
                    Caesarload load("C:/Users/User/CLionProjects/Assaignment2_texteditor/cmake-build-debug/caesar1.dll", Key);
                    if (!load.valid()) break;
                    saveState();
                    auto stat = text.state();
                    text.clear();
                    for (auto& full : stat) {
                        size_t sep = full.find(':');
                        if (sep == std::string::npos) {
                            text.add(new TextLine(full));
                            continue;
                        }

                        std::string prefix = full.substr(0, sep + 1);
                        std::string data = full.substr(sep + 1);

                        if (prefix == "CHECK:") {
                            size_t sub = data.find(':');
                            if (sub != std::string::npos) {
                                std::string flag = data.substr(0, sub + 1);
                                std::string enc = data.substr(sub + 1);
                                std::string dec = load.decrypt(enc);
                                text.add(Line::deserialize(prefix + flag + dec));
                                continue;
                            }
                        }
                        std::string dec = load.decrypt(data);
                        text.add(Line::deserialize(prefix + dec));
                    }

                    std::cout << "Decrypted with key = " << Key << "\n";
                    break;
                }


                case 19: {
                    std::string f;
                    std::cout << "File to save (encrypted): ";
                    std::getline(std::cin, f);
                    int key1;
                    std::cout << "Enter key: ";
                    if (!(std::cin >> key1)) {
                        std::cin.clear();
                        std::cin.ignore(1e6, '\n');
                        std::cout << "Invalid key\n";
                        break;
                    }
                    std::cin.ignore(1e6, '\n');
                    Caesarload load("C:/Users/User/CLionProjects/Assaignment2_texteditor/cmake-build-debug/caesar1.dll", key1);
                    if (!load.valid()) break;
                    std::ofstream out(f);
                    if (!out) {
                        std::cout << "Failed to open file\n";
                        break;
                    }
                    for (size_t i = 0; i < text.size(); ++i) {
                        Line* line = text.get(i);
                        std::string full = line->serialize();
                        size_t sep = full.find(':');
                        if (sep == std::string::npos) {
                            out << full << "\n";
                            continue;
                        }
                        std::string prefix = full.substr(0, sep + 1);
                        std::string data   = full.substr(sep + 1);
                        if (prefix == "CHECK:") {
                            size_t sub = data.find(':');
                            if (sub != std::string::npos) {
                                std::string flag = data.substr(0, sub + 1);
                                std::string item = data.substr(sub + 1);
                                out << prefix << flag << load.encrypt(item) << "\n";
                            } else {
                                out << full << "\n";
                            }
                        } else if (prefix == "CONTACT: ") {
                            out << prefix << load.encrypt(data) << "\n";
                        } else if (prefix == "TEXT:") {
                            out << prefix << load.encrypt(data) << "\n";
                        } else {
                            out << full << "\n";
                        }
                    }

                    std::cout << "Saved encrypted.\n";
                    break;
                }

                case 20: {
                    std::string f;
                    std::cout << "File to load (encrypted): ";
                    std::getline(std::cin, f);
                    int key1;
                    std::cout << "Enter key: ";
                    if (!(std::cin >> key1)) {
                        std::cin.clear();
                        std::cin.ignore(1e6, '\n');
                        std::cout << "Invalid key\n";
                        break;
                    }
                    std::cin.ignore(1e6, '\n');
                    Caesarload load("C:/Users/User/CLionProjects/Assaignment2_texteditor/cmake-build-debug/caesar1.dll", key1);
                    if (!load.valid()) break;
                    std::ifstream in(f);
                    if (!in) {
                        std::cout << "Failed to open file\n";
                        break;
                    }
                    text.clear();
                    std::string line;
                    while (std::getline(in, line)) {
                        size_t sep = line.find(':');
                        if (sep == std::string::npos) {
                            text.add(new TextLine(line));
                            continue;
                        }
                        std::string prefix = line.substr(0, sep + 1);
                        std::string data   = line.substr(sep + 1);

                        if (prefix == "CHECK:") {
                            size_t sub = data.find(':');
                            if (sub != std::string::npos) {
                                std::string flag = data.substr(0, sub + 1);
                                std::string item = data.substr(sub + 1);
                                std::string decryptedItem = load.decrypt(item);
                                text.add(Line::deserialize(prefix + flag + decryptedItem));
                            } else {
                                text.add(Line::deserialize(line));
                            }
                        } else {
                            std::string decrypted = load.decrypt(data);
                            text.add(Line::deserialize(prefix + decrypted));
                        }
                    }
                    std::cout << "Loaded and decrypted.\n";
                    break;
                }


                default:
                    std::cout<<"Unknown command\n";
            }
        }
    }
};



#endif //TEXTEDITOR_H
