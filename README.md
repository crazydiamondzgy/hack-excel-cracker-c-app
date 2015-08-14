hack-excel-cracker-c-app
========================

I wrote this cracker in 2005, and it really works on Office97, I don't know what's everything going on higher version, if you have any result please let'me known.

Compatibility
-------------

Install nasm and VC6 first, and run this program in win32 operation system like win2000 and XP which are tested

Introduction
------------

This paper discusses the way Microsoft Excel handles encryption. As far as I know, it is the most complete description on the internet. If anyone has anything to add, please send me an e-mail.
Workbook Encryption

The 1Table structure contains three 16 byte numbers:
1) A random salt.
2) An MD5 hashed nonce, encrypted using RC4 with a key (K). This encrypted hash is stored as a second 16 byte number in 1Table.
3) The nonce is encrypted using key (K) and MD5 hashed then stored as a third 16 byte number in 1Table.

The key (K) is calculated in the following way:
1) The password (expressed in Unicode) is MD5 hashed.
2) The first five bytes of the password hash are put into an array with the 1st 16 byte number (the salt) stored in the 1Table structure.
3) The salt is repeatedly concatenated with the password and then padded according to the MD5 algorithm.
4) The MD5 hash is taken.
5) The first five bytes of this hash are saved and then padded and MD5 hashed again.
6) The first five bytes of this hash along with a counter byte become the RC4 key for encrypting/decrypting the document. The counter periodically re-keys the RC4 engine by incrementing the counter byte modulo 8.

This key is first MD5 hashed before RC4 key scheduling. This hash is the key (K).

There is a better way to recover the document. This is to recover the
five byte value which is the document key, add the counter byte and
decrypt the document.

Worksheet Protection
--------------------

When an Excel sheet is being protected with a password, a 16-bit (two byte) long hash is generated. To verify a password, it is compared to the hash. Obviously, if the input data volume is great, numerous passwords will match the same hash. This can be easily reproduced:

Protect a sheet (Tools-Protection-Protect Sheet)
with the password "test" (no quotes)
Unprotect it with the password "zzyw" (again no quotes)

Here is the algorithm to create the hash value:

1) take the ASCII values of all characters shift left the first character 1 bit, the second 2 bits and so on (use only the lower 15 bits and rotate all higher bits, the highest bit of the 16-bit value is always 0 [signed short])
2) XOR all these values
3) XOR the count of characters
4) XOR the constant 0xCE4B
Example: The password is abcdefghij (10 characters) 
a -> 0x61 << 1 == 0x00C2
b -> 0x62 << 2 == 0x0188
c -> 0x63 << 3 == 0x0318
d -> 0x64 << 4 == 0x0640
e -> 0x65 << 5 == 0x0CA0
f -> 0x66 << 6 == 0x1980
g -> 0x67 << 7 == 0x3380
h -> 0x68 << 8 == 0x6800
i -> 0x69 << 9 == 0x5201 (unrotated: 0xD200)
j -> 0x6A << 10 == 0x2803 (unrotated: 0x1A800)
count: 0x000A 
constant: 0xCE4B 
result: 0xFEF1 
This value occurs in the PASSWORD record.

Resources
---------

An article at SecuriTeam.com
Some source code that shows how to implement the Word/Excel 8 encryption algorithm. This code is part of the WvWare project. It needs to be modified to handle excel workbooks. If you want to help with this, please send me an E-mail
E-mail messages on the StarCalc list at OpenOffice.org regarding Excel Encryption:
