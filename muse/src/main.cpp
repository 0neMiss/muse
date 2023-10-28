#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include "parser.hpp"
#include "tokenization.hpp"

// An enum to help us parse
// each thing that needs to be treated differently could be considered a token.




std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output <<  "global _start\n_start:\n";
    for(int i = 0; i < tokens.size(); i++ ){
        const Token& token = tokens.at(i);
        if(token.type == TokenType::exit){
           if(i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit){
               if(i + 2 < tokens.size() &&  tokens.at(i + 2).type == TokenType::semi){
                   //building dumb assembly
                   // move the exit code "60" into the rax or "alpha" register
                   // this is where syscalls, which are calls to the operating system, are made.
                   output << "    mov rax, 60\n";
                   // move the number we are returning to the rdi register that will be returned on the exit code.
                   output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                   // syscall with the rax code and the rdi value.
                   output << "    syscall";
               }
           }
        }
    }
    return output.str();
}

int main(int argc, char* argv[] ) {
    if(argc != 2){
        // in case they provided bad input
        std::cerr << "Incorrect usage, Correct usage..." << std::endl;
        std::cerr << "muse <input.mu>" << std::endl;
        return EXIT_FAILURE;
    }

    // let contents:string basically
    std::string contents;
    {
        // basically a string with methods
        std::stringstream contents_stream;
        // this is a file stream
        // we are taking the first argument passed in the terminal and looking at that file path?
        std::fstream input(argv[1], std::ios::in);
        //then set contents_stream to the result of input's rdbuf();
        // to better understand this syntax look into overloads
        contents_stream << input.rdbuf();
        // then convert this content stream to a string
        contents = contents_stream.str();
    }
    //tokenize the contents
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();
    std::cout << tokens_to_asm(tokens) << std::endl;
    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    return EXIT_SUCCESS;
}