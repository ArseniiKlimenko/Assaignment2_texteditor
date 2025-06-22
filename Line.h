

#ifndef LINE_H
#define LINE_H

#include <string>
#include <iostream>


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



#endif //LINE_H
