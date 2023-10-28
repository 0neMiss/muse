#pragma once
#include <optional>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token{
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
    public:
        inline explicit Tokenizer(const std::string& src)
            : m_src(std::move(src))
        {
        }
        inline std::vector<Token> tokenize(){
            std::vector<Token> tokens;
            std::string buf;
            // while we haven't reached the last character
            while(peek().has_value()){
                // if we are a character that is not a number
                if(std::isalpha(peek().value())){
                    // push that character to the buffer
                    buf.push_back(consume());
                    // while there are still alphanumeric characters to find within the substring
                    while(peek().has_value() && std::isalnum(peek().value())){
                        // push to the buffer
                        buf.push_back(consume());
                    }
                    if(buf == "exit"){
                        tokens.push_back({.type = TokenType::exit });
                        buf.clear();
                        continue;
                    } else {
                       std::cerr << "You messed up!" << std::endl;
                       exit(EXIT_FAILURE);
                    }
                }
                else if( std::isdigit(peek().value()) ){
                    buf.push_back(consume());
                    while(peek().has_value() && std::isdigit(peek().value())){
                        buf.push_back(consume());
                    }
                    tokens.push_back({ .type = TokenType::int_lit, .value = buf});
                    buf.clear();
                }
                else if(peek().value() == ';'){
                    buf.push_back(consume());
                    //consume the token even if we dont use their value
                    tokens.push_back({.type = TokenType::semi});
                    continue;
                }
                else if(std::isspace(peek().value())){
                    //consume the token even if we dont use their value
                    consume();
                    continue;
                }
                else{
                    std::cerr << "You messed up!" << std::endl;
                }
            }
            m_index = 0;
            return tokens;
        }
private:
    [[nodiscard]] std::optional<char> peek(int ahead = 1) const
    {
            if(m_index + ahead > m_src.length()){
                return {};
            } else {
                return m_src.at(m_index);
            }
    }
    char consume(){
           // increment and return the character at the index before incrementing.
            return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;

};