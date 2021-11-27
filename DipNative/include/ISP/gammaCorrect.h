#ifndef __GAMMA_CORRECT_H__
#define __GAMMA_CORRECT_H__
void generateTable(unsigned char * table,unsigned int size);
void GammaCorrection(unsigned char *src,unsigned char *dst,unsigned int pixel_nb,unsigned char * table);
#endif //__GAMMA_CORRECT_H__