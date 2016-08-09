#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

typedef enum {
    SYSTEM,
    SESSION,
} scope_t;

class SingleInstance
{
public:
    static bool unique(QString key, scope_t scope);
};

#endif // SINGLEINSTANCE_H
