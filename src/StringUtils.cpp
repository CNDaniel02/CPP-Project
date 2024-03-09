#include "StringUtils.h"

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    ssize_t len = str.length();
    if (start < 0) { start += len; }
    if (end <= 0) { end += len; }
    if (start >= len) { return ""; }
    if (end > len) { end = len; }
    if (start > end) { start = end; }
    
    ssize_t len1 = end - start;

    return str.substr(start,len1);
}


std::string Capitalize(const std::string &str) noexcept{
    // Replace code here
    
    if (!str.empty()) {
        std::string temp = str;
        size_t len = str.length();
        for (size_t i = 0; i < len; i++) {
            temp[i] = std::tolower(temp[i]);
        }
        temp[0] = std::toupper(temp[0]);
        return temp;
    }

    return str;
}

std::string Upper(const std::string &str) noexcept{
    if (!str.empty()) {
        std::string temp = str;
        for (size_t i = 0; i < temp.length(); i++) {
            temp[i] = std::toupper(temp[i]);

        }
        return temp;
    }

    return str;
}

std::string Lower(const std::string &str) noexcept{
    if (!str.empty()) {
        std::string temp = str;
        for (size_t i = 0; i < temp.length(); i++) {
            temp[i] = std::tolower(temp[i]);

        }
        return temp;
    }
    return str;
}

std::string LStrip(const std::string &str) noexcept{
    // Replace code here
    size_t i = 0;
    if (!str.empty() && str[0] == ' ') {
        while(i<str.length()) {
            if (str[i] != ' ') { break; }
            i++;
        }
        return str.substr(i);
    }
    return str;
}

std::string RStrip(const std::string &str) noexcept{
    size_t end = str.length() - 1;
    if (!str.empty()) {
        while (end>0 && str[end]==' ') {
            
            end--;
        }
        if (end == 0 && str[end] == ' ') { return ""; }
        return str.substr(0, end + 1);
    }
    return str;
}

std::string Strip(const std::string &str) noexcept{
    std::string temp=LStrip(str);
    temp = RStrip(temp);
    return temp;
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    size_t widths = width;
    if (widths < str.length()) { return str; }
    int i = widths - str.length();
    int left = i / 2;
    int right = i - left;
    std::string temp = str;
    std::string lefill;
    std::string rifill;
    for (int k = 0; k < left; k++) {
        lefill += fill;
    }
    for (int l = 0; l < right; l++) {
        rifill += fill;
    }
    
    return lefill + str + rifill;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    size_t widths = width;
    if (widths < str.length()) { return str; }
    std::string temp = str;
    int i = widths - str.length();
    for (int j = 0; j < i; j++) {
        temp += fill;
    }
    return temp;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    size_t widths = width;
    if (widths < str.length()) { return str; }
    std::string temp = str;
    std::string filling;
    int i = widths - str.length();
    for (int j = 0; j < i; j++) {
        filling += fill;
    }

    return filling + temp;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    if (old.empty()) { return str; }
    std::string temp = str;
    if (str.find(old) == std::string::npos) { return str; }
    else {
        
        size_t index = 0;
        while (temp.find(old,index)!=std::string::npos) {
            index= temp.find(old, index);
            temp.replace(index, old.length(), rep);
            index+=rep.length();
        }
        return temp;
    }
    
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
 
    std::vector<std::string> temp;
    size_t start = 0;
    size_t finish = str.find(splt);
    while (finish != std::string::npos) {
        temp.push_back(str.substr(start, finish - start));
        start = finish + splt.length();
        finish = str.find(splt, start);
    }
    temp.push_back(str.substr(start, str.length() - start));
    return temp;

}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    // Replace code here
    std::string temp;
    for (size_t i = 0; i < vect.size(); i++) {
        temp += vect[i];
        if (i < vect.size() - 1) {
            temp += str;
        }
        
    }
    return temp;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    // Replace code here
    std::string temp = str;
    size_t index = 0;
    while (temp.find('\t',index) != std::string::npos) {
        index = temp.find('\t', index);
        temp.replace(index, 1, std::string(tabsize, ' '));
        index += tabsize;
    }
    return temp;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
 

    int leftlen = left.length();
    int rightlen = right.length();
    std::vector<std::vector<int>> result1(leftlen + 1, std::vector<int>(rightlen + 1));
    for (int i = 0; i <= leftlen; i++) { result1[i][0] = i; }
    for (int j = 0; j <= rightlen; j++) { result1[0][j] = j; }
    for (int i = 1; i <= leftlen; i++) {
        for (int j = 1; j <= rightlen; j++) {
            if (left[i - 1] == right[j - 1]) {
                result1[i][j] = result1[i - 1][j - 1]; 
            }
            else {
                result1[i][j] = std::min(std::min(result1[i - 1][j], result1[i][j - 1]), result1[i - 1][j - 1]) + 1;



            }

        }


    }

    std::vector<std::vector<int>> result2(leftlen + 1, std::vector<int>(rightlen + 1));
    for (int i = 0; i <= leftlen; i++) { result2[i][0] = i; }
    for (int j = 0; j <= rightlen; j++) { result2[0][j] = j; }
    for (int i = 1; i <= leftlen; i++) {
        for (int j = 1; j <= rightlen; j++) {
            if (left[i - 1] == right[j - 1]) {
                result2[i][j] = result2[i - 1][j - 1];
            }
            else if (std::tolower(left[i - 1]) == std::tolower(right[j - 1])) {
                result2[i][j] = result2[i - 1][j - 1]+1;
            }
            else {
                result2[i][j] = std::min(std::min(result2[i - 1][j], result2[i][j - 1]), result2[i - 1][j - 1]) + 1;



            }

        }


    }


    if (ignorecase == true) {
        return result2[leftlen][rightlen];
    }
    else {
        return result1[leftlen][rightlen];
    }
    
}

};