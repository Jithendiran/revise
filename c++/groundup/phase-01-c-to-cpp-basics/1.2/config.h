#ifndef CONFIG_H
#define CONFIG_H

// A shared structure definition
struct DatabaseConfig {
    int port;
};

// A function declaration (prototype) — tells the compiler this exists *somewhere*
void print_config(struct DatabaseConfig* cfg);

#endif