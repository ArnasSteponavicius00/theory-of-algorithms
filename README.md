# SHA-512 - Theory of Algorithms Project 2021

## Introduction
Theory of Algorithms Project - 4th Year. The goal of this project is to create a C program without the use of external libraries to take in a file or the path to a file as input through the command line and output the SHA-512 digest of the contents of the file.

## Compilation
Clone this repository: ``` git clone https://github.com/ArnasSteponavicius00/theory-of-algorithms.git```

Change into the project directory and create the executable using Make:

```
cd project
make sha512
```

Run the executable using:

```
./sha512 path/to/file
```

### **Extras**
#### Test
If the *coreutils* package is installed, you can run: ```make test```
to compare the output of this program to the sha512sum utility to make sure the program is creating the correct hash digest.
#### Clean
Using: ```make clean```
you can remove the sha512 executable that was created.

## What is the SHA-512 algorithm?
The SHA-512 algorithm applies a hashing function on the data/input that is given or passed into the function, and it produces an output which is called a *hash digest*. The *hash digest* has a fixed output length of 512 bits, and the function is able to take in an input message at the limit of 2^128. 2^128 is a practical limit, since it is such a massive number this creates *collision resistance*, meaning it is not feasible and extremely difficult to find two inputs to the function that would result in the same *hash digest*. This [blog post](https://bugcharmer.blogspot.com/2012/06/how-big-is-2128.html?m=1) goes into detail as to how long it would take to compute two inputs that match.

The algorithm is used in a variety of fields like internet security and blockchain technology. It is part of the SHA-2 family of cryptographic functions which consist of other algorithms like SHA-256. However it is not used as commonly as SHA-256 since SHA-256 is just as secure as SHA-512, and our current state of technology is unlikely to produce a collision. Reasons being SHA-256 is the universal choice for modern security certs for websites, has more compatibility with technologies and is generally faster than SHA-512, however SHA-512 is more secure and can be more efficient than SHA-256 in the cases of hashing more than 16 bytes of data on 64-bit systems but slower on 32-bit systems, since it processes 64-bit words rather than 32-bit, however for small messages it may be slower than SHA-256. These two posts give good insight into the speed of SHA-512 vs SHA-256 and also why SHA-512 is not used more even though it is more secure. [SHA512 vs. SHA256 speed](https://crypto.stackexchange.com/a/26351), [Why choose SHA256 over SHA512](https://security.stackexchange.com/a/165568)

## Why can't we reverse the SHA512 algorithm to retrieve the original message from a hash digest?
Hashes can not be unhashed, it is used in cryptography for this specific reason, so that whatever hash is returned it can not be reversed. Which is what separates hashing from encrypting. Although, if a hash is not "salted" (adding random data to the input to add uniqueness) then it can be prone to a ***dictionary***, ***rainbow table***  or ***birthday*** attacks so it can be reversed in this way, but no one uses the SHA-2 algorithms without adding "salt" to the input. Also since no matter the input length that is passed in i.e 1024 bytes, the algorithm will always return 64 bytes, data gets lost through the hashing iterations making it impossible to regain the "lost" data [[1]](https://stackoverflow.com/a/35899567). The [pigeonhole principle](https://coincentral.com/hashing-basics-history/) also plays part when determining whether a hash can be reversed. It states that if we fit (n) elements into (m) spaces where n > m, then should exist atleast one space which has more than two elements causing a collision, but hashing algorithms aim to minimise these collisions, making it harder to retrieve the messages since the aim is to preserve the original message without allowing it to be retrieved. It is also believed that if it can be proved that a hash cannot be reversed or pseudo-reversed, it would aid in the solving of the P vs NP problem [[2]](https://www.quora.com/How-is-it-possible-that-hashing-is-impossible-to-reverse-Is-there-a-proof).

## Can you design an algorithm that, given enough time, will find input messages that give each of the possible 512-bit strings?
Given enough time there are algorithms that would be able to compute all the hashes, however, the amount of hashes that would need to be calculated is 2^512 which is an enormous number and impossible to fathom, our current state of technology would take way too long to find messages that return all possible values. [This blog post](https://bugcharmer.blogspot.com/2012/06/how-big-is-2128.html?m=1) which was also mentioned above, gives good insight as to how large 2^128. Here is a quote from one of the paragraphs: *"If your PC can try 2^40 keys per day, it would take you about 847,904,136,496,835,804,725,427 (848 sextillion) years in the worst case."*. So calculating all the possible 512 bit strings which is 2^512 would not be feasible at all. This [quora user](https://www.quora.com/How-many-SHA-512-hashes-exist?) also gives an estimate and believes that currently less than 100 billion practical use SHA-512 hashes have been computed to this date, comparing to the size of 2^512, 100 billion might aswell be a drop in the ocean.

## How difficult is it to find a hash digest beginning with at least twelve zeros?
The answer to this question depends on the hardware that is used. Since it the SHA-2 algorithms are one-way functions it is impossible to calculate inputs that would return a hash value with 12 leading zeros, however, it is possible to roughly calculate how long it would take to calculate a hash value with 12 leading zeros based on the hardware. The first step is to get a target hash value (which will have 12 leading zeros in this case). Then you divide by the maximum value of a 512-bit number. This will give you a probability of getting 12 zeros. Get the reciprocal of the value (reciprocal is a pair of numbers i.e. 2/3, 1/1.46E-11), then divide the value by the amount of hashes by the hashrate of the hardware. This will give back the amount of seconds required to solve the problem. [Explation adapted from this answer](https://bitcoin.stackexchange.com/a/81660). Crypto miners use calculations like this to create target hashes when mining for blocks.

## Author:
* Arnas Steponavicius

## References:
* [Ian McLoughlin](https://github.com/ianmcloughlin)
* [Secure Hash Standard NIST](https://www.nist.gov/publications/secure-hash-standard)
* [SHA-512 Input Limit](https://crypto.stackexchange.com/questions/64714/why-is-sha-512-limited-to-an-input-of-2128-bits)
* [Reversing Hashes, is it possible?](https://www.quora.com/How-is-it-possible-that-hashing-is-impossible-to-reverse-Is-there-a-proof)
* [How big is 2^128](https://bugcharmer.blogspot.com/2012/06/how-big-is-2128.html?m=1)
* [Creating a hash with 9 zeros SHA-256](https://bitcoin.stackexchange.com/a/81660)
* [Generation Calculator](https://en.bitcoin.it/wiki/Generation_Calculator)
* [Target Hash](https://www.investopedia.com/terms/t/target-hash.asp)
