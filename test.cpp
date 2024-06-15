#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    int firstPosition;
    TrieNode() : firstPosition(-1) {}
};

class Trie {
public:
    Trie() : root(new TrieNode()) {}

    // Busca la longitud más larga que coincide con el prefijo que comienza en startPos.
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

    // Insertar la cadena que comienza en startPos y actualizar la primera posición en el trie.
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

private:
    TrieNode* root;
};

std::vector<std::pair<std::string, int>> compress(const std::string& input) {
    Trie trie;
    std::vector<std::pair<std::string, int>> output;
    int i = 0;

    while (i < input.size()) {
        auto match = trie.searchLongestMatch(input, i);
        if (match.first == -1 || match.second == 0) {
            // No se encontró coincidencia
            output.push_back({std::string(1, input[i]), 0});
            trie.insert(input, i);
            i++;
        } else {
            // Se encontró coincidencia
            output.push_back({std::to_string(match.first), match.second});
            trie.insert(input, i);
            i += match.second;
        }
    }

    return output;
}

int main() {
    std::string input = "tangananica-tanganana";
    auto compressed = compress(input);

    std::cout << "Compressed output: \n";
    for (const auto& p : compressed) {
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << std::endl;

    return 0;
}
