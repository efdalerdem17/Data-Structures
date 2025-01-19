/* This homework prepared by Ayşe Efdal Erdem as HW3 for CS 300.
  This program implements a basic image detection system using hash tables.
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
// store and retrieve images
class HashTable {
private:
    struct ImageRecord {
        string encodingrle; // run length encoding
        vector<string> image; // image data
        bool inuse; // is slot occupied flag
        bool erased;  // is data deleted flag

        ImageRecord() : inuse(false), erased(false) {} //constructor
    };

    vector<ImageRecord> table; // storage
    unsigned int size; // hash table size

    unsigned int hashFunction(const string& encodingrle) const {
        unsigned int hash = 0;
        for (char c : encodingrle) {
            hash = (hash * 31 + c) % size; // Horner's method with modulo
        }
        return hash;
    }
// find next empty slot by using linear probing in hash tables
    unsigned int nextemptyposition(unsigned int startPos, const string& encodingrle) const {
        unsigned int pos = startPos;
        while (table[pos].inuse && !table[pos].erased && table[pos].encodingrle != encodingrle) {
            pos = (pos + 1) % size; // move next position
            if (pos == startPos) return size; // full
        }
        return pos;
    }

public:
    HashTable(unsigned int tableSize = 997) : size(tableSize) {
        table.resize(size);
    }
// insert image and encode
    bool insert(const string& encodingrle, const vector<string>& image) {
        unsigned int pos = hashFunction(encodingrle);
        pos = nextemptyposition(pos, encodingrle);

        if (pos == size) return false; // table full

        table[pos].encodingrle = encodingrle;
        table[pos].image = image;
        table[pos].inuse = true;
        table[pos].erased = false;
        return true;
    }
 // find image method
    const vector<string>* find(const string& encodingrle) const {
        unsigned int pos = hashFunction(encodingrle);
        unsigned int startPos = pos;

        while (table[pos].inuse || table[pos].erased) {
            if (table[pos].inuse && !table[pos].erased && table[pos].encodingrle == encodingrle) {
                return &table[pos].image;
            }
            pos = (pos + 1) % size;
            if (pos == startPos) break;
        }
        return nullptr;
    }
};

class ImageProcessor {
public:
    // flatten image
    static string convertflattenimg(const vector<string>& image) {
        string flattened;
        for (const auto& row : image) {
            flattened += row;
        }
        return flattened;
    }
// encoding using rle
    static string encodeencodingrle(const string& flattened) {
        string encodingrle;
        char current = flattened[0];
        int count = 1;

        for (unsigned int i = 1; i < flattened.length(); ++i) {
            if (flattened[i] == current) {
                count++;
            } else {
                encodingrle += to_string(count) + (current == '0' ? "B" : "W");
                current = flattened[i];
                count = 1;
            }
        }
        encodingrle += to_string(count) + (current == '0' ? "B" : "W");

        return encodingrle;
    }
 // read image file , return as vector
    static vector<string> processImageFile(const string& filename) {
        ifstream file(filename);
        vector<string> image;
        if (!file.is_open()) return image;

        string line;
        while (getline(file, line) && image.size() < 28) {
            if (line.length() >= 28) {
                image.push_back(line.substr(0, 28));
            }
        }
        file.close();
        return image;
    }
};

int main() {
    HashTable hashTable;
    string input;
 // image insertation
    while (true) {
        cout << "Enter image number to insert into the hash table (or 'query' to continue): ";
        cin >> input;
        cout << endl;

        if (input == "query") break;

        string filename = "image" + input + ".txt";
        vector<string> image = ImageProcessor::processImageFile(filename);

        if (!image.empty()) {
            string encodingrle = ImageProcessor::encodeencodingrle(ImageProcessor::convertflattenimg(image));
            hashTable.insert(encodingrle, image);
            cout << "Image " << input << " inserted into the hash table." << endl;
        }
    }
// query processing
    while (true) {
        cout << "Enter image number to query (or 'exit' to quit): ";
        cin >> input;
        cout << endl;

        if (input == "exit") {
            cout << "Exiting the program!" << endl;
            break;
        }

        string filename = "query" + input + ".txt";
        vector<string> queryImage = ImageProcessor::processImageFile(filename);

        if (!queryImage.empty()) {
            string encodingrle = ImageProcessor::encodeencodingrle(ImageProcessor::convertflattenimg(queryImage));
            const vector<string>* match = hashTable.find(encodingrle);

            if (match != nullptr) {
                cout << "RLE String for query" << input << ".txt found in hash table." << endl;
                for (const auto& line : *match) {
                    cout << line << endl;
                }
            } else {
                cout << "No match for the image with encoding: " << encodingrle << endl;
            }
        }
    }

    return 0;
}