/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

void ExtInt2Init(unsigned char bMode, unsigned char bPositive);
void ExtInt2PreFunc(int isr_vector_num, int isr_param);
void ExtInt2PostFunc(int isr_vector_num, int isr_param);
