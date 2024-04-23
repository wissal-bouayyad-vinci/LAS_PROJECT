#ifndef MESSAGES_H_
#define MESSAGES_H_

#define MAX_PSEUDO 256

#define INSCRIPTION_REQUEST 10
#define INSCRIPTION_OK 11
#define INSCRIPTION_KO 12
#define START_GAME 13
#define CANCEL_GAME 14
#define NUMERO_TUILE 15
#define PLACEMENT_TERMINE 16
#define RANKING 17

/* struct message used between server and client */
typedef struct
{
  char messageText[MAX_PSEUDO];
  int tuile;
  int code;
} StructMessage;

#endif
