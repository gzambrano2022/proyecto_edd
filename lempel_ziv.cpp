#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iterator>
#include <chrono>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    int firstPosition;
    TrieNode() : firstPosition(-1) {}
};

class Trie {
private:
    TrieNode* root;

    std::pair<int, int> searchLongestMatch(const std::string& s, int startPos) {
        TrieNode* node = root;
        int length = 0;
        int pos = -1;

        for (int i = startPos; i < s.size(); ++i) {
            char ch = s[i];
            if (node->children.find(ch) == node->children.end()) {
                break;
            }
            node = node->children[ch];
            if (node->firstPosition != -1 && node->firstPosition < startPos) {
                length = i - startPos + 1;
                pos = node->firstPosition;
            }
        }

        return {pos, length};
    }

    void insert(const std::string& s, int startPos) {
        TrieNode* node = root;
        for (int i = startPos; i < s.size(); ++i) {
            char ch = s[i];
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch];
            if (node->firstPosition == -1) {
                node->firstPosition = startPos;
            }
        }
    }

public:
    Trie() : root(new TrieNode()) {}

    std::vector<std::pair<std::string, int>> compress(const std::string& input) {
        std::vector<std::pair<std::string, int>> output;
        int i = 0;

        while (i < input.size()) {
            auto match = searchLongestMatch(input, i);
            if (match.first == -1 || match.second == 0) {
                output.push_back({std::string(1, input[i]), 0});
                insert(input, i);
                i++;
            } else {
                output.push_back({std::to_string(match.first), match.second});
                insert(input, i);
                i += match.second;
            }
        }

        return output;
    }

    std::string decompress(const std::vector<std::pair<std::string, int>>& compressed) {
        std::string decompressed;

        for (const auto& p : compressed) {
            if (p.second == 0) {
                decompressed += p.first;
            } else {
                int pos = std::stoi(p.first);
                int length = p.second;
                for (int i = 0; i < length; ++i) {
                    decompressed += decompressed[pos + i];
                }
            }
        }

        return decompressed;
    }
};

int main() {
    std::ifstream archivo("test.txt");
    if(!archivo.is_open()){
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::string texto((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
    archivo.close();


    Trie trie;

    auto inicio_com = std::chrono::high_resolution_clock::now();
    auto compressed = trie.compress(texto);
    auto fin_com = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_com = fin_com - inicio_com;

    std::cout << "Compressed output: \n";
    for (const auto& p : compressed) {
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << std::endl;

    auto inicio_decom = std::chrono::high_resolution_clock::now();
    std::string decompressed = trie.decompress(compressed);
    auto fin_decom = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_decom = fin_decom - inicio_decom;

    std::cout << "Decompressed output: " << decompressed << std::endl;

     std::cout << "Tiempo de compresion: " << duracion_com.count() << std::endl;
    std::cout << "Tiempo de decompresion: " << duracion_decom.count() << std::endl;

    return 0;
}
