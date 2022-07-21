/*ppm.h
 * @Řešení IJC-DU1, příklad b), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#ifndef IJC_DU1_PPM_H
#define IJC_DU1_PPM_H

#define MAX_LIMIT 3*8000*8000
#define RGB 3

 struct ppm
 {
    unsigned xsize;
    unsigned ysize;
    char data[];
};

struct ppm * ppm_read(const char * filename);

void ppm_free(struct ppm *p);

#endif
