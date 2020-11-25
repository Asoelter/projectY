#ifndef ERROR_PRINTER_H
#define ERROR_PRINTER_H

#include "ywin.h"

void printError(HRESULT result)
{
    if (FAILED(result))
    {
        std::string msg = "unknown error";

        if (result== E_INVALIDARG)
        {
            msg = "invalid arg";
        }
        else if (result == E_POINTER)
        {
            msg = "invalid pointer";
        }
        else if (result == E_HANDLE)
        {
            msg = "invalid handle";
        }
        else if (result == E_ABORT)
        {
            msg = "aborted";
        }
        else if (result == E_UNEXPECTED)
        {
            msg = "unexpected failure";
        }
        else if (result == E_FAIL)
        {
            msg = "failed";
        }

        MessageBox(NULL, msg.c_str(), msg.c_str(), 0);
    }
    else
    {
        MessageBox(NULL, "no error", "no error", 0);
    }
}

#endif //ERROR_PRINTER_H
