//
// Created by User on 23.06.2025.
//

#include "Line.h"

struct TextLine : Line {
    std::string text;
    TextLine(const std::string& t) : text(t) {}
    std::string serialize() const override {
        return "TEXT:" + text;
    }
    void print() const override {
        std::cout << text << "\n";
    }
    std::string getText() const override { return text; }
    void setText(const std::string& t) override { text = t; }
};

struct CheckLine : Line {
    std::string item;
    bool checked;
    CheckLine(const std::string& i, bool c) : item(i), checked(c) {}
    std::string serialize() const override {
        return "CHECK:" + (checked ? "1" : "0") + ":" + item;
    }
    void print() const override {
        std::cout << "[ " << (checked ? "x" : " ") << " ] " << item << "\n";
    }

};

