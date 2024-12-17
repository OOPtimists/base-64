#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

// функция проверяет, является ли символ допустимым для кодировки Base64
bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

// функция кодирования в Base64
string base64_encode(const string& input) {
    string encoded;
    unsigned char bytes_3[3];
    unsigned char bytes_4[4];
    int i = 0, j = 0;

    for (unsigned char c : input) {
        bytes_3[i++] = c;
        if (i == 3) {
            bytes_4[0] = (bytes_3[0] & 0xfc) >> 2;
            bytes_4[1] = ((bytes_3[0] & 0x03) << 4) + ((bytes_3[1] & 0xf0) >> 4);
            bytes_4[2] = ((bytes_3[1] & 0x0f) << 2) + ((bytes_3[2] & 0xc0) >> 6);
            bytes_4[3] = bytes_3[2] & 0x3f;

            for (i = 0; i < 4; i++)
                encoded += base64_chars[bytes_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) bytes_3[j] = '\0';

        bytes_4[0] = (bytes_3[0] & 0xfc) >> 2;
        bytes_4[1] = ((bytes_3[0] & 0x03) << 4) + ((bytes_3[1] & 0xf0) >> 4);
        bytes_4[2] = ((bytes_3[1] & 0x0f) << 2);

        for (j = 0; j < i + 1; j++)
            encoded += base64_chars[bytes_4[j]];

        while (i++ < 3)
            encoded += '=';
    }

    return encoded;
}

// функция декодирования из Base64
string base64_decode(const string& input) {
    int in_len = input.size();
    int i = 0, j = 0, in_ = 0;
    unsigned char bytes_4[4], bytes_3[3];
    string decoded;

    for (unsigned char c : input) {
        if (c == '=') break;
        if (!is_base64(c)) continue;

        bytes_4[i++] = base64_chars.find(c);
        if (i == 4) {
            bytes_3[0] = (bytes_4[0] << 2) + ((bytes_4[1] & 0x30) >> 4);
            bytes_3[1] = ((bytes_4[1] & 0xf) << 4) + ((bytes_4[2] & 0x3c) >> 2);
            bytes_3[2] = ((bytes_4[2] & 0x3) << 6) + bytes_4[3];

            for (i = 0; i < 3; i++)
                decoded += bytes_3[i];
            i = 0;
        }
    }

    return decoded;
}

// функция обработки файла
void processFile(const string& input_file, const string& output_file, bool encode) {
    ifstream in(input_file);
    ofstream out(output_file);
    string line, data;

    while (getline(in, line)) {
        data += line;
    }

    if (encode) {
        out << base64_encode(data);
    }
    else {
        out << base64_decode(data);
    }

    in.close();
    out.close();
}

int main() {
    system("chcp 1251");
    // файлы для кодирования
    string input_files_encode[] = { "input1.txt", "input2.txt", "input3.txt" };
    string output_files_encode[] = { "output1_encoded.txt", "output2_encoded.txt", "output3_encoded.txt" };

    // файлы для декодирования
    string input_files_decode[] = { "encoded1.txt", "encoded2.txt", "encoded3.txt" };
    string output_files_decode[] = { "output1_decoded.txt", "output2_decoded.txt", "output3_decoded.txt" };

    // кодирование файлов
    for (int i = 0; i < 3; i++) {
        processFile(input_files_encode[i], output_files_encode[i], true);
        cout << "Файл " << input_files_encode[i] << " закодирован в " << output_files_encode[i] << endl;
    }

    // декодирование файлов
    for (int i = 0; i < 3; i++) {
        processFile(input_files_decode[i], output_files_decode[i], false);
        cout << "Файл " << input_files_decode[i] << " декодирован в " << output_files_decode[i] << endl;
    }

    return 0;
}
