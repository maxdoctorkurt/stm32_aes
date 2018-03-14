/*
 * AES.h
 *
 *  Created on: 6 мар. 2018 г.
 *      Author: mdk
 */

#ifndef AES_H_
#define AES_H_



int get_SBox_Value(int num);

int get_SBox_Inverse(int num);

void Expand_Keys();

void Add_Round_Key(int round, unsigned char state[4][4]);

void Sub_Bytes(unsigned char state[4][4]);

void Inv_Sub_Bytes(unsigned char state[4][4]);

void Shift_Rows(unsigned char state[4][4]);

void Inv_Shift_Rows(unsigned char state[4][4]);

void Mix_Columns(unsigned char state[4][4]);

void Inv_Mix_Columns(unsigned char state[4][4]);

void Encrypt(unsigned char plaintext[16], unsigned char Key[32], unsigned char encrypted[16], int rounds = 128);

void Decrypt(unsigned char encrypted[16], unsigned char Key[32], unsigned char plaintext[16], int rounds = 128);

#endif /* AES_H_ */
