#ifndef __MatesErrors_H__
#define __MatesErrors_H__

typedef enum {
  MATES_ERROR_NONE,
  MATES_ERROR_COMMAND_FAILED,
  MATES_ERROR_COMMAND_TIMEOUT,
  MATES_ERROR_RESPONSE_TIMEOUT,
  MATES_ERROR_NOT_INITIALIZED
} MatesError;

#endif
//__MatesErrors_H__
