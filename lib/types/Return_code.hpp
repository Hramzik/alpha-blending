#ifndef RETURN_CODE_HPP_INCLUDED
#define RETURN_CODE_HPP_INCLUDED








enum  Return_code  {

    SUCCESS      = 0,
    MEMORY_ERR,
    BAD_ARGS,
    FILE_ERR,
    LIB_ERR,
    CRITICAL,
    SPECIAL_CASE,
};



#define LOG_ERROR(code) _log_error (code,    __FILE__, __PRETTY_FUNCTION__, __LINE__)


#define try(x, ...) _try_inner##__VA_OPT__ (_with_args) (x, __VA_ARGS__)

#define _try_inner(x, ...) {\
\
    Return_code _return_code_try_special = (x);\
    if (_return_code_try_special) { LOG_ERROR (_return_code_try_special); return _return_code_try_special; }\
}

#define _try_inner_with_args(x, ...) {\
\
    Return_code _return_code_try_special = (x);\
    if (_return_code_try_special) { LOG_ERROR (_return_code_try_special); return __VA_ARGS__; }\
}

#define try_return(x, return_value) {\
\
    Return_code _return_code_try_special = (x);\
    if (_return_code_try_special) { LOG_ERROR (_return_code_try_special); return return_value; }\
}


//--------------------------------------------------
//help macros vsc
#endif