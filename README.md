# Data Link Layer Error Detection

This project is a C++ implementation for error detection and correction methods used in the Data Link Layer.

The program includes two main parts:

- Hamming Code
- Ethernet CRC-32 / FCS

It also includes a built-in test mechanism to check that the implementation works correctly.

## Project Description

In computer networks, the Data Link Layer is responsible for reliable frame transmission between directly connected devices. Error detection and correction are important parts of this layer.

This project implements:

1. Hamming Code for single-bit error detection and correction.
2. Ethernet CRC-32 for Frame Check Sequence generation and verification.

## Features

- Generate Hamming Code from binary data
- Detect error position in received Hamming Code
- Correct single-bit error using Hamming Code
- Extract original data from corrected Hamming Code
- Calculate Ethernet CRC-32
- Append FCS to an Ethernet frame
- Verify received Ethernet frame using FCS
- Detect corrupted Ethernet frames
- Built-in test mode with PASS and FAIL results

## Programming Language

C++

## Repository Name

data-link-layer-error-detection

## Files

```text
data-link-layer-error-detection/
│
├── main.cpp
├── README.md
└── .gitignore
```

## How to Compile

Use the following command to compile the program:

```bash
g++ main.cpp -o app
```

## How to Run

After compiling, run the program with:

```bash
./app
```

On Windows, you can run:

```bash
app.exe
```

## Program Menu

After running the program, this menu will be shown:

```text
Data Link Layer Error Detection Tool
1. Hamming Code
2. Ethernet CRC-32
3. Run Tests
Choose an option:
```

## Option 1: Hamming Code

This option is used to generate and check Hamming Code.

Example input:

```text
1010101000011010
```

Expected encoded Hamming Code:

```text
001001001010000111010
```

The program can also receive a Hamming Code and check whether it has an error or not.

If a single-bit error exists, the program detects the error position and corrects it.

## Option 2: Ethernet CRC-32

This option is used to calculate and verify Ethernet CRC-32 / FCS.

The user enters the Ethernet frame header and payload in hexadecimal format.

Example input:

```text
FFFFFFFFFFFF001122334455080048656C6C6F
```

The program calculates the CRC-32 value and appends it as the FCS field at the end of the frame.

Then the user can enter a received Ethernet frame with FCS, and the program checks whether the frame is valid or corrupted.

## Ethernet CRC-32

The standard Ethernet CRC-32 is used in this project.

The normal Ethernet generator polynomial is:

```text
0x04C11DB7
```

In the implementation, the reflected form is used:

```text
0xEDB88320
```

The CRC initial value is:

```text
0xFFFFFFFF
```

The final CRC value is XORed with:

```text
0xFFFFFFFF
```

The FCS is appended to the frame in little-endian byte order, which is the normal Ethernet transmission format.

## Option 3: Test Mechanism

The project includes a simple built-in test mechanism.

To run the tests, choose option 3 from the menu:

```text
3
```

The test mode checks:

- Hamming Code encoding
- Hamming Code no-error detection
- Hamming Code single-bit error correction
- Ethernet CRC-32 generation
- Ethernet CRC-32 valid frame verification
- Ethernet CRC-32 corrupted frame detection

Each test prints one of these results:

```text
PASS
FAIL
```

## Test Cases

### Hamming Code Encoding Test

Input data:

```text
1010101000011010
```

Expected encoded data:

```text
001001001010000111010
```

If the generated output is equal to the expected value, the test result is PASS.

### Hamming Code Error Detection Test

The program checks the encoded Hamming Code without changing any bit.

Expected result:

```text
No error detected
```

### Hamming Code Error Correction Test

The program changes one bit of the encoded data on purpose.

Then it checks:

- Error position detection
- Correction of the corrupted bit
- Extraction of original data

If all of these steps are correct, the result is PASS.

### CRC-32 Generation Test

The program uses this sample Ethernet frame without FCS:

```text
FFFFFFFFFFFF001122334455080048656C6C6F
```

It calculates the CRC-32 value and appends the FCS to the frame.

If four FCS bytes are added correctly, the result is PASS.

### CRC-32 Valid Frame Verification Test

The program verifies the frame after adding the correct FCS.

Expected result:

```text
Valid
```

### CRC-32 Corrupted Frame Verification Test

The program changes one byte of the frame on purpose.

Then it checks the FCS again.

Expected result:

```text
Corrupted
```

## Sample Output for Test Mode

```text
Running Hamming Code Tests
Test 1 - Encoding
Input Data: 1010101000011010
Encoded Data: 001001001010000111010
Result: PASS

Test 2 - No Error Detection
Received Data: 001001001010000111010
Detected Error Position: 0
Result: PASS

Test 3 - Single Bit Error Correction
Corrupted Data: 001001101010000111010
Detected Error Position: 7
Corrected Data: 001001001010000111010
Extracted Original Data: 1010101000011010
Result: PASS

Running Ethernet CRC-32 Tests
Test 1 - CRC Generation
Frame Without FCS: FFFFFFFFFFFF001122334455080048656C6C6F
Calculated FCS: The program prints the calculated value
Frame With FCS: The program prints the final frame
Result: PASS

Test 2 - Valid Frame Verification
Verification Result: Valid
Result: PASS

Test 3 - Corrupted Frame Verification
Verification Result: Corrupted
Result: PASS
```

## Notes

This project was created for the second Data Link Layer assignment.

The code is written in C++ and does not need any external library.

The test mechanism is included inside the main program, so the user can test the project directly from the menu.
