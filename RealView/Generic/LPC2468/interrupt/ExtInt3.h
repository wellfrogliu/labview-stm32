/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

void ExtInt3Init(unsigned char bMode, unsigned char bPositive);
void ExtInt3PreFunc(int isr_vector_num, int isr_param);
void ExtInt3PostFunc(int isr_vector_num, int isr_param);
