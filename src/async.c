#include "vim.h"

#ifdef FEAT_ASYNC
    void
insert_timeout(timeout_T *to) {
    timeout_T *cur = timeouts;
    timeout_T *prev = NULL;

    if (timeouts == NULL) {
        timeouts = to;
        return;
    }
    while (cur != NULL) {
        if (cur->tm > to->tm) {
            if (prev) {
                prev->next = to;
            } else {
                timeouts = to;
            }
            to->next = cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

    void
call_timeouts() {
    struct timeval now;
    gettimeofday(&now, NULL);
    unsigned long tm = now.tv_sec * 1000 + now.tv_usec/1000;
    timeout_T *tmp;

    while (timeouts != NULL && timeouts->tm < tm) {
        call_func_retnr(timeouts->cmd, 0, 0, FALSE);
        tmp = timeouts;
        timeouts = timeouts->next;
        if (tmp->interval == -1) {
            free(tmp->cmd);
            free(tmp);
        } else {
            tmp->tm = tm + tmp->interval;
            insert_timeout(tmp);
        }
    }
}

#endif
