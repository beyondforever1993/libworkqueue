/**
* @file simple test example
* @author Karl Hiramoto <karl@hiramoto.org>
* @brief simple example showing how to doing a polling job that reschedules
* @brief itself to do something every X ammount of time.
*
* Example code is Distributed under Dual BSD / LGPL licence
* Copyright 2009 Karl Hiramoto
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#if !defined(WINDOWS)
#define WINDOWS
#endif

#pragma once
#include "windows.h"            // big fat windows lib
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#endif

#include "workqueue.h"

static struct workqueue_ctx *ctx;

void callback_func(void *data)
{
	int *counter = (int*) data;
	int ret;
	(*counter)++;
	printf("counter=%d\n",*counter);
	sleep(1);

	/* NOTE This kind of function to do polling every X ammount of time */

	/* reschedule myself */
	if (*counter < 20)
		ret = workqueue_add_work(ctx, 2, 3000,
			callback_func, counter);

	if (ret >= 0) {
		printf("Added job %d \n", ret);
	} else {
		printf("Error adding job err=%d\n", ret);
	}

}

int main(int argc, char *argv[]) {
	
	int counter = 0;
	int i;
	int ret;
	printf("starting\n");
	ctx = workqueue_init(32, 1);

	ret = workqueue_add_work(ctx, 2, 2000,
		callback_func, &counter);

	workqueue_show_status(ctx, stdout);

	sleep(30);

	for (i = 20; i && (ret = workqueue_get_queue_len(ctx)); i--) {
	  	printf("waiting for %d jobs \n", ret);
		sleep(1);
	}

	workqueue_destroy(ctx);

	#ifdef WINDOWS
	system("pause");
	#endif
	return 0;
}