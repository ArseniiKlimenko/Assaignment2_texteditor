

#ifndef LINE_H
#define LINE_H

#include <string>
#include <iostream>
#include <sstream>


struct Line {
    virtual ~Line() {}
    virtual std::string serialize() const = 0;
    virtual void print() const = 0;
    static Line * deserialize(const std::string& data);
    virtual std::string GetText() const
    {
        return "";
    }
    virtual void SetText(const std::string&){}

};

struct TextLine : Line {
    std::string text;
    TextLine(const std::string& t) : text(t) {}
    std::string serialize() const override {
        return "TEXT:" + text;
    }
    void print() const override {
        std::cout << text << "\n";
    }
    std::string GetText() const override { return text; }
    void SetText(const std::string& t) override { text = t; }
};

struct CheckLine : Line {
    std::string item;
    bool checked;
    CheckLine(const std::string& i, bool c) : item(i), checked(c) {}
    std::string serialize() const override {
        return std::string("CHECK:") + (checked ? "1" : "0") + ":" + item;
    }
    void print() const override {
        std::cout << "[ " << (checked ? "x" : " ") << " ] " << item << "\n";
    }
};

struct ContactLine : Line {
    std::string name, surname, email;
    ContactLine(const std::string& n, const std::string& s, const std::string& e) : name(n), surname(s), email(e) {}
    std::string serialize() const override {
        return "CONTACT: " + name + " " + surname + ", E-mail: " + email;
    }
    void print() const override {
        std::cout << name << " " << surname << ", E-mail: " << email << "\n";
    }
};

Line* Line::deserialize(const std::string& data) {
    if (data.rfind("TEXT:", 0) == 0) {
        return new TextLine(data.substr(5));
    }
    if (data.rfind("CHECK:", 0) == 0) {
        auto rest = data.substr(6);
        auto pos  = rest.find(':');
        bool chk  = (rest.substr(0,pos) == "1");
        std::string itm = rest.substr(pos+1);
        return new CheckLine(itm, chk);
    }
    if (data.rfind("CONTACT:", 0) == 0) {
        auto rest = data.substr(8);
        std::istringstream ss(rest);
        std::string n,s,e;
        std::getline(ss,n,';');
        std::getline(ss,s,';');
        std::getline(ss,e);
        return new ContactLine(n,s,e);

    }

    return new TextLine(data);

}





#endif //LINE_H
