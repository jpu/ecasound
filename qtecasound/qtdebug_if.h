#ifndef _QTDEBUG_IF_H
#define _QTDEBUG_IF_H

#include <string>
#include <iostream.h>

#include <kvutils.h>

#include <eca-debug.h>

class QTDEBUG_IF : public MAINDEBUG {
private:

     int debug_level;
    
public:

    void flush(void) { }
    void set_debug_level(int level);
    int get_debug_level(void) { return(debug_level); }

    QTDEBUG_IF(void);

    void control_flow(const string& part);
    void msg(const string& info);
    void msg(int level, const string& info);
};

extern QTDEBUG_IF qtdebug_if;
extern COMMAND_QUEUE qtdebug_queue;

#endif






