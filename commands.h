#ifndef COMMANDS_H
#define COMMANDS_H

void processCommand(const char *input, void (*appendOutput)(const char *));

#endif