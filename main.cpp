#include <bits/stdc++.h>
#include "BinaryStdIn.h"
#include "BinaryStdOut.h"
#include "HuffmanEncoding.h"
#include "DCT.h"
#include "LZW.h"
#include "RunLengthEncoding.h"
#include <benchmark/benchmark.h>
#include "CLI11.hpp"

//using namespace std;
//namespace fs = std::filesystem;
//
//size_t getFileSize(const string& filename) {
//    ifstream file(filename, ios::binary | ios::ate);
//    return file.tellg();
//}
//
//vector<fs::path> findFiles(const std::string& path) {
//    auto res = vector<filesystem::path>();
//
//    for(const auto& entry : fs::recursive_directory_iterator(path)) {
//        if(!fs::is_directory(entry) ) { // && fs::file_size(entry) < meta.size
//            res.push_back(entry.path().filename());
//        }
//    }
//
//    return res;
//}
//
//static void BM_HuffmanEncoding(benchmark::State& state) {
//    size_t originalSize = getFileSize("text.txt");
//
//    for (auto _ : state) {
//        Huffman h = Huffman();
//        h.compress("text.txt","huff_text.bin");
//        h.expand("huff_text.bin", "expanded_huff.txt");
//    }
//
//    size_t compressedSize = getFileSize("huff_text.bin");
//
//    double compressionRatio = (1-((double)(compressedSize) / (double)originalSize));
//
//    state.counters["Improvement Ratio"] = compressionRatio;
//
//
//}
//
//
//static void BM_LZWCompression(benchmark::State& state) {
//    size_t originalSize = getFileSize("text.txt");
//
//    for (auto _ : state) {
//        LZW c = LZW("text.txt", "lzw_text.bin");
//        c.compress();
//        LZW e = LZW("lzw_text.bin", "expanded_lzw.txt");
//        e.expand();
//    }
//
//    size_t compressedSize = getFileSize("lzw_text.bin");
//
//    double compressionRatio = (1-((double)(compressedSize) / (double)originalSize));
//
//    state.counters["ImprovementRatio"] = compressionRatio;
//}
//
//static void BM_RLECompression(benchmark::State& state) {
//    size_t originalSize = getFileSize("text.txt");
//
//    for (auto _ : state) {
//        RunLengthEncoding c = RunLengthEncoding("text.txt", "rle_text.bin");
//        c.compress();
//        RunLengthEncoding e = RunLengthEncoding("rle_text.bin", "expanded_rle.txt");
//        e.expand();
//    }
//
//    size_t compressedSize = getFileSize("rle_text.bin");
//
//    double compressionRatio = ((double)(compressedSize) / (double)originalSize);
//
//    state.counters["Improvement Ratio"] = compressionRatio;
//}


//turn off the main function and uncomment these to run the Google benchmarks

//BENCHMARK(BM_HuffmanEncoding)->Unit(benchmark::kMillisecond);
//BENCHMARK(BM_LZWCompression)->Unit(benchmark::kMillisecond);
//BENCHMARK(BM_RLECompression)->Unit(benchmark::kMillisecond);
//BENCHMARK_MAIN();


//int main() {
//    size_t originalSize = getFileSize("text.txt");
//    std::cout << "Original file size: " << originalSize << " bytes" << endl;
//
//    Huffman h = Huffman();
//    h.compress("text.txt","huff_text.bin");
//    h.expand("huff_text.bin", "expanded_huff.txt");
//
//    std::cout << "Compressed with Huffman size: " << getFileSize("huff_text.bin") << " bytes" << endl;
//
//
//    LZW c = LZW("text.txt", "lzw_text.bin");
//    c.compress();
//    LZW e = LZW("lzw_text.bin", "expanded_lzw.txt");
//    e.expand();
//
//    std::cout << "Compressed with LZW size: " << getFileSize("lzw_text.bin") << " bytes" << endl;
//
//    RunLengthEncoding cr = RunLengthEncoding("text.txt", "rle_text.bin");
//    cr.compress();
//    RunLengthEncoding er = RunLengthEncoding("rle_text.bin", "expanded_rle.txt");
//    er.expand();
//
//    std::cout << "Compressed with RLE size: " << getFileSize("rle_text.bin") << " bytes" << endl;
//
//    return 0;
//}

int main(int argc, char* argv[]) {
    CLI::App app{"Compression CLI"};

    std::string inputFilePath;
    std::string outputFilePath;
    std::string algorithm;
    bool compress = false;
    bool decompress = false;

    // Command-Line Arguments
    app.add_option("-i,--input", inputFilePath, "Input file path")->required()->default_val("text.txt");
    app.add_option("-o,--output", outputFilePath, "Output file path")->required()->default_val("output.bin");
    app.add_flag("-c,--compress", compress, "Compress the input file");
    app.add_flag("-d,--decompress", decompress, "Decompress the input file");
    app.add_option("-a,--algorithm", algorithm, "Compression algorithm (rle, lzw, huffman, dct)")->required();

    // Parsing Arguments
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    // Ensuring only one of compress or decompress is specified
    if (compress == decompress) {
        std::cerr << "Error: You must specify either -c/--compress or -d/--decompress, but not both." << std::endl;
        return 1;
    }

    // Creating compression/decompression objects
    if (algorithm == "rle") {
        RunLengthEncoding rle(std::move(inputFilePath), std::move(outputFilePath));
        if (compress) {
            rle.compress();
            std::cout << "File compressed using Run-Length Encoding successfully!" << std::endl;
        } else {
            rle.expand();
            std::cout << "File decompressed using Run-Length Encoding successfully!" << std::endl;
        }
    } else if (algorithm == "lzw") {
        LZW lzw(std::move(inputFilePath), std::move(outputFilePath));
        if (compress) {
            lzw.compress();
            std::cout << "File compressed using LZW successfully!" << std::endl;
        } else {
            lzw.expand();
            std::cout << "File decompressed using LZW successfully!" << std::endl;
        }
    } else if (algorithm == "huffman") {
        Huffman huffman;
        if (compress) {
            huffman.compress(std::move(inputFilePath), std::move(outputFilePath));
            std::cout << "File compressed using Huffman Encoding successfully!" << std::endl;
        } else {
            huffman.expand(std::move(inputFilePath), std::move(outputFilePath));
            std::cout << "File decompressed using Huffman Encoding successfully!" << std::endl;
        }
    } else if (algorithm == "dct") {
        dctCompression(inputFilePath);
    } else {
        std::cerr << "Algorithm not supported! (yet)" << std::endl;
        return 1;
    }

    return 0;
}
