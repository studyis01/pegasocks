#ifndef _CRM_LOG
#define _CRM_LOG

#include "crm_mpsc.h"
#include "stdio.h"

#define MAX_MSG_LEN 1024
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define crm_logger_debug(logger, ...)                                          \
	crm_logger_log(DEBUG, logger, __VA_ARGS__);
#define crm_logger_info(logger, ...) crm_logger_log(INFO, logger, __VA_ARGS__);
#define crm_logger_warn(logger, ...) crm_logger_log(WARN, logger, __VA_ARGS__);
#define crm_logger_error(logger, ...)                                          \
	crm_logger_log(ERROR, logger, __VA_ARGS__);

typedef struct crm_logger_s crm_logger_t;
typedef enum { DEBUG, INFO, WARN, ERROR } LOG_LEVEL;
typedef struct crm_logger_msg_s crm_logger_msg_t;
typedef struct crm_logger_server_s crm_logger_server_t;

void crm_logger_debug_buffer(crm_logger_t *logger, unsigned char *buf,
			     int size);

struct crm_logger_s {
	crm_mpsc_t *mpsc;
	LOG_LEVEL level;
	crm_tid tid;
};

struct crm_logger_msg_s {
	char *msg;
	crm_tid tid;
};

struct crm_logger_server_s {
	crm_logger_t *logger;
	FILE *output;
};

crm_logger_t *crm_logger_new(crm_mpsc_t *mpsc, LOG_LEVEL level);

void crm_logger_free(crm_logger_t *logger);

// for client, construct and send string to mpsc
void crm_logger_log(LOG_LEVEL level, crm_logger_t *logger, const char *fmt,
		    ...);

crm_logger_server_t *crm_logger_server_new(crm_logger_t *logger, FILE *output);

void crm_logger_server_free(crm_logger_server_t *server);

// drain log and write to output
void crm_logger_server_serve(crm_logger_server_t *server);

crm_logger_msg_t *crm_logger_msg_new(char *msg, crm_tid tid);
void crm_logger_msg_free(crm_logger_msg_t *lmsg);

// start point, will run in a seperate thread
void *start_logger(void *logger);

#endif
