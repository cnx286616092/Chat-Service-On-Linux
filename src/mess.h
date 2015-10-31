/*
 * mess.h
 *
 *  Created on: May 3, 2015
 *      Author: XIsolated
 */

#ifndef SRC_MESS_H_
#define SRC_MESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define REGEX	"SENDTO:(.*).!.FROM:(.*).!.*?"
#define TEST  	"SENDTO:NANCY.!.FROM:CNX.!.TIME:2015/05/03.!.MESSAGE:HELLO WORLD!!"
#define REGNUM (2)

extern void mess_init(void);
extern void mess_prce(const char* ,char** ,char** );
extern void mess_des(void);

#endif /* SRC_MESS_H_ */
