#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <algorithm>

using namespace std;

bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int calculateParityBits(int dataLength) {
    int r = 0;
    while ((1 << r) < dataLength + r + 1) {
        r++;
    }
    return r;
}

string encodeHamming(const string& data) {
    int m = data.length();
    int r = calculateParityBits(m);
    int totalLength = m + r;

    vector<int> hamming(totalLength + 1, 0);
    int dataIndex = 0;

    for (int i = 1; i <= totalLength; i++) {
        if (!isPowerOfTwo(i)) {
            hamming[i] = data[dataIndex] - '0';
            dataIndex++;
        }
    }

    for (int i = 0; i < r; i++) {
        int parityPosition = 1 << i;
        int parity = 0;

        for (int j = 1; j <= totalLength; j++) {
            if (j & parityPosition) {
                parity ^= hamming[j];
            }
        }

        hamming[parityPosition] = parity;
    }

    string result;
    for (int i = 1; i <= totalLength; i++) {
        result += char(hamming[i] + '0');
    }

    return result;
}

int detectErrorPosition(const string& received) {
    int n = received.length();
    int errorPosition = 0;

    for (int i = 1; i <= n; i <<= 1) {
        int parity = 0;

        for (int j = 1; j <= n; j++) {
            if (j & i) {
                parity ^= received[j - 1] - '0';
            }
        }

        if (parity != 0) {
            errorPosition += i;
        }
    }

    return errorPosition;
}

string correctHamming(string received) {
    int errorPosition = detectErrorPosition(received);

    if (errorPosition > 0 && errorPosition <= (int)received.length()) {
        received[errorPosition - 1] = received[errorPosition - 1] == '0' ? '1' : '0';
    }

    return received;
}

string extractOriginalData(const string& hammingCode) {
    string data;

    for (int i = 1; i <= (int)hammingCode.length(); i++) {
        if (!isPowerOfTwo(i)) {
            data += hammingCode[i - 1];
        }
    }

    return data;
}

vector<uint8_t> hexToBytes(const string& hexInput) {
    string cleanHex;

    for (char c : hexInput) {
        if (!isspace((unsigned char)c)) {
            cleanHex += c;
        }
    }

    if (cleanHex.length() % 2 != 0) {
        throw runtime_error("Invalid hexadecimal input length.");
    }

    vector<uint8_t> bytes;

    for (size_t i = 0; i < cleanHex.length(); i += 2) {
        string byteString = cleanHex.substr(i, 2);
        uint8_t byteValue = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
        bytes.push_back(byteValue);
    }

    return bytes;
}

string bytesToHex(const vector<uint8_t>& bytes) {
    stringstream ss;

    for (uint8_t byte : bytes) {
        ss << hex << uppercase << setw(2) << setfill('0') << (int)byte;
    }

    return ss.str();
}

