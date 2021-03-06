//
//  socket_utils.c
//  3CAT-2_COMMS_SOCKET
//
//  Created by Juan Fran Muñoz Martin on 01/10/14.
//  Copyright (c) 2014 Juan Fran Muñoz Martin. All rights reserved.
//

#include "socket_utils.h"

#define FEND    0xC0
#define FESC    0xDB
#define TFEND   0xDC
#define TFESC   0xDD

#if 0
int read_kiss_from_socket(int fd, char * buffer){
    int i;
    int out_size;
    int size;
    char byte;
    bool no_frame;
    bool transpose;
    byte = 0x00;
    /* First of all read the two first bytes */
    while((unsigned char)byte != FEND){
        size = read(fd, &byte, 1);
        if (size <= 0){
            if (size == 0){
                printf("End of socket\n");
            }else{
                perror("Error reading socket -> ");
            }
            exit(-1);
        }
    }
    if ((unsigned char)byte == FEND){
        size = read(fd, &byte, 1);
        if (size <= 0){
            if (size == 0){
                printf("End of socket\n");
            }else{
                perror("Error reading socket -> ");
            }
            exit(-1);
        }
        if ((unsigned char)byte != 0x00){
            printf("Control frame found: ");
            no_frame = true;
            i = 0;
            transpose = false;
            while(no_frame){
                size = read(fd, &byte, 1);
                if (size <= 0){
                    if (size == 0){
                        printf("End of socket\n");
                    }else{
                        perror("Error reading socket -> ");
                    }
                    exit(-1);
                }
                if ((unsigned char)byte == FEND){
                    no_frame = false;
                }else if (transpose == true){
                    if ((unsigned char) byte == TFEND){
                        buffer[i] = FEND;
                    }else if ((unsigned char) byte == TFESC){
                        buffer[i] = FESC;
                    }
                    transpose = false;
                    i++;
                }else if ((unsigned char) byte == FESC){
                    transpose = true;
                }else{
                    buffer[i] = byte;
                    i++;
                }
            }
            buffer[i] = '\0';
            printf("%s\n", buffer);
            return -2;
        }
    }else{
        return -1;
    }
    no_frame = true;
    i = 0;
    transpose = false;
    while(no_frame){
        size = read(fd, &byte, 1);
        if (size <= 0){
            if (size == 0){
                printf("End of socket\n");
            }else{
                perror("Error reading socket -> ");
            }
            exit(-1);
        }
        if ((unsigned char)byte == FEND){
            no_frame = false;
        }else if (transpose == true){
            if ((unsigned char) byte == TFEND){
                buffer[i] = FEND;
            }else if ((unsigned char) byte == TFESC){
                buffer[i] = FESC;
            }
            transpose = false;
            i++;
        }else if ((unsigned char) byte == FESC){
            transpose = true;
        }else{
            buffer[i] = byte;
            i++;
        }
    }
    return i;
}
#else

int read_kiss_from_socket(int fd, char * buffer)
{
    return (read(fd, buffer, 255));
}


#endif

int socket_init(int port){
    int fd;
    struct hostent *he;
    /* estructura que recibirá información sobre el nodo remoto */
    struct sockaddr_in server;
    /* información sobre la dirección del servidor */
    if ((he=gethostbyname("localhost"))==NULL){
        /* llamada a gethostbyname() */
        perror("gethostbyname() error\n");
        return -1;
    }

    if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){
        /* llamada a socket() */
        printf("socket() error\n");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    /* htons() es necesaria nuevamente ;-o */
    server.sin_addr = *((struct in_addr *)he->h_addr);
    /*he->h_addr pasa la información de ``*he'' a "h_addr" */
    bzero(&(server.sin_zero),8);

    if(connect(fd, (struct sockaddr *)&server,
               sizeof(struct sockaddr))==-1){
        /* llamada a connect() */
        printf("connect() error\n");
        return -1;
    }
    return fd;
}
