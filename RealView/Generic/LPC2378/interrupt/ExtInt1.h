/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

void ExtInt1Init(unsigned char bMode, unsigned char bPositive);
void ExtInt1PreFunc(int isr_vector_num, int isr_param);
void ExtInt1PostFunc(int isr_vector_num, int isr_param);
