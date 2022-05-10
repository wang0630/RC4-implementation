#include <iostream>
#include <algorithm>

using namespace std;

#define STATE_SIZE 256

class Rc4 {
public:
    void run(const char* key_str, const int& bytesNeeded) {
        this->KEY_SIZE = strlen(key_str) / 2;
        this->init(key_str);

        unsigned char finalStream[bytesNeeded];
        for (unsigned char & i : finalStream) {
            i = this->getBits();
        }
        this->printHex(finalStream, bytesNeeded);
    }

private:
    // key size in byte
    unsigned long KEY_SIZE;
    unsigned char key[STATE_SIZE] = {0};
    unsigned char state_arr[STATE_SIZE] = {0};
    pair<int, int> state_indexes;

    void hexString2Char(const char* s, unsigned char* buff) {
        for (int i = 0, j = 0; i < strlen(s); i = i + 2, j++) {
            char p[2];
            memcpy(p, s + i, 2);
            // Convert hex string "7b" to one byte number 118
            // and assign to unsigned char
            unsigned char k = strtol(p, nullptr, 16);
            buff[j] = k;
        }
    }

    void printHex(unsigned char* test, int size) {
        for (int i = 0; i < size; i++) {
            printf("%.2x", test[i]);
        }
        cout << endl;
    }

    void init(const char* key_str) {
        // Convert key to actual byte array
        this->hexString2Char(key_str, this->key);
        for(int i = 0; i < STATE_SIZE; i++) {
            this->state_arr[i] = i;
            this->key[i] = this->key[i % this->KEY_SIZE];
        }

        int j = 0;
        for(int i = 0; i < STATE_SIZE; i++) {
            j = (j + (int)this->state_arr[i] + (int)this->key[i]) % STATE_SIZE;
            swap(this->state_arr[i], this->state_arr[j]);
        }

        this->state_indexes.first = 0;
        this->state_indexes.second = 0;
    }

    unsigned char getBits() {
        int new_i = (this->state_indexes.first + 1) % STATE_SIZE;
        int new_j = (this->state_indexes.second + (int)this->state_arr[new_i]) % STATE_SIZE;

        swap(this->state_arr[new_i], this->state_arr[new_j]);

        int t = ((int)this->state_arr[new_i] + (int)this->state_arr[new_j]) % STATE_SIZE;

        this->state_indexes.first = new_i;
        this->state_indexes.second = new_j;

        return this->state_arr[t];
    }
};

int main() {
    string key;
    int bytesNeeded;
    cout << "Enter key: " << endl;
    cin >> key;
    cout << "Enter bytes needed: " << endl;
    cin >> bytesNeeded;
    auto rc4 = new Rc4();
    rc4->run(key.c_str(), bytesNeeded);

    return 0;
}
