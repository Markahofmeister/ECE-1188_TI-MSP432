/*
 * main.c
 *
 *  Created on: Jan 10, 2023
 *      Author: marka
 *
 */


#include "msp.h"

int modify_value(int z);

int main(void) {

    int i,j,y,alpha,beta,finished;

    y= 0;
    finished = 0;
    alpha = 10;
    beta = 3;


    for(i = 0; i < alpha; i++) {
        for (j = 0; j < beta; j++) {
            y = modify_value(y);
        }
    }

    finished = 1;
    return finished;

}

int modify_value(int z) {
    int w;
    w = z + 1;

    return(w);
}

