//
// Created by atman on 12/24/25.
//
// from canonical_coder import CanonicalCoder
#include "iostream"
#include "canonical_coder.hpp"
#include "canonical_coder.hpp"


int main(int argc, char** argv) {
    // if (argc != 2) {
    //     std::cout << "usage: $TARGET $FILETOCOMPRESS" << std::endl;
    //     return 1;
    // }
    // std::string file_name(argv[1]);
    // std::string file_name = "/home/atman/Pictures/elaina.jpg";
    // CanonicalCoder(file_name, true);
    std::string file_in("/home/atman/Pictures/twilight.jpm");
    std::string file_out("/home/atman/Pictures/twilight1.jpg");
    CanonicalEncoderDecoder encoder_decoder(file_in, file_out, false);
    encoder_decoder.decode();
}