uint32_t ethernetCRC32(const vector<uint8_t>& data) {
    uint32_t crc = 0xFFFFFFFF;

    for (uint8_t byte : data) {
        crc ^= byte;

        for (int i = 0; i < 8; i++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc ^ 0xFFFFFFFF;
}

vector<uint8_t> appendFCS(vector<uint8_t> frame) {
    uint32_t crc = ethernetCRC32(frame);

    frame.push_back(crc & 0xFF);
    frame.push_back((crc >> 8) & 0xFF);
    frame.push_back((crc >> 16) & 0xFF);
    frame.push_back((crc >> 24) & 0xFF);

    return frame;
}

bool verifyFCS(const vector<uint8_t>& receivedFrame) {
    if (receivedFrame.size() < 5) {
        return false;
    }

    vector<uint8_t> frameData(receivedFrame.begin(), receivedFrame.end() - 4);

    uint32_t receivedFCS = 0;
    receivedFCS |= (uint32_t)receivedFrame[receivedFrame.size() - 4];
    receivedFCS |= (uint32_t)receivedFrame[receivedFrame.size() - 3] << 8;
    receivedFCS |= (uint32_t)receivedFrame[receivedFrame.size() - 2] << 16;
    receivedFCS |= (uint32_t)receivedFrame[receivedFrame.size() - 1] << 24;

    uint32_t calculatedFCS = ethernetCRC32(frameData);

    return receivedFCS == calculatedFCS;
}

void hammingMenu() {
    string data;

    cout << "Enter binary data: ";
    cin >> data;

    string encoded = encodeHamming(data);

    cout << "Encoded Hamming Code: " << encoded << endl;

    string received;
    cout << "Enter received Hamming Code for checking: ";
    cin >> received;

    int errorPosition = detectErrorPosition(received);

    if (errorPosition == 0) {
        cout << "No error detected." << endl;
        cout << "Original Data: " << extractOriginalData(received) << endl;
    } else {
        cout << "Error detected at position: " << errorPosition << endl;

        string corrected = correctHamming(received);

        cout << "Corrected Code: " << corrected << endl;
        cout << "Original Data: " << extractOriginalData(corrected) << endl;
    }
}

void crcMenu() {
    cin.ignore();

    string hexFrame;

    cout << "Enter Ethernet frame header and payload in hexadecimal: ";
    getline(cin, hexFrame);

    try {
        vector<uint8_t> frame = hexToBytes(hexFrame);
        uint32_t crc = ethernetCRC32(frame);
        vector<uint8_t> transmittedFrame = appendFCS(frame);

        cout << "Calculated FCS: ";
        cout << hex << uppercase << setw(8) << setfill('0') << crc << dec << endl;

        cout << "Final Transmitted Frame: " << bytesToHex(transmittedFrame) << endl;

        string receivedHexFrame;

        cout << "Enter received Ethernet frame with FCS in hexadecimal: ";
        getline(cin, receivedHexFrame);

        vector<uint8_t> receivedFrame = hexToBytes(receivedHexFrame);

        if (verifyFCS(receivedFrame)) {
            cout << "The received frame is valid." << endl;
        } else {
            cout << "The received frame is corrupted." << endl;
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void runHammingTests() {
    cout << endl;
    cout << "Running Hamming Code Tests" << endl;

    string data = "1010101000011010";
    string encoded = encodeHamming(data);

    cout << "Test 1 - Encoding" << endl;
    cout << "Input Data: " << data << endl;
    cout << "Encoded Data: " << encoded << endl;

    if (encoded == "001001001010000111010") {
        cout << "Result: PASS" << endl;
    } else {
        cout << "Result: FAIL" << endl;
    }

    cout << endl;

    string received = encoded;
    int errorPosition = detectErrorPosition(received);

    cout << "Test 2 - No Error Detection" << endl;
    cout << "Received Data: " << received << endl;
    cout << "Detected Error Position: " << errorPosition << endl;

    if (errorPosition == 0) {
        cout << "Result: PASS" << endl;
    } else {
        cout << "Result: FAIL" << endl;
    }

    cout << endl;

    string corrupted = encoded;
    corrupted[6] = corrupted[6] == '0' ? '1' : '0';

    int detectedPosition = detectErrorPosition(corrupted);
    string corrected = correctHamming(corrupted);
    string extractedData = extractOriginalData(corrected);

    cout << "Test 3 - Single Bit Error Correction" << endl;
    cout << "Corrupted Data: " << corrupted << endl;
    cout << "Detected Error Position: " << detectedPosition << endl;
    cout << "Corrected Data: " << corrected << endl;
    cout << "Extracted Original Data: " << extractedData << endl;

    if (detectedPosition == 7 && corrected == encoded && extractedData == data) {
        cout << "Result: PASS" << endl;
    } else {
        cout << "Result: FAIL" << endl;
    }
}

void runCRCTests() {
    cout << endl;
    cout << "Running Ethernet CRC-32 Tests" << endl;

    string hexFrame = "FFFFFFFFFFFF001122334455080048656C6C6F";
    vector<uint8_t> frame = hexToBytes(hexFrame);
    uint32_t crc = ethernetCRC32(frame);
    vector<uint8_t> transmittedFrame = appendFCS(frame);

    cout << "Test 1 - CRC Generation" << endl;
    cout << "Frame Without FCS: " << hexFrame << endl;
    cout << "Calculated FCS: ";
    cout << hex << uppercase << setw(8) << setfill('0') << crc << dec << endl;
    cout << "Frame With FCS: " << bytesToHex(transmittedFrame) << endl;

    if (transmittedFrame.size() == frame.size() + 4) {
        cout << "Result: PASS" << endl;
    } else {
        cout << "Result: FAIL" << endl;
    }

    cout << endl;

    cout << "Test 2 - Valid Frame Verification" << endl;

    bool validResult = verifyFCS(transmittedFrame);

    if (validResult) {
        cout << "Verification Result: Valid" << endl;
        cout << "Result: PASS" << endl;
    } else {
        cout << "Verification Result: Corrupted" << endl;
        cout << "Result: FAIL" << endl;
    }

    cout << endl;

    cout << "Test 3 - Corrupted Frame Verification" << endl;

    vector<uint8_t> corruptedFrame = transmittedFrame;
    corruptedFrame[3] ^= 0x01;

    bool corruptedResult = verifyFCS(corruptedFrame);

    if (!corruptedResult) {
        cout << "Verification Result: Corrupted" << endl;
        cout << "Result: PASS" << endl;
    } else {
        cout << "Verification Result: Valid" << endl;
        cout << "Result: FAIL" << endl;
    }
}

void runAllTests() {
    runHammingTests();
    runCRCTests();
}

int main() {
    int choice;

    cout << "Data Link Layer Error Detection Tool" << endl;
    cout << "1. Hamming Code" << endl;
    cout << "2. Ethernet CRC-32" << endl;
    cout << "3. Run Tests" << endl;
    cout << "Choose an option: ";
    cin >> choice;

    if (choice == 1) {
        hammingMenu();
    } else if (choice == 2) {
        crcMenu();
    } else if (choice == 3) {
        runAllTests();
    } else {
        cout << "Invalid option." << endl;
    }

    return 0;
}
