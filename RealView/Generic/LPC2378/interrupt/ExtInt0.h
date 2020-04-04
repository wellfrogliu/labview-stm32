/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

void ExtInt0Init(unsigned char bMode, unsigned char bPositive);
void ExtInt0PreFunc(int isr_vector_num, int isr_param);
void ExtInt0PostFunc(int isr_vector_num, int isr_param);
