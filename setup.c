#include "setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

arguments get_flags(int argc, char** argv){
    arguments a = malloc(sizeof (arguments));
    a->is_pre = 0;
    int temp;
    while((temp = getopt (argc, argv, "x:y:f:p")) != -1){
        switch (temp) {
            case 'x':
                if(atoi(optarg)<2|| atoi(optarg)>1024){
                    printf("Podana wielkość jest za duża/mała");
                    break;
                }
                a->x = atoi(optarg);
                break;
            case 'y':
                if(atoi(optarg)<2|| atoi(optarg)>1024){
                    printf("Podana wielkość jest za duża/mała");
                    break;
                }
                a->y = atoi(optarg);
                break;
            case 'f':
                a->path = optarg;
                break;
            case 'p':
                a->is_pre = 1;
                break;
        }
    }
    return a;
}